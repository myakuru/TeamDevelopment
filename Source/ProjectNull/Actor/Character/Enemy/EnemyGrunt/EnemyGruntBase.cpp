
#include "EnemyGruntBase.h"

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

	// プレイヤーの座標を取得
	const FVector playerLocation = Player->GetActorLocation();

	if (EnemyStatus.KnockBackFlg)
	{
		MoveToKnockBack(FVector::ZeroVector, 0, DeltaTime);
		return;
	}

	// プレイヤーへ向かう処理を更新
	MoveToPlayer(playerLocation,DeltaTime);

	// 攻撃可能か判断
	CheckCanAttack();
}

//void AEnemyGruntBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (Cast<ACharacter>(OtherActor))
//	{
//		//float KnockBackPower = 10.0f;
//		//const FVector ActorLocation = OtherActor->GetActorLocation();
//		//// プレイヤーと接触したら吹き飛ばされる
//		//const float distanceToPlayer = FVector::Distance(ActorLocation, GetActorLocation());
//		//const FVector knockBackDir = GetActorLocation() - ActorLocation;
//		//KnockBack(knockBackDir, KnockBackPower, 1.0f);
//		UE_LOG(LogTemp, Warning, TEXT("=== AEnemyGruntBase Overlap ==="));
//		SetKnockBackData(OtherActor->GetActorLocation(), 2, 1);
//	}
//}
//
