#include "PlayerBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h"
#include "ProjectNull/Component/PlayerAttackComponent/PlayerAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerBase::APlayerBase()
	:	SpringArmComponent(nullptr),
		CameraComponent(nullptr),
		AttackComponent(nullptr),
		GearComponent(nullptr)
{
	//　================================================================
	//　自身の設定
	//　================================================================

	//　Tickを有効にする
	PrimaryActorTick.bCanEverTick = true;

	//　コントローラーのYaw回転をキャラクターに反映させない
	bUseControllerRotationYaw = false;

	//　================================================================
	//　カメラスプリングアームコンポーネントの作成と設定
	//　================================================================
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 600.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	//　================================================================
	//　カメラコンポーネントの作成と設定
	//　================================================================
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

	Super::BeginPlay();

	//2100
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
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