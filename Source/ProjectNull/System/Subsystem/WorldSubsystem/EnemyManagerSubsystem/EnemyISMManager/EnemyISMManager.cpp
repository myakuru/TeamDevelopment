#include "EnemyISMManager.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2D.h"
#include "RenderGraphUtils.h"
#include "RenderGraphBuilder.h"
#include "GlobalShader.h"
#include "ShaderParameterUtils.h"
#include "RHICommandList.h"
#include "TextureResource.h"

#include "Engine/StaticMesh.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/Animation/AnimDataAsset.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>

AEnemyISMManager::AEnemyISMManager()
{
	PrimaryActorTick.bCanEverTick = false;	// Tickは必要ないので無効化
	// RootComponentとしてISMを生成
	ISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM"));
	RootComponent = ISM;
	// CustomDataは使わない（マテリアルがAnimStateRTを直接読む）
	//ISM->NumCustomDataFloats = 0;

	// マテリアルに渡すCustomDataのチャンネル数を設定する
	// AnimToTextureのAnimTime・AnimIndexを渡すために最低２チャンネル必要
	ISM->NumCustomDataFloats = NumCustomDataFloats;
}

void AEnemyISMManager::BeginPlay()
{
	Super::BeginPlay();
	
	// エディタで設定したメッシュをISMに適用する
	if (EnemyMesh)
	{
		ISM->SetStaticMesh(EnemyMesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyISMManager] EnemyMesh Null : %s"), *GetName());
	}

	// 
	InitAnimBuffer();

	// アニメーション状態テクスチャの初期化
	InitAnimStateTexture();

	// アニメーション情報テクスチャの初期化（AnimDataAssetから静的に作成）
	InitAnimInfoTexture();

	// マテリアルにAnimStateRTをセット
	// ※全インスタンスが同じマテリアルを使い、インスタンスIndexで別ピクセルを読む
	if (AnimStateRT)
	{
		// ISMのMaterial Slot 0に対して動的変更可能なMaterial Instanceを生成
		UMaterialInstanceDynamic* MID = ISM->CreateAndSetMaterialInstanceDynamic(0);
		if (MID)
		{
			// SetScalarParameterValue
			// マテリアル内のScalar Parameter "○○"に第２引き数を代入する
			// ScalarParameter Name = SampleRateを作り、それを適当なノードにつなげると、今回の場合30.0fが入る
			MID->SetTextureParameterValue(TEXT("AnimStateTexture"), AnimStateRT);
			MID->SetTextureParameterValue(TEXT("AnimInfoTexture"), AnimInfoTexture);
			MID->SetScalarParameterValue(TEXT("MaxInstances"), (float)MaxInstances);
			MID->SetScalarParameterValue(TEXT("SampleRate"), 30.0f);
			MID->SetScalarParameterValue(TEXT("NumAnimations"), (float)AnimDataAsset->Animations.Num());
			MID->SetScalarParameterValue(TEXT("NextAnimIndex"), (float)1.0f); // これはいらないかも
		}
	}

	// 敵管理サブシステムに自身を登録する
	if (auto* Subsystem = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>())
	{
		Subsystem->RegisterISMManager(this);
	}
}

// GPU上でアニメーション状態を保持・更新するためのレンダーターゲットの作成
void AEnemyISMManager::InitAnimStateTexture()
{
	// GPU上で使用するレンダーターゲットを作成（ダブルバッファ）
	// GPUは同じテクスチャーの読み込みと書き込みを同時にできないため、読み込み用と書き込み用を分ける
	// SRVとUAVを作る感じだが、SRVは読み込みしかできないがUAVよりも早い
	// ラムダ式でレンダーターゲット生成関数をその場で作成
	auto CreateAnimStateRT = [this](const TCHAR* Name)
		{
			// UTextureRenderTarget2DはGPUが読み書きできるテクスチャー
			UTextureRenderTarget2D* RT = NewObject<UTextureRenderTarget2D>(this, Name);
			// レンダーターゲットのフォーマット
			// 1ピクセルにfloat型4つを入れれるイメージ
			// R : CurrentFrame, G : PrevFrame, B : NextAnimTime, A : BlendWeight みたいな感じ
			RT->RenderTargetFormat = RTF_RGBA16f;
			// MipMap無効
			// 画像ではなくてデータとして扱うため、値が保管されてしまうと壊れる
			RT->bAutoGenerateMips = false;
			RT->bCanCreateUAV = true;	// GPUから書き込み可能に
			RT->Filter = TF_Nearest;	// Linearだと保管されるためNearest
			RT->InitAutoFormat(MaxInstances, 2);	// MaxInstances列、2行のイメージ
			RT->UpdateResourceImmediate(true);	// GPUリソース生成
			return RT;
		};

	// ラムダで作った関数をもとに、AとBのレンダーターゲット（テクスチャー）を作成
	AnimStateRT = CreateAnimStateRT(TEXT("AnimStateRT"));
}

// 各アニメーションのメタ情報をGPUから読めるテクスチャーに変換（固定LoopupTable）
void AEnemyISMManager::InitAnimInfoTexture()
{
	if (!AnimDataAsset) { return; }

	// アニメーション数の取得
	const int32 NumAnims = AnimDataAsset->Animations.Num();
	if (NumAnims == 0) { return; }

	// Width = アニメ数、Height = 1でテクスチャーを生成
	// PF_A32B32G32R32Fは1ピクセルに4つ分のfloatを入れれる
	AnimInfoTexture = UTexture2D::CreateTransient(NumAnims, 1, PF_A32B32G32R32F);
	AnimInfoTexture->Filter = TF_Nearest; // ポイントサンプリング（データなので補間しない）

	// ロックして書き込み
	// テクスチャーの生データをアクセス
	FTexture2DMipMap& Mip = AnimInfoTexture->GetPlatformData()->Mips[0];
	// Mip.BulkData.Lock(LOCK_READ_WRITE)でCPUがテクスチャーメモリを編集可能に
	// FLinearColor化することで、CPU側でのデータの扱いかたを決めている（float型4つ）
	FLinearColor* Data = static_cast<FLinearColor*>(Mip.BulkData.Lock(LOCK_READ_WRITE));
	for (int32 i = 0; i < NumAnims; ++i)
	{
		Data[i] = FLinearColor(
			AnimDataAsset->Animations[i].StartTime,
			AnimDataAsset->Animations[i].NumFrames,
			AnimDataAsset->Animations.Num(),
			0.0f
		);
	}

	// 編集終了
	Mip.BulkData.Unlock();
	// CPUからGPUへ転送
	AnimInfoTexture->UpdateResource();
}

void AEnemyISMManager::InitAnimBuffer()
{
	// 初期データをCPU側で作成
	// インスタンス数分、配列を確保して0で初期化する
	TArray<FGPUAnimState> InitData;
	InitData.SetNumZeroed(MaxInstances);

	// RDGPoolBufferとして作成
	// ゲームスレッドからは直接触れないのでレンダースレッドに投げる
	ENQUEUE_RENDER_COMMAND(InitAnimBuffer)(
		[this, InitData](FRHICommandListImmediate& RHICmdList)
		{
			// RDGBuilderを作成（Render Dependency Graph）
			// GPU処理の依存関係、読み書き、同期、バリアをUEに管理させる仕組み
			FRDGBuilder GraphBuilder(RHICmdList);

			// リクエストをGPUバッファにアップロード
			// CPU側のRequestCopyをGPUで読めるバッファにアップロード
			// TArray<FGPUAnimState>をGPU StructuredBufferに
			FRDGBufferRef Buffer = CreateStructuredBuffer(
				GraphBuilder,
				TEXT("AnimStateBuffer"),
				sizeof(FGPUAnimState),
				MaxInstances,
				InitData.GetData(),
				sizeof(FGPUAnimState) * MaxInstances
			);
			// PoolBufferとして外に持ち出す
			GraphBuilder.QueueBufferExtraction(
				Buffer,
				&AnimStatePoolBuffer
			);

			GraphBuilder.Execute();
		});
}

void AEnemyISMManager::RequestAnimChange(int32 InstanceIndex, int32 NextAnimIndex, bool bLooping, float BlendSpeed)
{
	// ゲームスレッドからリクエストをキューに積む
	// DispatchAnimUpdateで一括処理する
	FGPUAnimChangeRequest Req;
	Req.InstanceIndex = InstanceIndex;
	Req.NextAnimIndex = NextAnimIndex;
	Req.bLooping = bLooping ? 1 : 0;
	Req.BlendSpeed = BlendSpeed;
	PendingChangeRequests.Add(Req);
}

int32 AEnemyISMManager::AllocateIndex()
{
	if (FreeIndices.Num() > 0)
	{
		// 削除で空いた穴を再利用することでインスタンス総数を増やさない
		return FreeIndices.Pop();
	}

	// 穴がない場合のみ末尾にインスタンスを追加する
	ISM->AddInstance(FTransform::Identity);
	return MaxInstanceCount++;
}

void AEnemyISMManager::ReleaseIndex(int32 Index)
{
	// RemoveInstanceを呼ぶと後ろのインデックスがずれるため呼ばない
	// 代わりにスケール0で負荷しにしてFreeListに積む
	FTransform Hidden;
	Hidden.SetScale3D(FVector::ZeroVector);
	ISM->UpdateInstanceTransform(Index, Hidden, true, false);

	FreeIndices.Add(Index);

	// 描画を更新してインスタンスの削除を反映させる
	ISM->MarkRenderStateDirty();
}

void AEnemyISMManager::RegisterEnemy(AEnemyBase* Enemy)
{
	if (!IsValid(Enemy))
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyISMManager] RegisterEnemy: Enemy is invalid"));
		return;
		//return INDEX_NONE;
	}

	// 既に登録済みの場合は一度解除してから再登録する
	// Activate→Deactivate→Activateのサイクルで
	// 前回のインデックスが残っている場合の対処
	if (Enemy->ISMInstanceIndex != INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ISMManager] RegisterEnemy: 既存インデックスを解除して再登録: %d"), Enemy->ISMInstanceIndex);
		ReleaseIndex(Enemy->ISMInstanceIndex);
		Enemy->ISMInstanceIndex = INDEX_NONE;
	}

	FTransform AdjustTransform = Enemy->GetActorTransform();
	// 90度回転させる
	FRotator Rotation = AdjustTransform.GetRotation().Rotator();
	Rotation.Yaw -= 90.0f;
	AdjustTransform.SetRotation(Rotation.Quaternion());

	FVector Location = AdjustTransform.GetLocation();
	Location.Z -= 90.0f;
	AdjustTransform.SetLocation(Location);


	// 空いているインデックスを確保してEnemyに持たせる
	// Enemy自身がインデックスを知っているのでマイフレームのFindが不要になる
	int32 Index = AllocateIndex();
	Enemy->ISMInstanceIndex = Index;
	
	ISM->UpdateInstanceTransform(
		Index,
		AdjustTransform,
		true,
		false	// MarkRenderStateDirtyをfalseにしてレンダリング状態の更新を遅延させる
	);

	Enemies.Add(Enemy);
}

void AEnemyISMManager::UnregisterEnemy(AEnemyBase* Enemy)
{
	if(!IsValid(Enemy))
	{
		//UE_LOG(LogTemp, Warning, TEXT("[EnemyISMManager] UnregisterEnemy: Enemy is invalid"));
		return;
	}

	// INDEX_NONEは未登録またはすでに解除済みを意味する
	// 二重登録を防ぐためチェック
	if (Enemy->ISMInstanceIndex == INDEX_NONE) { return; }

	// インスタンスをISMから解除してインデックスを解放する
	ReleaseIndex(Enemy->ISMInstanceIndex);

	// インスタンスを解放してEnemyからインデックスをクリアする
	Enemy->ISMInstanceIndex = INDEX_NONE;
	Enemies.Remove(Enemy);
}

void AEnemyISMManager::UpdateEnemies(float DeltaTime)
{
	if (Enemies.Num() == 0) { return; }

	// ② CSをDispatchしてGPU側のAnimStateRTを更新
	DispatchAnimUpdate(DeltaTime);

	// ③ TransformだけCPUから更新（当たり判定の座標と同期するため）
	for (AEnemyBase* Enemy : Enemies)
	{
		if (!IsValid(Enemy) || Enemy->ISMInstanceIndex == INDEX_NONE) { continue; }

		// ISMManagerのTransform補正（90度回転・Z補正）
		FTransform AdjustedTransform = Enemy->GetActorTransform();
		FRotator Rot = AdjustedTransform.GetRotation().Rotator();
		Rot.Yaw -= 90.0f;
		AdjustedTransform.SetRotation(Rot.Quaternion());

		FVector Loc = AdjustedTransform.GetLocation();
		Loc.Z -= 90.0f;
		AdjustedTransform.SetLocation(Loc);

		ISM->UpdateInstanceTransform(
			Enemy->ISMInstanceIndex,
			AdjustedTransform,
			true,
			false // まとめてDirtyを立てる
		);

		ISM->SetCustomDataValue(Enemy->ISMInstanceIndex, 0, Enemy->ISMInstanceIndex);
		ISM->SetCustomDataValue(Enemy->ISMInstanceIndex, 1, Enemy->ISMInstanceIndex);
	}

	ISM->MarkRenderStateDirty();

	// ④ 処理済みのリクエストをクリア
	PendingChangeRequests.Reset();

	DebugReadbackAnimStateRT();
}

// デバッグ用：AnimStateRTの中身をCPUに読み戻してログに出す
void AEnemyISMManager::DebugReadbackAnimStateRT()
{
	if (!AnimStateRT) { return; }

	FTextureRenderTargetResource* RTResource =
		AnimStateRT->GameThread_GetRenderTargetResource();

	TArray<FFloat16Color> Pixels;
	RTResource->ReadFloat16Pixels(Pixels);

	const int32 Width = AnimStateRT->SizeX;
	const int32 Height = AnimStateRT->SizeY;

	if (Pixels.Num() >= Width * Height && Height > 1)
	{
		const FFloat16Color Row0 = Pixels[0 + 0 * Width];
		const FFloat16Color Row1 = Pixels[0 + 1 * Width];

		UE_LOG(LogTemp, Warning,
			TEXT("[AnimStateRT] Inst0 Row0: R=%.2f G=%.2f B=%.2f A=%.2f"),
			(float)Row0.R, (float)Row0.G, (float)Row0.B, (float)Row0.A);

		UE_LOG(LogTemp, Warning,
			TEXT("[AnimStateRT] Inst0 Row1: R=%.2f G=%.2f B=%.2f A=%.2f"),
			(float)Row1.R, (float)Row1.G, (float)Row1.B, (float)Row1.A);
	}

	static int kari = 0;
	if (kari > 1) { return; }
	kari++;
	if (!AnimInfoTexture) { return; }

	FTexture2DMipMap& Mip = AnimInfoTexture->GetPlatformData()->Mips[0];
	FLinearColor* Data = static_cast<FLinearColor*>(
		Mip.BulkData.Lock(LOCK_READ_ONLY));

	const int32 NumAnims = AnimDataAsset->Animations.Num();
	for (int32 i = 0; i < NumAnims; ++i)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[AnimInfoTexture] Anim[%d]: StartTime=%.2f NumFrames=%.2f"),
			i,
			(float)Data[i].R,
			(float)Data[i].G);
	}



	Mip.BulkData.Unlock();
}

// ReadRT → ComputeShader → WriteRT → Material
void AEnemyISMManager::DispatchAnimUpdate(float DeltaTime)
{
	if (!AnimStateRT) { return; }

	const int32 NumInstances = Enemies.Num();
	const int32 NumRequests = PendingChangeRequests.Num();

	// ENQUEUE_RENDER_COMMANDでRenderTargetに渡すさい
	// PendingChangeRequestsに変更があったら困るので、ローカルにコピーしたものを渡す
	TArray<FGPUAnimChangeRequest> RequestsCopy = PendingChangeRequests;

	// UTextureRenderTarget2DからRenderTargetResourceを取得
	// UTextureRenderTarget2DはUObject（Unrealのゲームオブジェクト）のため
	// もっと低レベルなGPUリソースが必要
	FTextureRenderTargetResource* RTResource = AnimStateRT->GameThread_GetRenderTargetResource();

	// RHITextureを取得
	// FTextureRenderTargetResourceからFTextureRHIRefを取得
	// CSにはこのFTextureRHIRefを渡す
	FTextureRHIRef AnimStateRTRHI = RTResource->GetRenderTargetTexture();

	// AnimInfoTexture用のRHIも受け取る
	FTextureRHIRef AnimInfoTextureRHI = AnimInfoTexture->GetResource()->TextureRHI;

	// ここまでで、GPU側で処理を行うためのリソースの準備

	// GameThreadで準備したTextureをRenderThreadに渡して
	// RDG + ComputeShaderで実際にGPUで更新する部分

	// このラムダ式の中身をRenderThreadに投げる
	ENQUEUE_RENDER_COMMAND(AnimUpdate)(
		[this, DeltaTime, NumInstances, NumRequests, RequestsCopy, AnimStateRTRHI,
		AnimInfoTextureRHI](FRHICommandListImmediate& RHICmdList)
		{
			// RDGBuilderを作成（Render Dependency Graph）
			// GPU処理の依存関係、読み書き、同期、バリアをUEに管理させる仕組み
			FRDGBuilder GraphBuilder(RHICmdList);

			// AnimStateRTをRDGに登録
			// ReadTextureRHIをRDG内で使える形に登録
			// FTextureRHIRefからRDG Textureに
			FRDGBufferRef AnimStateBuf = GraphBuilder.RegisterExternalBuffer(AnimStatePoolBuffer,TEXT("AnimStateBuffer"));

			// UAVを作る
			FRDGBufferUAVRef AnimStateUAV = GraphBuilder.CreateUAV(AnimStateBuf);

			// Pass 1: アニメ変更リクエストの適用（リクエストがある時だけ）
			if (NumRequests > 0)
			{
				//UE_LOG(LogTemp, Warning, TEXT("NumRequests: %d"), PendingChangeRequests.Num());

				// リクエストをGPUバッファにアップロード
				// CPU側のRequestCopyをGPUで読めるバッファにアップロード
				// TArray<FGPUAnimChangeRequest>をGPU StructuredBufferに
				FRDGBufferRef RequestBuffer = CreateStructuredBuffer(
					GraphBuilder,
					TEXT("ChangeRequestBuffer"),
					sizeof(FGPUAnimChangeRequest),
					NumRequests,
					RequestsCopy.GetData(),
					sizeof(FGPUAnimChangeRequest) * NumRequests
				);
				// ComputeShaderがこのリクエストバッファを読むためにSRVに
				FRDGBufferSRVRef RequestSRV = GraphBuilder.CreateSRV(RequestBuffer);

				// ComputeShader1に渡すパラメータ構造体をRDG上に確保
				FApplyChangeRequestCS::FParameters* Pass1Params =
					GraphBuilder.AllocParameters<FApplyChangeRequestCS::FParameters>();

				// シェーダーに渡す値をセット
				Pass1Params->GPUAnimStateBuffer = AnimStateUAV;				// 変更前の状態を読む用
				Pass1Params->ChangeRequestBuffer = RequestSRV;					// 変更要求一覧
				Pass1Params->ChangeRequestCount = (uint32)NumRequests;	// 変更要求数
				Pass1Params->InstanceCount = (uint32)NumInstances;				// インスタンス総数

				// Pass1のComputeShaderを指定
				// FApplyChangeRequestCSというGlobalShaderを取得
				TShaderMapRef<FApplyChangeRequestCS> Pass1CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
				// スレッドグループ数を決定
				const int32 Pass1Groups = FMath::CeilToInt((float)NumRequests / 64.0f);

				// RDGにこのComputeShaderを後で実行するように登録
				// GraphBuilder.Execute()で実行
				FComputeShaderUtils::AddPass(
					GraphBuilder,
					RDG_EVENT_NAME("ApplyChangeRequest"),
					Pass1CS,
					Pass1Params,
					FIntVector(Pass1Groups, 1, 1)
				);
			}

			//------------------------------------------------------
			// Pass 2: アニメ状態の更新（毎フレーム）
			//------------------------------------------------------

			FRDGTextureRef AnimStateRTRDG = GraphBuilder.RegisterExternalTexture(
				CreateRenderTarget(
					AnimStateRTRHI,
					TEXT("AnimStateRT")
				)
			);

			// AnimInfoTextureをRDGに登録,読み取り専用なのでSRVのみ作成
			FRDGTextureRef AnimInfoRDG = GraphBuilder.RegisterExternalTexture(
				CreateRenderTarget(
					AnimInfoTextureRHI,
					TEXT("AnimInfoTex")
				)
			);
			FRDGTextureSRVRef AnimInfoSRV = GraphBuilder.CreateSRV(AnimInfoRDG);

			FRDGTextureUAVRef AnimStateRTUAV = GraphBuilder.CreateUAV(AnimStateRTRDG);

			FAnimUpdateCS::FParameters* Pass2Params = GraphBuilder.AllocParameters<FAnimUpdateCS::FParameters>();

			Pass2Params->DeltaTime = DeltaTime;
			Pass2Params->SampleRate = 30.0f;
			Pass2Params->InstanceCount = (uint32)NumInstances;
			Pass2Params->MaxInstances = (uint32)MaxInstances;
			Pass2Params->AnimStateTextureReadWrite = AnimStateRTUAV;
			Pass2Params->AnimInfoTexture = AnimInfoSRV;
			Pass2Params->GPUAnimStateBuffer = AnimStateUAV;

			TShaderMapRef<FAnimUpdateCS> Pass2CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
			const int32 Pass2Groups = FMath::CeilToInt((float)NumInstances / 64.0f);
			{
				/* CeilToInt()・小数点以下を切り上げる */
				// 0.1->1, 0.5->1, 1.0->1, 1.1->2, 3.9->4
			}

			FComputeShaderUtils::AddPass(
				GraphBuilder,
				RDG_EVENT_NAME("AnimUpdate"),
				Pass2CS,
				Pass2Params,
				FIntVector(Pass2Groups, 1, 1)
			);

			// 登録したComputeShaderを実行
			// RDGがUAV→SRVのバリアを自動で挿入してくれるので
			// マテリアルの読み取り時には同期が保証される
			GraphBuilder.Execute();
		});

		UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(ISM->GetMaterial(0));

		if (MID)
		{
			MID->SetTextureParameterValue(TEXT("AnimStateTexture"),AnimStateRT);
			MID->SetTextureParameterValue(TEXT("AnimInfoTexture"), AnimInfoTexture);
		}
}

//// 1〜2フレームのレイテンシが発生するが実用上問題なし
//void UEnemyISMManager::ReadbackFinishedFlags()
//{
//	// GPU→CPUへ非同期コピー
//	FRHIGPUBufferReadback* Readback = new FRHIGPUBufferReadback(TEXT("AnimFinishedReadback"));
//
//	ENQUEUE_RENDER_COMMAND(ReadbackAnimState)(
//		[this, Readback](FRHICommandListImmediate& RHICmdList)
//		{
//			Readback->EnqueueCopy(RHICmdList, AnimStateBufferRHI);
//		});
//
//	// 次フレームで読む（レイテンシ1フレーム）
//	if (Readback->IsReady())
//	{
//		FGPUAnimState* Data = (FGPUAnimState*)Readback->Lock(sizeof(FGPUAnimState) * Enemies.Num());
//		for (int32 i = 0; i < Enemies.Num(); ++i)
//		{
//			if (Data[i].bFinished)
//			{
//				// EnemyRuntimeDataのフラグに反映
//				// StateTreeのTickがGetAnimFinished()で検知する
//			}
//		}
//		Readback->Unlock();
//	}
//}