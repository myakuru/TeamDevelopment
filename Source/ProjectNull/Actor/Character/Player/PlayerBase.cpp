#include "PlayerBase.h"

#include "GameFramework\SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../../Component/PlayerAttackComponent/PlayerAttackComponent.h"
#include "../../../Component/PlayerGearComponent/PlayerGearComponent.h"

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
	GearComponent = CreateDefaultSubobject<UPlayerGearComponent>("Gear");
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
	
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

