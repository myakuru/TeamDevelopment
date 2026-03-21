// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "../../../System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h"
#include "../../../System/Subsystem/WorldSubsystem/GameProgressSubsystem/GameProgressSubsystem.h"



// Sets default values
AEnemyBase::AEnemyBase()
	:	EnemyManager(nullptr)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	//　敵管理クラスの情報取得
	EnemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();

	//　敵が生成された際に敵管理クラス経由でリストへ登録する
	if (EnemyManager) {
		EnemyManager->RegisterEnemy(this);
	}

	//　ゲームの進行管理クラスの情報取得
	GameProgress = GetWorld()->GetSubsystem<UGameProgressSubsystem>();

	//　ゲームの進行の進行に合わせて敵パラメータを更新
	UpdateParams();
}

void AEnemyBase::MoveToPlayer(const FVector& PlayerLocation, float DeltaTime)
{
	EnemyStatus.MoveDir = PlayerLocation - GetActorLocation();
	EnemyStatus.MoveDir.Normalize();
	
	//　移動方向へ補間する回転を計算
	const FRotator calcResultRotation = CalculateRotationToMoveDirection(
										GetActorRotation(),
										EnemyStatus.MoveDir.Rotation(),
										EnemyStatus.RotationInterpSpeed,
										DeltaTime);

	//　回転を更新
	SetActorRotation(calcResultRotation);

	//　座標を更新
	SetActorLocation(CalculateNextActorLocation(EnemyStatus.MoveDir,EnemyStatus.MoveSpeed,DeltaTime), true);
}

void AEnemyBase::UpdateParams()
{
	if (!GameProgress) { return; }

	//　倒した敵数を元に
	const int32 killCount = GameProgress->GetKillCount();

	//　ヒットポイントの更新
	EnemyStatus.FinalHP = EnemyStatus.HPScaling.GetFinalValue(killCount);

	//　攻撃力の更新
	EnemyStatus.FinalAttack = EnemyStatus.AttackScaling.GetFinalValue(killCount);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::OnDeath()
{
	//　敵が死んだ際に敵管理クラス経由でリストから自身を削除する
	if (EnemyManager) {
		EnemyManager->RemoveEnemy(this);
	}

	//　敵が死んだ際にゲームの進行管理クラス経由で倒した敵数を加算する
	if (GameProgress) {
		GameProgress->AddKillCount();
	}
}

FVector AEnemyBase::CalculateNextActorLocation(const FVector& MoveDir, float Speed, float DeltaTime)
{
	return GetActorLocation() + MoveDir * Speed * DeltaTime;
}

FRotator AEnemyBase::CalculateRotationToMoveDirection(const FRotator& CurrentRotation, const FRotator& TargetRotation, float RotationInterpSpeed, float DeltaTime)
{
	return FMath::RInterpTo(CurrentRotation,
							TargetRotation,
							DeltaTime,
							RotationInterpSpeed);
}

