#include "EnemyBase.h"
#include "EnemyDataAsset.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StateTreeComponent.h"
#include <ProjectNull/Utility/StateMachine/StateMachine.h>
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
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/Animation/AnimDataAsset.h>
#include <ProjectNull/Utility/Common/Definitions/CollisionChannels.h>

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

void AEnemyBase::OnEnterSlope()
{
	/* 備忘録 */
	// 坂道の範囲内に入った場合
	// １．フラグを立てる
	// ２．移動処理内で坂道移動処理を実行
	// ３．判定処理をタイマーに格納して毎フレーム計算を回避
}

void AEnemyBase::BeginPlay()
{
	AActor::BeginPlay();

	// 敵管理クラスの情報取得
	//EnemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();


	// 敵が生成された際に敵管理クラス経由でリストへ登録する
	//if (EnemyManager) {
		//EnemyManager->RegisterEnemy(this);
	//}

	// コンポーネントに自身の参照を渡す
	{
		if (EnemyAttackComponent)
		{
			EnemyAttackComponent->SetOwnerEnemy(this);
		}
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
	if (EnemyStatus.StateTag == EEnemyState::Hit)return;
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
	SetEnemyState(EEnemyState::Hit);
	//EnemyStatus.CanAttack			= false;
}

void AEnemyBase::SetEnemyState(EEnemyState a_TargetState)
{
	EnemyStatus.StateTag = a_TargetState;

	EnemyRuntimeData->ChangedEnemyState(a_TargetState);
}

void AEnemyBase::ApplyDamaged(float a_Damage)
{
	// 渡された値分、FinalHPを減算
	EnemyRuntimeData->AddHealth(-a_Damage);
	OnHit();

	// 体力が0以下なら死亡フラグを立てる
	if (EnemyRuntimeData->GetHealth() <= 0)
	{
		EnemyStatus.IsAlive = false;
		EnemyRuntimeData->ChangedIsAlive(EnemyStatus.IsAlive);
		OnDeath();
	}
}

void AEnemyBase::MoveToKnockBack(const FVector& KnockBackDir, float KnockBackPower, float DeltaTime)
{
	//FVector CurrentLocation = GetActorLocation();

	//// 重力（仮）
	//const float Gravity = -980.0f;

	//// 重力を速度に加算
	//EnemyStatus.KNockBackVelocity.Z += Gravity * DeltaTime;

	//// 位置更新
	//FVector NextLocation = CurrentLocation + EnemyStatus.KNockBackVelocity * DeltaTime;

	//FHitResult HitResult;
	//SetActorLocation(NextLocation, true, &HitResult);

	//// どこかに当たったら停止
	//if (HitResult.bBlockingHit)
	//{
	//	AActor* HitActor = HitResult.GetActor();

	//	if (HitActor)
	//	{
	//		// プレイヤー or エネミーなら無視
	//		if (HitActor->IsA(ACharacter::StaticClass()) ||
	//			HitActor->IsA(AEnemyBase::StaticClass()))
	//		{
	//			return; // 停止しない
	//		}
	//	}

	//	EnemyStatus.StateTag = EEnemyState::Idle;
	//	EnemyStatus.KNockBackVelocity = FVector::ZeroVector;
	//}
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
		//SpawnDeathEffect();

		//SpawnDeathExperience();
	}

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
	// 既に攻撃中なら処理を飛ばす
	if (EnemyStatus.StateTag == EEnemyState::Attack) { return; }

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

	EnemyStatus.StateTag = EEnemyState::Idle;
	EnemyRuntimeData->ChangedEnemyState(EEnemyState::Idle);

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

float AEnemyBase::GetCapsuleHalfHeight()const
{
	if (!CapsuleComponent) { return 0.0f; }

	return CapsuleComponent->GetScaledCapsuleHalfHeight();
}

void AEnemyBase::PlayAnimation(int32 NextAnimIndex, bool bLoop)
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

	if (!AnimData->Animations.IsValidIndex(NextAnimIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid AnimIndex: %d / Num: %d"),
			NextAnimIndex, AnimData->Animations.Num());
		return;
	}

	if (!EnemyRuntimeData)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyRuntimeData is null"));
		return;
	}

	const float Duration = AnimData->Animations[NextAnimIndex].NumFrames / 30.0f;

	EnemyRuntimeData->SetNextAnimData(NextAnimIndex, bLoop, true);
	ISMManager->RequestAnimChange(ISMInstanceIndex, NextAnimIndex, bLoop, 1.2f);

	if (!bLoop && GetWorld())
	{
		AnimFinishTime = GetWorld()->GetTimeSeconds() + Duration;
	}
}
