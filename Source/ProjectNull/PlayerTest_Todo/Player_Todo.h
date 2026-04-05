#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player_Todo.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class UAnimBlueprint;
struct FInputActionValue;

UCLASS()
class APlayer_Todo : public ACharacter
{
	GENERATED_BODY()

public:
	APlayer_Todo();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Enhanced Input の FInputActionValue を受け取るハンドラ
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

protected:
	// コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;

	// Enhanced Input 用リソース（エディタで割り当て）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* ThirdPersonContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
};