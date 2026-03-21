// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "../../../System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h"
#include "Components/CapsuleComponent.h"

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

	CapsuleCollision = GetCapsuleComponent();

	// コリジョンプリセット設定
	if (CapsuleCollision)
	{
		CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CapsuleCollision->SetCollisionObjectType(ECC_Pawn);
		CapsuleCollision->SetCollisionResponseToAllChannels(ECR_Block);
		CapsuleCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		CapsuleCollision->SetGenerateOverlapEvents(true);
	}

	// 当たり判定の設定
	{
		// 自身のアクタについているSphereComponentを探す
		CapsuleCollision = FindComponentByClass<UCapsuleComponent>();

		// SphereCollisionの中にデータがあれば｛｝内を実行する
		if (CapsuleCollision)
		{
			// オーバーラップ開始時にonOverlap関数を呼ぶように設定
			CapsuleCollision->OnComponentBeginOverlap.AddDynamic
			(
				this,
				&AEnemyBase::OnOverlap
			);

			UE_LOG(LogTemp, Warning, TEXT("=== AEnemyBase BeginPlay ==="));
		}
	}
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

void AEnemyBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ほかのアクタと重なったら派生先でKnockBack関数を呼び出す
	UE_LOG(LogTemp, Warning, TEXT("=== AEnemyBase OnOverlap ==="));
}

void AEnemyBase::SetKnockBackData(const FVector& PlayerLocation, float AttackPower, float EnemyWeight)
{
	// 吹き飛ばしに使う数値を決める
	int knockBackPowerLevel = AttackPower - EnemyWeight;
	if (knockBackPowerLevel < 0)
	{
		knockBackPowerLevel = 0;
	}
	const FName RowName = FName(*FString::FromInt(knockBackPowerLevel));

	// RowNameから型付で取得
	const FKnockBackData* KnockBackData =
		KnockBackDataTable->FindRow<FKnockBackData>(RowName, TEXT("KnockBack"));
	
	if (!KnockBackData)
	{
		UE_LOG(LogTemp, Warning, TEXT("KnockBack row not found: %s"), *RowName.ToString());
		return;
	}

	// 水平方向
	FVector HorizontalDir = GetActorLocation() - PlayerLocation;
	HorizontalDir.Z = 0.0f;
	HorizontalDir.Normalize();

	// 吹き飛び角度
	const float Rad = FMath::DegreesToRadians(KnockBackData->LaunchAngleDeg);
	// 水平方向の角度とラジアン角をもとに上向きの角度を作る
	FVector LanchDir = HorizontalDir * FMath::Cos(Rad) + FVector::UpVector * FMath::Sin(Rad);
	LanchDir.Normalize();

	EnemyStatus.KNockBackVelocity = LanchDir * KnockBackData->LaunchSpeed;
	EnemyStatus.KnockBackFlg = true;

	UE_LOG(LogTemp, Warning, TEXT("=== ノックバック初期値完了 ==="));
}

void AEnemyBase::MoveToKnockBack(const FVector& KnockBackDir, float KnockBackPower, float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();

	// 重力（仮）
	const float Gravity = -980.0f;

	// 重力を速度に加算
	EnemyStatus.KNockBackVelocity.Z += Gravity * DeltaTime;

	// 位置更新
	FVector NextLocation = CurrentLocation + EnemyStatus.KNockBackVelocity * DeltaTime;

	FHitResult HitResult;
	SetActorLocation(NextLocation, true, &HitResult);

	// どこかに当たったら停止
	if (HitResult.bBlockingHit)
	{
		EnemyStatus.KnockBackFlg = false;
		EnemyStatus.KNockBackVelocity = FVector::ZeroVector;
		UE_LOG(LogTemp, Warning, TEXT("=== ノックバック停止 ==="));
	}
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

