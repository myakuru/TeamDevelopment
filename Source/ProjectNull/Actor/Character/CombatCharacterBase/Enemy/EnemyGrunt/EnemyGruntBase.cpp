
#include "EnemyGruntBase.h"
#include <ProjectNull\Data\CharacterRuntimeData\EnemyRuntimeData\EnemyRuntimeData.h>

// Sets default values
AEnemyGruntBase::AEnemyGruntBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void AEnemyGruntBase::BeginPlay()
{
	AEnemyBase::BeginPlay();
}

void AEnemyGruntBase::TransitionIdleToWalk()
{
	if (EnemyStatus.StateTag != EEnemyState::Idle) { return; }

	NotifyChangedStateEnum(EEnemyState::Walk);
}

// Called every frame
void AEnemyGruntBase::Tick(float DeltaTime)
{
	AEnemyBase::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyGruntBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	AEnemyBase::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyGruntBase::OnUpdate(APawn* Player, float DeltaTime)
{
	if (!Player) { return; }

	/*PrevAnimTime = AnimTime;
	AnimTime += DeltaTime;

	if (AnimChangeFlg)
	{
		PlayAnimation(0,true);
	}*/

	//EnemyRuntimeData->UpdateAnimation(DeltaTime, EnemyStatus.BlendSpeed);

	// プレイヤーの座標を取得
	const FVector playerLocation = Player->GetActorLocation();

	EnemyRuntimeData->CalcDistanceToTarget(playerLocation, GetActorLocation());

	/*if (EnemyStatus.StateTag == EEnemyState::Hit)
	{
		MoveToKnockBack(FVector::ZeroVector, 0, DeltaTime);
		return;
	}*/

	// 攻撃可能か判断
	CheckCanAttack();
	
	// IdleステートからWalkステートへの切り替え処理
	TransitionIdleToWalk();
}
