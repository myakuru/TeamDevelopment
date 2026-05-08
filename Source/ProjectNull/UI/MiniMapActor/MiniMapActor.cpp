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

	// 上から見た視点のカメラを作成
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	RootComponent = SceneCaptureComponent;

	// カメラの回転を設定
	SceneCaptureComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));


	// 平面投影に設定
	SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;

	SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

	// 映すものを制限するためのShowFlagsの設定

	SceneCaptureComponent->ShowFlags.SetFog(false);              // フォグを無効化
	SceneCaptureComponent->ShowFlags.SetAtmosphere(false);       // 大気を無効化
	SceneCaptureComponent->ShowFlags.SetVolumetricFog(false);    // 体積フォグを無効化
	SceneCaptureComponent->ShowFlags.SetMotionBlur(false);       // モーションブラーを無効化
	SceneCaptureComponent->ShowFlags.SetBloom(false);            // ブルームを無効化
	SceneCaptureComponent->ShowFlags.SetAmbientOcclusion(false); // アンビエントオクルージョンを無効化
	SceneCaptureComponent->ShowFlags.SetAntiAliasing(false);     // アンチエイリアシングを無効化
	SceneCaptureComponent->ShowFlags.SetEyeAdaptation(false);    // 露出を無効化
	SceneCaptureComponent->ShowFlags.SetTemporalAA(false);       // Temporal AAを無効化

	// ========================================
	// シャドウ関連の設定
	// ========================================
	SceneCaptureComponent->ShowFlags.SetDynamicShadows(false);      // 動的シャドウ
	SceneCaptureComponent->ShowFlags.SetContactShadows(false);      // 接触シャドウ
	SceneCaptureComponent->ShowFlags.SetCapsuleShadows(false);      // カプセルシャドウ
	SceneCaptureComponent->ShowFlags.SetShadowFrustums(false);      // シャドウフラスタム

	// ライディング関連の設定
	SceneCaptureComponent->ShowFlags.SetLighting(false);			 // ライティングを無効化
	SceneCaptureComponent->ShowFlags.SetReflectionEnvironment(false); // 環境反射を無効化
	SceneCaptureComponent->ShowFlags.SetGlobalIllumination(false);    // グローバルイルミネーションを無効化

	// パーティクル関連の設定
	SceneCaptureComponent->ShowFlags.SetParticles(false);  // パーティクルを無効化
	SceneCaptureComponent->ShowFlags.SetDecals(false);    // デカールを無効化
	
	// LOD関連の設定
	SceneCaptureComponent->ShowFlags.SetLOD(false);

	// その他の設定
	SceneCaptureComponent->ShowFlags.SetScreenSpaceReflections(false); // SSRを無効化
	SceneCaptureComponent->ShowFlags.SetAmbientCubemap(false);         // 環境キューブマップを無効化
	SceneCaptureComponent->ShowFlags.SetDistanceFieldAO(false);        // DFAOを無効化

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
		// 見つからなかったら再度取得
		TargetCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}

}

