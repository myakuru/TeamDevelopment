
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

	UE_LOG(LogTemp, Warning,
		TEXT("EnemyID:%d | AnimIndex:%d | NextAnimIndex:%d | BlendAlpha:%.3f | AnimTime:%.3f | Flg:%d"),
		ISMInstanceIndex, AnimIndex, NextAnimIndex, AnimBlendWeight, AnimTime, AnimChangeFlg ? 1 : 0
	);

	PrevAnimTime = AnimTime;
	AnimTime += DeltaTime;

	if (AnimChangeFlg)
	{
		AnimBlendWeight += DeltaTime / BlendSpeed;
		NextAnimTime += DeltaTime;
		// ブレンド完了したら切り替える（最初ではなく最後）
		if (AnimBlendWeight >= 1.0f)
		{
			AnimBlendWeight = 0.0f;
			AnimChangeFlg = false;

			// ここで初めてアニメ切り替え
			AnimIndex = NextAnimIndex;
			AnimTime = NextAnimTime;	// 1回だけリセット
			NextAnimTime = 0.0f;
		}
	}

	// プレイヤーの座標を取得
	const FVector playerLocation = Player->GetActorLocation();

	EnemyRuntimeData->CalcDistanceToTarget(playerLocation, GetActorLocation());

	if (EnemyStatus.StateTag==EEnemyState::KnockBack)
	{
		MoveToKnockBack(FVector::ZeroVector, 0, DeltaTime);
		return;
	}

	// 攻撃可能か判断
	CheckCanAttack();
}