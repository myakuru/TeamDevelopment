#include "PlayerBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/CharacterParameterData.h>

APlayerBase::APlayerBase()
	:	SpringArmComponent(nullptr),
		CameraComponent(nullptr),
		AutoAttack(nullptr),
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

	GearComponent = CreateDefaultSubobject<UPlayerGearComponent>("Gear");
}

void APlayerBase::BeginPlay()
{
	ACombatCharacterBase::BeginPlay();

	if (GearComponent) {
		GearComponent->SetOwnerPlayer(this);
	}

	if (AutoAttack) {
		AutoAttack->Initialize(this);
	}

	UpdateHUDHP();

	if (!Instance
		|| !Instance->GetCharacterParameterData()
		|| !GetCharacterMovement()
		|| !GearComponent) { return; }

	const int32 gearLevel = GearComponent->GetCurrentGearLevel();
	GetCharacterMovement()->MaxWalkSpeed = Instance->GetCharacterParameterData()->CalculateFinalSpeed();
}

void APlayerBase::Tick(float DeltaTime)
{
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	if (!enemyManager) { return; }

	
	ACombatCharacterBase::Tick(DeltaTime);

	if (AutoAttack) {
		AutoAttack->Update(DeltaTime,nullptr,enemyManager);
	}

	if (ARobotController* RobotController = Cast<ARobotController>(GetController())) {
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

void APlayerBase::AddGearEnergy(float Amount)
{
	if (!Instance || !Instance->GetCharacterParameterData()) { return; }
	
	Instance->GetCharacterParameterData()->AddGearEnergy(Amount);
}

void APlayerBase::AddExperience(float Amount)
{
	if (!Instance || !Instance->GetCharacterParameterData()) { return; }

	Instance->GetCharacterParameterData()->AddExperience(Amount);
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
	if(GearComponent && GearComponent->IsMovementBlockedByGear()) {
		return false;
	}

	return true;
}

void APlayerBase::UpdateHUDHP()
{
	if (HUDWidget) {
		HUDWidget->SetPlayerHp(CombatStats.HP.Current, CombatStats.HP.Max);
	}
}
