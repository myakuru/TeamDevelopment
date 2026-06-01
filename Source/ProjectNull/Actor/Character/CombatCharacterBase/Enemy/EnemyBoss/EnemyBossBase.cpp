
#include "EnemyBossBase.h"
#include "AIC_EnemyBoss.h"
#include <ProjectNull\Data\CharacterRuntimeData\EnemyRuntimeData\EnemyRuntimeData.h>
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemyBossBase::AEnemyBossBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// 視野
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	// 見える範囲
	PawnSensingComp->SightRadius = 2000;
	//PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyBossBase::OnSeePlayer);
}

// Called when the game starts or when spawned
void AEnemyBossBase::BeginPlay()
{
	Super::BeginPlay();
	//AEnemyBase::BeginPlay();
}

// Called every frame
void AEnemyBossBase::Tick(float DeltaTime)
{
	ACombatCharacterBase::Tick();
	ActorLocation = GetActorLocation();
	//AEnemyBase::Tick(DeltaTime);
}

//void AEnemyBossBase::OnUpdate(APawn* Player, float DeltaTime)
//{
//	if (!Player) { return; }
//
//	/*UE_LOG(LogTemp, Warning,
//		TEXT("EnemyID:%d | AnimIndex:%d | NextAnimIndex:%d | BlendWeight:%.3f | AnimTime:%.3f | NextAnimTime:%.3f | PrevAnimTime:%.3f | Flg:%d"),
//		ISMInstanceIndex, AnimIndex, NextAnimIndex, AnimBlendWeight, AnimTime, PrevAnimTime, NextAnimTime, AnimChangeFlg ? 1 : 0
//	);*/
//
//	/*if (GetWorld()->GetTimeSeconds() >= AnimFinishTime && EnemyRuntimeData->GetAnimRoopFlg())
//	{
//		PlayAnimation(0,true);
//	}*/
//
//	//EnemyRuntimeData->UpdateAnimation(DeltaTime, EnemyStatus.BlendSpeed);
//
//	//// プレイヤーの座標を取得
//	//const FVector playerLocation = Player->GetActorLocation();
//
//	////EnemyRuntimeData->CalcDistanceToTarget(playerLocation, GetActorLocation());
//
//	//if (EnemyStatus.StateTag==EEnemyState::KnockBack)
//	//{
//	//	MoveToKnockBack(FVector::ZeroVector, 0, DeltaTime);
//	//	return;
//	//}
//
//	//// 攻撃可能か判断
//	//CheckCanAttack();
//}

void AEnemyBossBase::OnSeePlayer(APawn* Pawn)
{
	AAIC_EnemyBoss* AIController = Cast<AAIC_EnemyBoss>(GetController());

	if (AIController && Pawn)
	{
		// AIControllerにプレイヤー情報を設定
		AIController->SetPlayerKey(Pawn);
	}

	// 視野に入ったら画面に"See"と表示
	UKismetSystemLibrary::PrintString(this, "See", true, true, FColor::Blue, 2.0f);
}
