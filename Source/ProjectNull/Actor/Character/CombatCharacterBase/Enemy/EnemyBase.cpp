#include "EnemyBase.h"
#include "EnemyDataAsset.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StateTreeComponent.h"
#include <ProjectNull/Utility/StateMachine/StateMachine.h>
#include <ProjectNull/Actor/Item/Pickup/ExperiencePickup/ExperiencePickup.h>
#include <ProjectNull/Component/EnemyAttackComponent/EnemyAttackComponent.h>
#include <ProjectNull/System/WorldSystem/EnemyPoolSubSystem/EnemyPoolSubSystem.h>
#include <ProjectNull\Data\CharacterRuntimeData\EnemyRuntimeData\EnemyRuntimeData.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/GameProgressSubsystem/GameProgressSubsystem.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/States/EnemyStateChase/EnemyStateChase.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ExperiencePickupManager/ExperiencePickupManager.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyISMManager/EnemyISMManager.h>

AEnemyBase::AEnemyBase()
	:	EnemyManager(nullptr)
	,	GameProgress(nullptr)
	,	EnemyRuntimeData(nullptr)
	,	EnemyStatus(FEnemyStatus())
	,	LanchVelocity(FVector::ZeroVector)
{
	PrimaryActorTick.bCanEverTick = false;
	
	// 敵の攻撃コンポーネントの生成
	EnemyAttackComponent	= CreateDefaultSubobject<UEnemyAttackComponent>("EnemyAttack");
	
	// 敵のランタイムパラメータ管理クラスの生成
	EnemyRuntimeData		= CreateDefaultSubobject<UEnemyRuntimeData>("EnemyRuntimeData");

	// カプセル形状コリジョンの生成・プリセット設定
	{
		CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
		CapsuleComponent->InitCapsuleSize(34.f, 88.f);
		CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		CapsuleComponent->CanCharacterStepUpOn = ECB_No;
		CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
		CapsuleComponent->SetCanEverAffectNavigation(false);
		CapsuleComponent->bDynamicObstacle = true;
		RootComponent = CapsuleComponent;
	}
	StateTreeComponent		= CreateDefaultSubobject<UStateTreeComponent>("StateTreeComponent");
}

AEnemyBase::~AEnemyBase() = default;

void AEnemyBase::NotifyChengedStateEnum(EEnemyState a_TargetState)
{
	if (!EnemyRuntimeData) { return; }

	EnemyRuntimeData->ChangedEnemyState(a_TargetState);
}

void AEnemyBase::NotifyChangedCollisionResponseToChannel(ECollisionChannel Channel, ECollisionResponse NewResponse)
{
	if (!CapsuleComponent) { return; }

	CapsuleComponent->SetCollisionResponseToChannel(Channel, NewResponse);
}

void AEnemyBase::BeginPlay()
{
	AActor::BeginPlay();

	// 敵管理クラスの情報取得
	EnemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();


	// 敵が生成された際に敵管理クラス経由でリストへ登録する
	if (EnemyManager) {
		EnemyManager->RegisterEnemy(this);
	}

	// カプセルコリジョンをRootにセット
	if (CapsuleComponent)
	{
		RootComponent = CapsuleComponent;
	}

	// コンポーネントに自身の参照を渡す
	{
		if (EnemyAttackComponent)EnemyAttackComponent->SetOwnerEnemy(this);
	}

	// ゲームの進行に合わせて敵パラメータを設定
	UpdateParams();

	// デリゲートへの関数登録
	RegisterDelegates();
}

void AEnemyBase::RegisterDelegates()
{
	if (!EnemyRuntimeData) { return; }

	// ~ AddUObject() ~
	// GC管理・Weak参照
	// EnemyDestroyした後、残存しても呼ばれない

	// 移動方向
	EnemyRuntimeData->OnMoveDirChanged.AddUObject(this,&AEnemyBase::SetMoveDir);

	// 距離の二乗値
	EnemyRuntimeData->OnTargetDistChanged.AddUObject(this, &AEnemyBase::SetTargetDistanceSqr);

	// ステートEnum切り替え
	EnemyRuntimeData->OnStateEnumChanged.AddUObject(this, &AEnemyBase::SetEnemyState);

	EnemyRuntimeData->OnIsAliveChanged.AddUObject(this, &AEnemyBase::SetIsAlive);
}

void AEnemyBase::UpdateParams()
{
	if (!GameProgress) { return; }

	// 倒した敵数を元に
	const int32 killCount = GameProgress->GetKillCount();

	// ヒットポイントの更新
	EnemyStatus.FinalHP = EnemyStatus.HPScaling.GetFinalValue(killCount);

	// 攻撃力の更新
	EnemyStatus.FinalAttack = EnemyStatus.AttackScaling.GetFinalValue(killCount);
}

void AEnemyBase::SetKnockBackData(const FVector& PlayerLocation, float AttackPower, float EnemyWeight)
{
	if (EnemyStatus.StateTag == EEnemyState::KnockBack)return;
	// 吹き飛ばしに使う数値を決める
	int KnockBackPowerLevel = AttackPower - EnemyWeight;
	if (KnockBackPowerLevel < 0)
	{
		KnockBackPowerLevel = 0;
	}

	const FName RowName = FName(*FString::FromInt(KnockBackPowerLevel));

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

	EnemyStatus.KNockBackVelocity	= LanchDir * KnockBackData->LaunchSpeed;
	SetEnemyState(EEnemyState::KnockBack);
	//EnemyStatus.CanAttack			= false;
}

void AEnemyBase::SetEnemyState(EEnemyState a_TargetState)
{
	EnemyStatus.StateTag = a_TargetState;

	EnemyRuntimeData->ChangedEnemyState(a_TargetState);
}

void AEnemyBase::SetTakeDamaged(int32 AttackPower)
{
	// 簡易的に渡された値分,FinalHPを減算
	EnemyStatus.FinalHP -= AttackPower;

	if (EnemyStatus.FinalHP <= 0)
	{
		EnemyStatus.IsAlive = false;
		EnemyRuntimeData->ChangedIsAlive(EnemyStatus.IsAlive);
		OnDeath();
	}
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
		AActor* HitActor = HitResult.GetActor();

		if (HitActor)
		{
			// プレイヤー or エネミーなら無視
			if (HitActor->IsA(ACharacter::StaticClass()) ||
				HitActor->IsA(AEnemyBase::StaticClass()))
			{
				return; // 停止しない
			}
		}

		EnemyStatus.StateTag = EEnemyState::None;
		EnemyStatus.KNockBackVelocity = FVector::ZeroVector;
	}
}

void AEnemyBase::OnDeath()
{
	SetEnemyState(EEnemyState::KnockBack);
	// 敵が死んだ際に敵管理クラス経由でリストから自身を削除する
	if (EnemyManager) {
		EnemyManager->RemoveEnemy(this);
	}

	// 敵が死んだ際にゲームの進行管理クラス経由で倒した敵数を加算する
	if (GameProgress) {
		GameProgress->AddKillCount();
	}

	//SpawnDeathEffect();

	//SpawnDeathExperience();

	// ゲームインスタンス経由で、経験値とギアエネルギーをセット
	if (USuperGameInstance* GameInstance =
		GetWorld()->GetGameInstance<USuperGameInstance>())
	{
		GameInstance->GetPlayerRuntimeData()->AddExperience(EnemyStatus.Exp);
		GameInstance->GetPlayerRuntimeData()->AddGearEnergy(EnemyStatus.GearEnergy);
	}
	
	// PoolSubSystemに返却する
	// Return()の中でDeactivate()が呼ばれて非表示・Tick停止でPool待機に戻る
	if (UEnemyPoolSubSystem* PoolSubSystem =
		GetWorld()->GetSubsystem<UEnemyPoolSubSystem>())
	{
		PoolSubSystem->Return(this);
	}
}

void AEnemyBase::CheckCanAttack()
{
	// ���ɍU���\�Ȃ珈����I��
	//if (CanAttack()) { return; }

	// プレイヤーとの距離が攻撃可能距離内か
	if (EnemyStatus.TargetDistanceSqr < FMath::Square(EnemyStatus.AttackDistance))
	{
		EnemyStatus.StateTag = EEnemyState::Attack;
	}
	else
	{
		EnemyStatus.StateTag = EEnemyState::None;
	}
}

void AEnemyBase::SetAnimSequence(UAnimSequence* AnimSequence, bool LoopFlg = false)
{
	/*if (EnemyMesh && AnimSequence)
	{
		EnemyMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		EnemyMesh->PlayAnimation(AnimSequence, LoopFlg);
	}*/
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

void AEnemyBase::Activate(const FVector& LocalPos, UEnemyDataAsset* InData)
{
	// ヌルチェック
	check(InData != nullptr);
	SetEnemyStatusData(InData);

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	EnemyStatus.StateTag = EEnemyState::None;
	EnemyRuntimeData->ChangedEnemyState(EEnemyState::None);

	EnemyStatus.IsAlive = true;

	// StateTreeを起動
	if (StateTreeComponent)
	{
		StateTreeComponent->StartLogic();
	}

	// 敵管理クラスの情報取得
	EnemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();


	// 敵が生成された際に敵管理クラス経由でリストへ登録する
	if (EnemyManager) {
		EnemyManager->RegisterEnemy(this);
	}

	// コリジョンプリセット設定
	if (CapsuleComponent)
	{
		CapsuleComponent->InitCapsuleSize(34.f, 88.f);
		CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		CapsuleComponent->CanCharacterStepUpOn = ECB_No;
		CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
		CapsuleComponent->SetCanEverAffectNavigation(false);
		CapsuleComponent->bDynamicObstacle = true;
	}

	// コンポーネントに自身の参照を渡す
	{
		if (EnemyAttackComponent)EnemyAttackComponent->SetOwnerEnemy(this);
	}

	// ゲームの進行に合わせて敵パラメータを設定
	UpdateParams();

	SetActorLocation(LocalPos);

	/** ISMManagerへの自己登録*/
	if (auto* ISMManager = EnemyManager->GetISMManager(ISMManagerClass))
	{
		ISMManager->RegisterEnemy(this);
	}

#if WITH_EDITOR
	SetFolderPath(TEXT("Pool/Active"));
#endif

	UE_LOG(LogTemp, Warning, TEXT("EnemyBase Activate"));
}

void AEnemyBase::Deactivate()
{	
	/** エディタ上でフォルダに入れる*/
#if WITH_EDITOR
	SetFolderPath(TEXT("Pool/Inactive"));
#endif

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	// StateTreeを停止
	if (StateTreeComponent)
	{
		StateTreeComponent->StopLogic(TEXT("Deactivate"));
	}

	EnemyStatus.StateTag = EEnemyState::None;
	if (!EnemyManager) { return; }

	/** ISMManagerから解除*/
	if (auto* ISMManager = EnemyManager->GetISMManager(ISMManagerClass))
	{
		ISMManager->UnregisterEnemy(this);
	}
}

void AEnemyBase::SetEnemyStatusData(UEnemyDataAsset* InData)
{
	if (!InData) { return; }

	EnemyStatus.MoveSpeed = InData->MoveSpeed;
	EnemyStatus.RotationInterpSpeed = InData->RotationInterpSpeed;
	EnemyStatus.FinalHP = InData->FinalHP;
	EnemyStatus.HPScaling = InData->HPScaling;
	EnemyStatus.FinalAttack = InData->FinalAttack;
	EnemyStatus.AttackScaling = InData->AttackScaling;
	EnemyStatus.KnockBackWeight = InData->KnockBackWeight;
	EnemyStatus.Exp = InData->Exp;
	EnemyStatus.GearEnergy = InData->GearEnergy;
	EnemyStatus.AttackDistance = InData->AttackDistance;
}

void AEnemyBase::SpawnDeathEffect()
{
	// 敵が死んだ際にパーティクルを出す
	if (EnemyParticle.DeathEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			EnemyParticle.DeathEffect,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.0f),
			true,   // bAutoDestroy
			true,   // bAutoActivate
			ENCPoolMethod::None,
			true    // bPreCullCheck
		);
	}
}

void AEnemyBase::SpawnDeathExperience()
{
	// 経験値ドロップ
	if (UItemManagerSubsystem* ItemSubsystem =
		GetWorld()->GetSubsystem<UItemManagerSubsystem>())
	{
		const FLinearColor Color = EnemyStatus.ExpColor;
		const float Size = EnemyStatus.ExpSize;

		ItemSubsystem->GetExperiencePickupManager().SpawnExperience(
			GetActorLocation(),
			static_cast<float>(EnemyStatus.Exp),
			Color,
			Size
		);
	}
}