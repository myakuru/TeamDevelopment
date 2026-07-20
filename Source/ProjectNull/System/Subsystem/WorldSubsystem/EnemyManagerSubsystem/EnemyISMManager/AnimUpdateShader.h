#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

///** GPU側のアニメーション状態*/
//struct FGPUAnimation
//{
//	/** 進行アニメーション時間*/
//	float AnimTime = 0.0f;
//	/** １つ前のアニメーション時間*/
//	float PrevAnimTime = 0.0f;
//	/** 次のアニメーションの開始時間*/
//	float NextAnimTime = 0.0f;
//	/** ブレンド速度*/
//	float BlendSpeed = 0.0f;
//	/** アニメーションブレンドの進行率*/
//	float AnimBlendWeight = 0.0f;
//	/** 現在のアニメーション番号*/
//	int32 AnimIndex = 1;
//	/** 次のアニメーション番号*/
//	int32 NextAnimIndex = 0;
//	int32 AnimRoopFlg = false;
//	/** アニメーションチェンジフラグ*/
//	int32 AnimChangeFlg = false;
//	/** アニメーション終了確認フラグ*/
//	int32 bAnimFinished = false;
//
//	float Padding[2];	// 16バイトアライメント
//};

/**
 * CPU→GPUへのアニメ変更リクエスト
 * HLSL側のFAnimChangeRequestと完全一致させること（16バイト）
 */
struct FGPUAnimChangeRequest
{
	int32   NextAnimIndex = 0;
	float   BlendSpeed = 0.3f;
	int32   bLooping = 1;
	int32   InstanceIndex = 0;
};

/**
* CS内でアニメーションデータの更新にのみ使うデータ
*/
struct FGPUAnimState
{
	float AnimTime = 0.0f;
	float PrevAnimTime = 0.0f;
	float NextAnimTime = 0.0f;
	float BlendWeight = 0.0f;
	int32 AnimIndex = 0;
	int32 NextAnimIndex = 0;
	uint32 Flags = 0;
	float BlendSpeed = 0.3f;
};

/**
 * Pass1: アニメ変更リクエスト適用CS
 */
class FApplyChangeRequestCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FApplyChangeRequestCS);
	SHADER_USE_PARAMETER_STRUCT(FApplyChangeRequestCS, FGlobalShader);

public:

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FGPUAnimChangeRequest>, ChangeRequestBuffer)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<FGPUAnimState>, GPUAnimStateBuffer)
		SHADER_PARAMETER(uint32, ChangeRequestCount)
		SHADER_PARAMETER(uint32, InstanceCount)
		SHADER_PARAMETER(uint32, MaxInstances)
	END_UNIFORM_BUFFER_STRUCT()

	/** 
	* 処理的には必要ない
	* グローバルシェーダーをコンパイルするかどうかを制御する
	* プラットフォーム（Parameters.Platform）がShaderModel5をサポートしている場合のみ、コンパイルしてビルドに含める
	*/
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	/** 古い記述の仕方*/
	//static void ModifyCompilationEnvironment(
	//	EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment)
	//{
	//	FGlobalShader::ModifyCompilationEnvironment(Platform, OutEnvironment);
	//	// Add your own defines for the shader code
	//	OutEnvironment.SetDefine(TEXT("NUM_CUSTOM_DATA"), 8);
	//}
};
/**
 * Pass2: アニメ状態更新CS（毎フレーム）
 */
class FAnimUpdateCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FAnimUpdateCS);
	SHADER_USE_PARAMETER_STRUCT(FAnimUpdateCS, FGlobalShader);

public:

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(float, DeltaTime)
		SHADER_PARAMETER(float, SampleRate)
		SHADER_PARAMETER(uint32, ActiveInstanceCount)
		SHADER_PARAMETER(uint32, MaxInstances)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, AnimStateTextureReadWrite)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, AnimInfoTexture)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<FGPUAnimState>, GPUAnimStateBuffer)
		SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<uint32>, ActiveInstanceIndexBuffer)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
	{
		return IsFeatureLevelSupported(Params.Platform, ERHIFeatureLevel::SM5);
	}

	static void ModifyCompilationEnvironment(
		const FGlobalShaderPermutationParameters& Params,
		FShaderCompilerEnvironment& OutEnv)
	{
		FGlobalShader::ModifyCompilationEnvironment(Params, OutEnv);
		OutEnv.SetDefine(TEXT("THREADS_PER_GROUP"), 64);
	}
};