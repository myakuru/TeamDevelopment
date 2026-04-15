
#include "RobotController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectNull/System/Gear/GearBase.h"
#include "ProjectNull/Actor/Character/Player/Robot/Robot.h"
#include "ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h"
#include "ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h"


ARobotController::ARobotController()
	:	InputContext(nullptr),
		MoveAction(nullptr),
		LookAction(nullptr),
		JumpAction(nullptr),
		PlayerHUD(nullptr)
{
	bReplicates = true;
}

void ARobotController::BeginPlay()
{
	Super::BeginPlay();

	// 入力マッピングコンテキスト関連の初期化
	InitializeInputContext();

	// UIの初期化
	InitializeUI();
}

void ARobotController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* enhacedInput = Cast<UEnhancedInputComponent>(InputComponent)) 
	{
		enhacedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARobotController::Move);
		enhacedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARobotController::Look);
		enhacedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ARobotController::Jump);
		enhacedInput->BindAction(GearAction01, ETriggerEvent::Started, this, &ARobotController::GearExecute01);
	}
}

void ARobotController::InitializeInputContext()
{
	if (!InputContext) { return; }

	if (UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) 
	{
		subSystem->AddMappingContext(InputContext, 0);
	}
}

void ARobotController::Move(const FInputActionValue& MoveActionValue)
{
	APlayerBase* controlledPlayer = Cast<APlayerBase>(GetCharacter());
	if (!controlledPlayer) { return; }

	controlledPlayer->Move(MoveActionValue.Get<FVector2D>());
}

void ARobotController::Look(const FInputActionValue& LookActionValue)
{
	const FVector2D lookVector = LookActionValue.Get<FVector2D>();
	AddYawInput(lookVector.X);
	AddPitchInput(lookVector.Y);
}

void ARobotController::Jump(const FInputActionValue& JumpActionValue)
{
	// ジャンプの実行
	if (ACharacter* controlledCharacter = GetCharacter())
	{
		controlledCharacter->Jump();
	}
}

void ARobotController::GearExecute01(const FInputActionValue& GearActionValue01)
{
	UE_LOG(LogTemp, Warning, TEXT("aaaaa"));
	if (APlayerBase* controlledPlayer = Cast<APlayerBase>(GetCharacter()))
	{
		if (UPlayerGearComponent* gearComponent = controlledPlayer->GetGearComponent())
		{
			gearComponent->ExecuteGear(0);
		}
	}
}

// UIの初期化関数
void ARobotController::InitializeUI()
{

	if (IsLocalController() && PlayerHUDClass)
	{
		PlayerHUD		= CreateWidget<UPlayerHUDWidget>	(this, PlayerHUDClass);

		// HUDの表示
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
	}
}

