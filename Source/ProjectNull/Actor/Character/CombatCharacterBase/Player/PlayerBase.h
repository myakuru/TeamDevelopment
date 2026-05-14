#pragma once

#include "CoreMinimal.h"
#include "../CombatCharacterBase.h"
#include "PlayerBase.generated.h"

class UPlayerHUDWidget;
class USpringArmComponent;
class UCameraComponent;
class UPlayerAttackComponent;
class UPlayerGearComponent;
class UAttackBase;
class UAutoAttack;
class USuperGameInstance;
class UPlayerAnimInstance;

UCLASS()
class PROJECTNULL_API APlayerBase : public ACombatCharacterBase
{
	GENERATED_BODY()
public:

	APlayerBase();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime)													override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FVector2d& InputVector);

	void ChangeGear();
	void CanChangeGear() const;

	int32 GetCurrentGearLevel() const;
	

	/** ゲッター */
	inline UPlayerGearComponent*			GetGearComponent() const		{ return GearComponent; }
	inline TObjectPtr<USuperGameInstance>	GetSuperGameInstance() const	{ return Instance; }
	UPlayerAnimInstance*					GetPlayerAnimInstance() const;

private:

	bool CanMove();

	void UpdateHUDHP();


	UPROPERTY()
	UPlayerHUDWidget* HUDWidget;

	UPROPERTY(VisibleAnywhere,Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere,Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Gear")
	UPlayerGearComponent* GearComponent;

	UPROPERTY(EditAnywhere, Instanced, Category = "Attack")
	TObjectPtr<UAutoAttack> AutoAttack;

	UPROPERTY()
	TObjectPtr<USuperGameInstance> Instance;

};
