// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapCamera.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMinimapCamera::AMinimapCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// キャプチャコンポーネントの作成
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));

	// キャプチャコンポーネントをルートコンポーネントに設定
	RootComponent = SceneCaptureComponent;

	// 平行投影に設定
	SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
	// 表示範囲の広さ
	SceneCaptureComponent->OrthoWidth = 2048.0f;
	// 真下を向く（-90度）
	SetActorRotation(FRotator(-90.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void AMinimapCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinimapCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetCharacter)
	{
		// プレイヤーキャラクターを取得して追跡する
		TargetCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}

}

