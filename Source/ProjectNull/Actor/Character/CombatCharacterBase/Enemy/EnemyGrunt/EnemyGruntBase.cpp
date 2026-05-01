
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

	// �v���C���[�̍��W��擾
	const FVector playerLocation = Player->GetActorLocation();

	if (EnemyStatus.KnockBackFlg)
	{
		MoveToKnockBack(FVector::ZeroVector, 0, DeltaTime);
		return;
	}

	// �v���C���[�֌�����������X�V
	MoveToPlayer(playerLocation,DeltaTime);

	// �U���\�����f
	CheckCanAttack();
}
