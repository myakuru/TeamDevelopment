// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_Todo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APlayer_Todo::APlayer_Todo()
{
	PrimaryActorTick.bCanEverTick = true;

	// 公式ThirdPersonと同じ設定：
	// カメラはコントローラ回転に従うが、キャラクター自体は移動方向に向く
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	if (GetCharacterMovement())
	{
		// 移動方向に合わせてキャラクターを回転させる
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	// カメラブーム（スプリングアーム）
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	check(CameraBoom != nullptr);
	// ルートにアタッチ（GetMesh() だと回転基準が混ざることがあるため公式と同じにする）
	CameraBoom->SetupAttachment(GetRootComponent());
	// ブームはコントローラ回転を使う（カメラを回す）
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = true;

	// カメラコンポーネント（ブームのソケットにアタッチ）
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCameraComponent"));
	check(ThirdPersonCameraComponent != nullptr);
	ThirdPersonCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ThirdPersonCameraComponent->bUsePawnControlRotation = false; // ブームが回転を処理する
}

// Called when the game starts or when spawned
void APlayer_Todo::BeginPlay()
{
	Super::BeginPlay();

	// 入力サブシステムへ MappingContext を追加（Enhanced Input）
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(ThirdPersonContext, 0);
			}
		}

		// マウス操作でカメラを回すときは通常 GameOnly にする（必要に応じて調整）
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

// Called every frame
void APlayer_Todo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayer_Todo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayer_Todo::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayer_Todo::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayer_Todo::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayer_Todo::StopJumping);
	}
}

// 移動関数
void APlayer_Todo::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();
	if (Controller)
	{
		// コントローラの回転（カメラの向き）に合わせて前方向と右方向を決める（公式テンプレ挙動）
		const FRotator	YawRot(0, Controller->GetControlRotation().Yaw, 0);
		const FVector	ForwardDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		const FVector	RightDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y); 

		AddMovementInput(ForwardDirection, MovementValue.Y);
		AddMovementInput(RightDirection, MovementValue.X);
	}
}

// 視点操作（マウスが来ているか確認する短いデバッグを追加）
void APlayer_Todo::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller)
	{
		// ここでは生のマウスデルタをコントローラ回転へ渡す（Sensitivity は必要なら掛ける）
		AddControllerYawInput	(LookAxisValue.X);
		AddControllerPitchInput	(LookAxisValue.Y);
	}
}