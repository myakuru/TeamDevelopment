#include "EnemyBase.h"
#include "EnemyDataAsset.h"
#include "Components/CapsuleComponent.h"
#include "Components/StateTreeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Utility/StateMachine/StateMachine.h>
#include <ProjectNull/Utility/Common/Definitions/CollisionChannels.h>
#include <ProjectNull/Component/EnemyAttackComponent/EnemyAttackComponent.h>
#include <ProjectNull/System/WorldSystem/EnemyPoolSubSystem/EnemyPoolSubSystem.h>
#include <ProjectNull\Data\CharacterRuntimeData\EnemyRuntimeData\EnemyRuntimeData.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/Animation/AnimDataAsset.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/GameProgressSubsystem/GameProgressSubsystem.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/States/EnemyStateChase/EnemyStateChase.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ExperiencePickupManager/ExperiencePickupManager.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyISMManager/EnemyISMManager.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/DamageNumberPoolSubsystem/DamageNumberPoolSubsystem.h>

#include "ProjectNull/Sound/SoundManager.h"

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
		CapsuleComponent->InitCapsuleSize(34.f, 88.f);									// カプセルサイズ
		//CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);	// Pawn用のCollision一括設定
		CapsuleComponent->CanCharacterStepUpOn = ECB_No;								// 他キャラが上に立てるか
		CapsuleComponent->SetShouldUpdatePhysicsVolume(true);							// 物理ボリューム(水中判定etc)を受けるか
		CapsuleComponent->SetCanEverAffectNavigation(false);							// NavMesh更新対象化
		CapsuleComponent->bDynamicObstacle = true;										// 動的障害物か
		RootComponent = CapsuleComponent;
	}
	

	StateTreeComponent		= CreateDefaultSubobject<UStateTreeComponent>("StateTreeComponent");
}

AEnemyBase::~AEnemyBase() = default;

void AEnemyBase::NotifyChangedStateEnum(EEnemyState a_TargetState)
{
	if (!EnemyRuntimeData) { return; }

	EnemyRuntimeData->ChangedEnemyState(a_TargetState);
}

void AEnemyBase::NotifyChangedCollisionResponseToChannel(ECollisionChannel Channel, ECollisionResponse NewResponse)
{
	if (!CapsuleComponent) { return; }

	CapsuleComponent->SetCollisionResponseToChannel(Channel, NewResponse);
}

float AEnemyBase::GetFinalAttackPower() const
{
	if (!EnemyRuntimeData) { return 1.f; }
	return EnemyRuntimeData->GetCharacterAttackPower();
}

void AEnemyBase::BeginPlay()
{
	AActor::BeginPlay();

	// コンポーネントに自身の参照を渡す
	if (EnemyAttackComponent)
	{
		EnemyAttackComponent->SetOwnerEnemy(this);
	}

	if (CapsuleComponent)
	{
		// プレイヤーなどPawnとの物理的なぶつかりを無視
		CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
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
	if (!GameProgress||!EnemyRuntimeData) { return; }

	// 倒した敵数を元に
	const int32 killCount = GameProgress->GetKillCount();

	// ヒットポイントの更新
	{
		// 計算後の最終的なHPをセット
		EnemyRuntimeData->SetFinalHP(EnemyStatus.HPScaling.GetFinalValue(killCount));
	}

	// 攻撃パラメータの更新
	{
		// 最終的な攻撃倍率の更新
		float AttackScale =
			EnemyStatus.AttackScaling.CalculateFinalScaling(killCount);

		// 基礎攻撃力と倍率をセット
		EnemyRuntimeData->SetBaseAttackPower(EnemyStatus.AttackScaling.Base);
		EnemyRuntimeData->SetAttackScaling(AttackScale);

		// 攻撃のインターバル(秒)をセット
		EnemyRuntimeData->SetAttackInterval(EnemyStatus.AttackInterval);
	}
}

void AEnemyBase::SetEnemyState(EEnemyState a_TargetState)
{
	EnemyStatus.StateTag = a_TargetState;
}

void AEnemyBase::NotfyAttackFinishTime()
{
	if (!EnemyRuntimeData) { return; }
	EnemyRuntimeData->SetAttackFinishTime(GetWorld()->GetTimeSeconds());
}

void AEnemyBase::ApplyDamaged(float InDamaged)
{
	if (!EnemyRuntimeData) { return; }
	
	//ダメージ効果音
	GetWorld()->GetGameInstance<USuperGameInstance>()->
	GetSoundManager()->PlayAtLocation(
		EnemyManager->GetDamagedSound(),
		GetActorLocation()
	);

	EnemyRuntimeData->AddHealth(-InDamaged);		// 渡された値分、FinalHPを減算
	UE_LOG(LogTemp, Error, TEXT("Damage : %f"),InDamaged);

	EnemyRuntimeData
		->CalclateDamageToMaxHealthRatio(InDamaged);// 受けたダメージが最大体力に対して何割かを算出
	OnHit();

	// ダメージUI表示位置
	const FVector DamageUILocation = GetActorLocation() + FVector(0.0f, 0.0f, 120.0f);

	if (UDamageNumberPoolSubsystem* Pool = GetWorld()->GetSubsystem<UDamageNumberPoolSubsystem>())
	{
		Pool->ShowDamageNumber(DamageUILocation, InDamaged, false);
	}

	// 体力が0以下なら死亡フラグを立てる
	if (EnemyRuntimeData->GetHealth() <= 0)
	{
		EnemyStatus.IsAlive = false;
		EnemyRuntimeData->ChangedIsAlive(EnemyStatus.IsAlive);
		OnDeath();
	}
}

void AEnemyBase::ApplyKnockBack(const FVector& InOwnerLocation)
{
	if (!EnemyRuntimeData) { return; }
	EnemyRuntimeData->SetTargetLocation(InOwnerLocation);
}

void AEnemyBase::FinalizeDeath()
{
	// 敵が死んだ際に敵管理クラス経由でリストから自身を削除する
	if (EnemyManager) {
		EnemyManager->RemoveEnemy(this);
	}

	// 敵が死んだ際にゲームの進行管理クラス経由で倒した敵数を加算する
	if (GameProgress) {
		GameProgress->AddKillCount();
	}

	{
		SpawnDeathEffect();

		SpawnDeathExperience();
	}

	// ゲームインスタンス経由で、経験値とギアエネルギーをセット
	if (USuperGameInstance* GameInstance =
		GetWorld()->GetGameInstance<USuperGameInstance>())
	{
		//GameInstance->GetPlayerRuntimeData()->AddExperience(EnemyStatus.Exp);
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
	if (!EnemyRuntimeData||
		!EnemyRuntimeData->CanAttack()) { return; }

	// 既に攻撃中なら処理を飛ばす
	if (EnemyStatus.StateTag == EEnemyState::Attack||
		EnemyStatus.StateTag == EEnemyState::Death) { return; }


	// プレイヤーとの距離が攻撃可能距離内か
	if (EnemyStatus.TargetDistanceSqr < FMath::Square(EnemyStatus.AttackDistance))
	{
		NotifyChangedStateEnum(EEnemyState::Attack);
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

	SetEnemyState(EEnemyState::Idle);

	EnemyStatus.IsAlive = true;

	// StateTreeを起動
	if (StateTreeComponent)
	{
		StateTreeComponent->StartLogic();
	}

	// 敵管理クラスの情報取得
	EnemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();

	// ゲーム進行管理クラスの情報取得
	GameProgress = GetWorld()->GetSubsystem<UGameProgressSubsystem>();

	// 敵が生成された際に敵管理クラス経由でリストへ登録する
	if (EnemyManager) {
		EnemyManager->RegisterEnemy(this);
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
		UE_LOG(LogTemp, Warning, TEXT("ISMIndex:%d"), ISMInstanceIndex);
		ISMManager->RegisterEnemy(this);
		UE_LOG(LogTemp, Warning, TEXT("ISMIndex:%d"), ISMInstanceIndex);
	}

#if WITH_EDITOR
	SetFolderPath(TEXT("Pool/Active"));
#endif
	
	//UE_LOG(LogTemp, Warning, TEXT("EnemyBase Activate"));
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

	EnemyStatus.StateTag = EEnemyState::Idle;
	if (!EnemyManager) { return; }

	EnemyStatus.IsAlive = false;

	/** ISMManagerから解除*/
	if (auto* ISMManager = EnemyManager->GetISMManager(ISMManagerClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("ISMIndex:%d"), ISMInstanceIndex);
		ISMManager->UnregisterEnemy(this);
		UE_LOG(LogTemp, Warning, TEXT("EnemyDead"));
		UE_LOG(LogTemp, Warning, TEXT("ISMIndex:%d"), ISMInstanceIndex);
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
		FTransform AdjustedTransform = GetActorTransform();
		FRotator Rot = AdjustedTransform.GetRotation().Rotator();
		Rot.Yaw -= 90.0f;
		AdjustedTransform.SetRotation(Rot.Quaternion());

		FVector Loc = AdjustedTransform.GetLocation();
		Loc.Z -= 90.0f;
		AdjustedTransform.SetLocation(Loc);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			EnemyParticle.DeathEffect,
			AdjustedTransform.GetLocation(),
			AdjustedTransform.GetRotation().Rotator(),
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
		const FLinearColor Color	= EnemyStatus.ExpColor;
		const float Size			= EnemyStatus.ExpSize;

		ItemSubsystem->GetExperiencePickupManager().SpawnExperience(
			GetActorLocation(),
			static_cast<float>(EnemyStatus.Exp),
			Color,
			Size
		);
	}
}

float AEnemyBase::GetCapsuleHalfHeight()const
{
	if (!CapsuleComponent) { return 0.0f; }

	return CapsuleComponent->GetScaledCapsuleHalfHeight();
}

void AEnemyBase::PlayAnimation(int32 InNextAnimIndex, bool InbLoop,float InBlendSpeed)
{
	if (!EnemyManager)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyManager is null"));
		return;
	}

	if (!ISMManagerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ISMManagerClass is null"));
		return;
	}

	AEnemyISMManager* ISMManager = EnemyManager->GetISMManager(ISMManagerClass);
	if (!ISMManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ISMManager not found"));
		return;
	}

	UEnemyAnimDataAsset* AnimData = ISMManager->GetAnimDataAsset();
	if (!AnimData)
	{
		UE_LOG(LogTemp, Error, TEXT("AnimDataAsset is null"));
		return;
	}

	if (!AnimData->Animations.IsValidIndex(InNextAnimIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid AnimIndex: %d / Num: %d"),
			InNextAnimIndex, AnimData->Animations.Num());
		return;
	}

	if (!EnemyRuntimeData)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyRuntimeData is null"));
		return;
	}

	//const float Duration = AnimData->Animations[NextAnimIndex].NumFrames / 30.0f;
	float Duration = (AnimData->Animations[InNextAnimIndex].NumFrames - AnimData->Animations[InNextAnimIndex].StartTime) / 30.0f;

	//EnemyRuntimeData->SetNextAnimData(NextAnimIndex, bLoop, true);
	EnemyRuntimeData->StartAnimMonitor(InNextAnimIndex, InbLoop, Duration);
	ISMManager->RequestAnimChange(ISMInstanceIndex, InNextAnimIndex, InbLoop, InBlendSpeed);

	if (!InbLoop && GetWorld())
	{
		AnimFinishTime = GetWorld()->GetTimeSeconds() + Duration;
	}
}
