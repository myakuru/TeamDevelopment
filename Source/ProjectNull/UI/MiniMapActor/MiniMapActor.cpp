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

	// キャプチャ設定の最適化
	SceneCaptureComponent->bCaptureEveryFrame = true;
	SceneCaptureComponent->bCaptureOnMovement = false;

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

