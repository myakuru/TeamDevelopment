// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

// Sets default values
AMiniMapActor::AMiniMapActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// キャプチャコンポーネントの作成
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	RootComponent = SceneCaptureComponent;

	// 真下を向く
	SceneCaptureComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	// ========================================
	// ミニマップ最適化設定
	// ========================================

	// 投影設定 - 正投影（Orthographic）でフラットな表示
	SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;

	SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

	// 描画設定 - 簡易的な表示
	SceneCaptureComponent->ShowFlags.SetFog(false);              // フォグ無効
	SceneCaptureComponent->ShowFlags.SetAtmosphere(false);       // 大気無効
	SceneCaptureComponent->ShowFlags.SetVolumetricFog(false);    // ボリュメトリックフォグ無効
	SceneCaptureComponent->ShowFlags.SetMotionBlur(false);       // モーションブラー無効
	SceneCaptureComponent->ShowFlags.SetBloom(false);            // ブルーム無効
	SceneCaptureComponent->ShowFlags.SetAmbientOcclusion(false); // アンビエントオクルージョン無効
	SceneCaptureComponent->ShowFlags.SetAntiAliasing(false);     // アンチエイリアス無効（軽量化）
	SceneCaptureComponent->ShowFlags.SetEyeAdaptation(false);    // 目の順応無効
	SceneCaptureComponent->ShowFlags.SetTemporalAA(false);       // TAA無効

	// ========================================
	// 影を完全に無効化
	// ========================================
	SceneCaptureComponent->ShowFlags.SetDynamicShadows(false);      // 動的影無効
	SceneCaptureComponent->ShowFlags.SetContactShadows(false);      // コンタクトシャドウ無効
	SceneCaptureComponent->ShowFlags.SetCapsuleShadows(false);      // カプセルシャドウ無効
	SceneCaptureComponent->ShowFlags.SetShadowFrustums(false);      // シャドウフラスタム無効

	// ライティングを簡素化（オプション：完全フラットにしたい場合はLightingも無効化）
	SceneCaptureComponent->ShowFlags.SetLighting(false); // ライティング完全無効
	SceneCaptureComponent->ShowFlags.SetReflectionEnvironment(false); // 反射無効
	SceneCaptureComponent->ShowFlags.SetGlobalIllumination(false);    // GI無効

	// パーティクルやデカールの表示（必要に応じて調整）
	SceneCaptureComponent->ShowFlags.SetParticles(false);  // パーティクル表示
	SceneCaptureComponent->ShowFlags.SetDecals(false);    // デカール非表示

	// LOD設定 - 最低LODを使用してパフォーマンス向上
	SceneCaptureComponent->ShowFlags.SetLOD(false);

	// その他の最適化
	SceneCaptureComponent->ShowFlags.SetScreenSpaceReflections(false); // SSR無効
	SceneCaptureComponent->ShowFlags.SetAmbientCubemap(false);         // アンビエントキューブマップ無効
	SceneCaptureComponent->ShowFlags.SetDistanceFieldAO(false);        // DFAO無効

	// ========================================
	// オプション: 特定のアクターのみ表示
	// ========================================
	// Unreal Engineの「Show Only Actors」機能を使用する場合
	// エディタまたはコードでShowOnlyComponentsを設定可能

	TargetCharacter = nullptr;

}

// Called when the game starts or when spawned
void AMiniMapActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMiniMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetCharacter)
	{
		FVector CharacterLocation = TargetCharacter->GetActorLocation();

		// カメラの高さを考慮して新しい位置を計算
		FVector NewLocation = FVector(CharacterLocation.X, CharacterLocation.Y, CharacterLocation.Z + CameraHeight);

		// カメラの位置を更新
		SetActorLocation(NewLocation);
	}
	else
	{
		// キャラクターが見つからない場合は再取得を試みる
		TargetCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}

}

