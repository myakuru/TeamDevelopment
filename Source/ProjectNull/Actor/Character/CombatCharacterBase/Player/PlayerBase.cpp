#include "PlayerBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h"
#include "ProjectNull/Component/PlayerAttackComponent/PlayerAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>

APlayerBase::APlayerBase()
	:	SpringArmComponent(nullptr),
		CameraComponent(nullptr),
		AttackComponent(nullptr),
		GearComponent(nullptr)
{
	// ================================================================
	// ���g�̐ݒ�
	// ================================================================

	// Tick��L���ɂ���
	PrimaryActorTick.bCanEverTick = true;

	// �R���g���[���[��Yaw��]��L�����N�^�[�ɔ��f�����Ȃ�
	bUseControllerRotationYaw = false;

	// ================================================================
	// �J�����X�v�����O�A�[���R���|�[�l���g�̍쐬�Ɛݒ�
	// ================================================================
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 600.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	// ================================================================
	// �J�����R���|�[�l���g�̍쐬�Ɛݒ�
	// ================================================================
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;


	AttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>("Attack");
	GearComponent	= CreateDefaultSubobject<UPlayerGearComponent>("Gear");
}

void APlayerBase::BeginPlay()
{

	if (AttackComponent)
	{
		AttackComponent->SetOwnerPlayer(this);
	}

	if (GearComponent)
	{
		GearComponent->SetOwnerPlayer(this);
	}

	ACombatCharacterBase::BeginPlay();

	UpdateHUDHP();
}

void APlayerBase::Tick(float DeltaTime)
{
	ACombatCharacterBase::Tick(DeltaTime);

	if (ARobotController* RobotController = Cast<ARobotController>(GetController()))
	{
		HUDWidget = RobotController->GetPlayerHUD();
	}
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACombatCharacterBase::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void APlayerBase::ApplyDamage(float Damage)
{
	ACombatCharacterBase::ApplyDamage(Damage);

	UpdateHUDHP();
}

void APlayerBase::Move(const FVector2d& InputVector)
{
	if (!CanMove()) { return; }

	const FRotator yawRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	const FVector forward = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector right = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(forward, InputVector.Y);
	AddMovementInput(right, InputVector.X);
}

bool APlayerBase::CanMove()
{
	if(GearComponent && GearComponent->IsMovementBlockedByGear())
	{
		return false;
	}

	return true;
}

void APlayerBase::UpdateHUDHP()
{
	if (HUDWidget)
	{
		HUDWidget->SetPlayerHp(CombatStats.HP.Current, CombatStats.HP.Max);
	}
}
