#include "EnemyMidBossBase.h"

#include "Kismet/GameplayStatics.h"
#include "Components\CapsuleComponent.h"
#include "Components\StateTreeComponent.h"

#include <ProjectNull\GameInstance\SuperGameInstance.h>
#include <ProjectNull\Component\EnemyAttackComponent\EnemyAttackComponent.h>
#include <ProjectNull\Data\CharacterRuntimeData\PlayerRuntimeData\PlayerRuntimeData.h>
#include <ProjectNull\Data\CharacterRuntimeData\EnemyMidBossRuntimeData\EnemyMidBossRuntimeData.h>
#include <ProjectNull\System\Subsystem\WorldSubsystem\GameProgressSubsystem\GameProgressSubsystem.h>

#include "GameFramework/CharacterMovementComponent.h"

AEnemyMidBossBase::AEnemyMidBossBase()
	:	EnemyDataAsset(nullptr)
	,	EnemyStatus(FEnemyStatus())
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyMidBossRuntimeData = CreateDefaultSubobject<UEnemyMidBossRuntimeData>(TEXT("MidBossRuntimeData"));
	StateTreeComponent		= CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTreeComponent"));
	AttackComponent			= CreateDefaultSubobject<UEnemyAttackComponent>(TEXT("EnemyAttackComponent"));
}

void AEnemyMidBossBase::NotifyChangedStateEnum(EEnemyState InTargetState)
{
	if (!IsValid(EnemyMidBossRuntimeData)) { return; }

	EnemyMidBossRuntimeData->ChangedEnemyState(InTargetState);
}

void AEnemyMidBossBase::NotifyChangedAttackData(const FEnemyAttackData& InAttackData)
{
	if (!IsValid(EnemyMidBossRuntimeData)) { return; }

	EnemyMidBossRuntimeData->ChangedAttackData(InAttackData);
}

void AEnemyMidBossBase::NotifyChangedCollisionResponseToChannel(ECollisionChannel InChannel, ECollisionResponse InNewResponse)
{
	if (!GetCapsuleComponent()) { return; }

	GetCapsuleComponent()->SetCollisionResponseToChannel(InChannel, InNewResponse);
}

void AEnemyMidBossBase::BeginPlay()
{
	Super::BeginPlay();

	// ゲームの進行に合わせて敵パラメータを設定
	UpdateParams();

	// デリゲートへの登録
	RegisterDelegates();

	// 中ボスのデータをセット
	SetEnemyStatusData(EnemyDataAsset);

	// 座標
	TObjectPtr<APawn> PPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	const FVector playerLocation = PPlayerPawn->GetActorLocation();
	FVector SpawnLocation = CalculateEnemySpawnPointInRing(playerLocation);

	FHitResult HitResult;
	if (!IsIntersectingStaticObjects(HitResult, SpawnLocation)) { return; }
	SetActorLocation(SpawnLocation);
}

bool AEnemyMidBossBase::IsInChaseDistance()
{
	return EnemyStatus.TargetDistanceSqr < FMath::Square(EnemyStatus.ChaseDistance);
}

void AEnemyMidBossBase::RegisterDelegates()
{
	if (!IsValid(EnemyMidBossRuntimeData)) { return; }

	// 移動方向
	EnemyMidBossRuntimeData->OnMoveDirChanged.AddUObject(this, &AEnemyMidBossBase::SetMoveDir);

	// 距離の二乗値
	EnemyMidBossRuntimeData->OnTargetDistChanged.AddUObject(this, &AEnemyMidBossBase::SetTargetDistanceSqr);

	// ステートEnum切り替え
	EnemyMidBossRuntimeData->OnStateEnumChanged.AddUObject(this, &AEnemyMidBossBase::SetEnemyState);

	// 攻撃種類の切り替え
	EnemyMidBossRuntimeData->OnAttackDataChanged.AddUObject(this, &AEnemyMidBossBase::SetAttackData);
}

void AEnemyMidBossBase::UpdateParams()
{
	if (!IsValid(GameProgress) ||
		!IsValid(EnemyMidBossRuntimeData)) 
	{ return; }

	// 倒した敵数を元に
	const int32 killCount = GameProgress->GetKillCount();

	// ヒットポイントの更新
	{
		// 計算後の最終的なHPをセット
		EnemyMidBossRuntimeData->SetFinalHP(EnemyStatus.FinalHP);
	}

	// 攻撃パラメータの更新
	{
		// 最終的な攻撃倍率の更新
		float AttackScale =
			EnemyStatus.AttackScaling.CalculateFinalScaling(killCount);

		// 基礎攻撃力と倍率をセット
		EnemyMidBossRuntimeData->SetBaseAttackPower(EnemyStatus.FinalAttack);
		EnemyMidBossRuntimeData->SetAttackScaling(AttackScale);
	}
}

void AEnemyMidBossBase::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	if (!IsValid(EnemyMidBossRuntimeData))
	{
		return;
	}

	// プレイヤーの座標を取得
	{
		TObjectPtr<APawn> PPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		const FVector playerLocation = PPlayerPawn->GetActorLocation();
		EnemyMidBossRuntimeData->CalcDistanceToTarget(playerLocation, GetActorLocation());
	}
	
	UE_LOG(LogTemp,Warning,TEXT("MoveSpeed : %f"),GetCharacterMovement()->GetMaxSpeed());

	// 攻撃可能化の判断処理
	CheckCanAttack();

	// IdleステートからWalkステートへの切り替え処理
	TransitionIdleToWalk();
}

void AEnemyMidBossBase::FinalizeDeath()
{
	// 敵が死んだ際にゲームの進行管理クラス経由で倒した敵数を加算する
	if (IsValid(GameProgress)) 
	{
		GameProgress->AddTyuuBossCount();
	}

	// ゲームインスタンス経由で、経験値とギアエネルギーをセット
	if (USuperGameInstance* GameInstance =
		GetWorld()->GetGameInstance<USuperGameInstance>())
	{
		GameInstance->GetPlayerRuntimeData()->AddExperience(EnemyStatus.Exp);
		GameInstance->GetPlayerRuntimeData()->AddGearEnergy(EnemyStatus.GearEnergy);
	}

	// 削除
	Destroy();
}

void AEnemyMidBossBase::ActivateAttack(EEnemyAttackType InAttackTyp,const FVector& InTargetLocation)
{
	if (!IsValid(AttackComponent)) { return; }

	// 攻撃種類をキーとして攻撃を発動
	AttackComponent->SetTargetLocation(InTargetLocation);
	AttackComponent->AttackActive(InAttackTyp);
}

bool AEnemyMidBossBase::IsInAttackDistance()
{
	return EnemyStatus.TargetDistanceSqr < FMath::Square(EnemyStatus.AttackDistance);
}

float AEnemyMidBossBase::GetFinalAttackPower() const
{
	if (!IsValid(EnemyMidBossRuntimeData)) { return 1.f; }
	return EnemyMidBossRuntimeData->GetCharacterAttackPower();
}

void AEnemyMidBossBase::ApplyDamaged(float InDamaged)
{
	if (!IsValid(EnemyMidBossRuntimeData)) { return; }

	EnemyMidBossRuntimeData->AddHealth(-InDamaged);	// 渡された値分、FinalHPを減算

	EnemyMidBossRuntimeData
		->CalclateDamageToMaxHealthRatio(InDamaged);// 受けたダメージが最大体力に対して何割かを算出
	OnHit();

	// 体力が0以下なら死亡フラグを立てる
	if (EnemyMidBossRuntimeData->GetHealth() <= 0)
	{
		EnemyStatus.IsAlive = false;
		OnDeath();
	}
}

void AEnemyMidBossBase::ApplyKnockBack(const FVector& InOwnerLocation)
{
	if (!IsValid(EnemyMidBossRuntimeData)) { return; }

	EnemyMidBossRuntimeData->SetTargetLocation(InOwnerLocation);
}

void AEnemyMidBossBase::SetEnemyStatusData(UEnemyDataAsset* InData)
{
	if (!InData) { return; }

	EnemyStatus.MoveSpeed = InData->MoveSpeed;
	EnemyStatus.RotationInterpSpeed = InData->RotationInterpSpeed;
	EnemyStatus.HPScaling = InData->HPScaling;
	EnemyStatus.AttackScaling = InData->AttackScaling;
	EnemyStatus.KnockBackWeight = InData->KnockBackWeight;
	EnemyStatus.Exp = InData->Exp;
	EnemyStatus.GearEnergy = InData->GearEnergy;
	EnemyStatus.AttackDistance = InData->AttackDistance;
	EnemyStatus.ChaseDistance = InData->ChaseDistance;
}

void AEnemyMidBossBase::TransitionIdleToWalk()
{
	// 「棒立ち」で無いか、「追跡可能距離内」なら処理を飛ばす
	if (EnemyStatus.StateTag != EEnemyState::Idle||
		IsInChaseDistance()) 
	{
		return; 
	}

	NotifyChangedStateEnum(EEnemyState::Walk);
}

void AEnemyMidBossBase::CheckCanAttack()
{
	if (!IsValid(EnemyMidBossRuntimeData)) { return; }

	// インターバル分経過したか
	if(!EnemyMidBossRuntimeData->HasAttackIntervalElapsed(EnemyStatus.AttackInterval))
	{
		return;
	}

	// プレイヤーとの距離が攻撃可能距離内か
	if (IsInAttackDistance())
	{
		NotifyChangedStateEnum(EEnemyState::Attack);
	}
}

void AEnemyMidBossBase::FinalizeAttackEnd()
{
	if (!IsValid(EnemyMidBossRuntimeData)) { return; }

	// Runtimeクラスに攻撃終了時間を登録
	EnemyMidBossRuntimeData->NotifyAttackFinishTime();
}


bool AEnemyMidBossBase::IsIntersectingStaticObjects(
	FHitResult& HitResult,
	FVector& SpawnLocation)
{
	UWorld* World = GetWorld();
	UCapsuleComponent* Capsule = GetCapsuleComponent();

	if (!IsValid(World) || !IsValid(Capsule))
	{
		return false;
	}

	const FVector RayStart =
		SpawnLocation + FVector(0.0f, 0.0f, SpawnRayStartHeight);

	const FVector RayEnd =
		SpawnLocation - FVector(0.0f, 0.0f, SpawnRayEndDepth);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		RayStart,
		RayEnd,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		const float CapsuleHalfHeight =
			Capsule->GetScaledCapsuleHalfHeight();

		constexpr float GroundOffset = 2.0f;

		SpawnLocation = HitResult.ImpactPoint;
		SpawnLocation.Z += CapsuleHalfHeight + GroundOffset;
	}

	return bHit;
}

FVector AEnemyMidBossBase::CalculateEnemySpawnPointInRing(const FVector& Center) const
{
	// ランダム角度
	float angle = FMath::RandRange(0.0f, 360.0f);

	// XYオフセット
	FVector offset = {
		FMath::Cos(FMath::DegreesToRadians(angle)) * SpawnRadius,
		FMath::Sin(FMath::DegreesToRadians(angle)) * SpawnRadius,
		0.0f
	};

	return Center + offset;
}
