
#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include <ProjectNull/Component/EnemyAttackComponent/EnemyAttackComponent.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/GameProgressSubsystem/GameProgressSubsystem.h>

AEnemyBase::AEnemyBase()
	:	EnemyManager(nullptr)
	,	GameProgress(nullptr)
	,	EnemyStatus(FEnemyStatus())
	,	LanchVelocity(FVector::ZeroVector)
{
	PrimaryActorTick.bCanEverTick = false;
	
	// �G�̍U���R���|�[�l���g�̐���
	EnemyAttackComponent = CreateDefaultSubobject<UEnemyAttackComponent>("EnemyAttack");
}


void AEnemyBase::BeginPlay()
{
	ACombatCharacterBase::BeginPlay();
	
	// �G�Ǘ��N���X�̏��擾
	EnemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();


	// �G���������ꂽ�ۂɓG�Ǘ��N���X�o�R�Ń��X�g�֓o�^����
	if (EnemyManager) {
		EnemyManager->RegisterEnemy(this);
	}

	CapsuleCollision = GetCapsuleComponent();

	// �R���W�����v���Z�b�g�ݒ�
	if (CapsuleCollision)
	{
		CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CapsuleCollision->SetCollisionObjectType(ECC_Pawn);
		CapsuleCollision->SetCollisionResponseToAllChannels(ECR_Block);
		CapsuleCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		CapsuleCollision->SetGenerateOverlapEvents(true);
	}

	// �����蔻��̐ݒ�
	{
		//// ���g�̃A�N�^�ɂ��Ă���SphereComponent��T��
		//CapsuleCollision = FindComponentByClass<UCapsuleComponent>();

		//// SphereCollision�̒��Ƀf�[�^������΁o�p�����s����
		//if (CapsuleCollision)
		//{
		//	// �I�[�o�[���b�v�J�n����onOverlap�֐���ĂԂ悤�ɐݒ�
		//	CapsuleCollision->OnComponentBeginOverlap.AddDynamic
		//	(
		//		this,
		//		&AEnemyBase::OnOverlap
		//	);

		//	UE_LOG(LogTemp, Warning, TEXT("=== AEnemyBase BeginPlay ==="));
		//}
	}

	// �R���|�[�l���g�Ɏ��g�̎Q�Ƃ�n��
	{
		if (EnemyAttackComponent)EnemyAttackComponent->SetOwnerEnemy(this);
	}

	// �Q�[���̐i�s�ɍ��킹�ēG�p�����[�^��ݒ�
	UpdateParams();
}

void AEnemyBase::MoveToPlayer(const FVector& PlayerLocation, float DeltaTime)
{
	EnemyStatus.MoveDir = PlayerLocation - GetActorLocation();
	EnemyStatus.DistancePlayer = EnemyStatus.MoveDir.Size();
	EnemyStatus.MoveDir.Normalize();
	
	// �ړ������֕�Ԃ����]��v�Z
	const FRotator calcResultRotation = CalculateRotationToMoveDirection(
										GetActorRotation(),
										EnemyStatus.MoveDir.Rotation(),
										EnemyStatus.RotationInterpSpeed,
										DeltaTime);

	// ��]��X�V
	SetActorRotation(calcResultRotation);

	// ���W��X�V
	SetActorLocation(CalculateNextActorLocation(EnemyStatus.MoveDir,EnemyStatus.MoveSpeed,DeltaTime), true);
}

void AEnemyBase::UpdateParams()
{
	if (!GameProgress) { return; }

	// �|�����G�������
	const int32 killCount = GameProgress->GetKillCount();

	// �q�b�g�|�C���g�̍X�V
	EnemyStatus.FinalHP = EnemyStatus.HPScaling.GetFinalValue(killCount);

	// �U���͂̍X�V
	EnemyStatus.FinalAttack = EnemyStatus.AttackScaling.GetFinalValue(killCount);
}

//void AEnemyBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	// �ق��̃A�N�^�Əd�Ȃ�����h�����KnockBack�֐���Ăяo��
//	UE_LOG(LogTemp, Warning, TEXT("=== AEnemyBase OnOverlap ==="));
//}

void AEnemyBase::SetKnockBackData(const FVector& PlayerLocation, float AttackPower, float EnemyWeight)
{
	if (EnemyStatus.KnockBackFlg)return;
	// ������΂��Ɏg�����l����߂�
	int knockBackPowerLevel = AttackPower - EnemyWeight;
	if (knockBackPowerLevel < 0)
	{
		knockBackPowerLevel = 0;
	}

	const FName RowName = FName(*FString::FromInt(knockBackPowerLevel));

	// RowName����^�t�Ŏ擾
	const FKnockBackData* KnockBackData =
		KnockBackDataTable->FindRow<FKnockBackData>(RowName, TEXT("KnockBack"));
	if (!KnockBackData)
	{
		UE_LOG(LogTemp, Warning, TEXT("KnockBack row not found: %s"), *RowName.ToString());
		return;
	}

	// ��������
	FVector HorizontalDir = GetActorLocation() - PlayerLocation;
	HorizontalDir.Z = 0.0f;
	HorizontalDir.Normalize();

	// ������ъp�x
	const float Rad = FMath::DegreesToRadians(KnockBackData->LaunchAngleDeg);
	// ���������̊p�x�ƃ��W�A���p���Ƃɏ�����̊p�x����
	FVector LanchDir = HorizontalDir * FMath::Cos(Rad) + FVector::UpVector * FMath::Sin(Rad);
	LanchDir.Normalize();

	EnemyStatus.KNockBackVelocity	= LanchDir * KnockBackData->LaunchSpeed;
	EnemyStatus.KnockBackFlg		= true;
	EnemyStatus.CanAttack			= false;

	UE_LOG(LogTemp, Warning, TEXT("=== �m�b�N�o�b�N�����l���� ==="));
}

void AEnemyBase::SetTakeDamaged(int32 AttackPower)
{
	// �ȈՓI�ɓn���ꂽ�l��,FinalHP����Z
	EnemyStatus.FinalHP -= AttackPower;

	if (EnemyStatus.FinalHP <= 0)
	{
		OnDeath();
	}
}

void AEnemyBase::MoveToKnockBack(const FVector& KnockBackDir, float KnockBackPower, float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();

	// �d�́i���j
	const float Gravity = -980.0f;

	// �d�͂𑬓x�ɉ��Z
	EnemyStatus.KNockBackVelocity.Z += Gravity * DeltaTime;

	// �ʒu�X�V
	FVector NextLocation = CurrentLocation + EnemyStatus.KNockBackVelocity * DeltaTime;

	FHitResult HitResult;
	SetActorLocation(NextLocation, true, &HitResult);

	// �ǂ����ɓ����������~
	if (HitResult.bBlockingHit)
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor)
		{
			// �v���C���[ or �G�l�~�[�Ȃ疳��
			if (HitActor->IsA(ACharacter::StaticClass()) ||
				HitActor->IsA(AEnemyBase::StaticClass()))
			{
				return; // ��~���Ȃ�
			}
		}

		EnemyStatus.KnockBackFlg = false;
		EnemyStatus.KNockBackVelocity = FVector::ZeroVector;
		UE_LOG(LogTemp, Warning, TEXT("=== �m�b�N�o�b�N��~ ==="));
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	ACombatCharacterBase::Tick(DeltaTime);
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACombatCharacterBase::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::OnDeath()
{
	// �G�����񂾍ۂɓG�Ǘ��N���X�o�R�Ń��X�g���玩�g��폜����
	if (EnemyManager) {
		EnemyManager->RemoveEnemy(this);
	}

	// �G�����񂾍ۂɃQ�[���̐i�s�Ǘ��N���X�o�R�œ|�����G������Z����
	if (GameProgress) {
		GameProgress->AddKillCount();
	}

	// ���g����x���������
	Destroy();
}

void AEnemyBase::CheckCanAttack()
{
	// ���ɍU���\�Ȃ珈����I��
	//if (CanAttack()) { return; }

	// �v���C���[�Ƃ̋������U���\�������
	if (EnemyStatus.DistancePlayer < EnemyStatus.AttackDistance)
	{
		EnemyStatus.CanAttack = true;
	}
	else
	{
		EnemyStatus.CanAttack = false;
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

