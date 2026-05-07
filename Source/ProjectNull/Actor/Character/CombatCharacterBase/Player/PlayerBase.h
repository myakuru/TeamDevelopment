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
class UPlayerRuntimeData;

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
	virtual void ApplyDamage(float Damage = 1.0f)										override;

	/**
	 * @brief ギアエネルギーを加算する処理
	 * @param Amount 加算量
	 */
	void AddGearEnergy(float Amount);

	/**
	 * @brief 経験値を加算する処理
	 * @param Amount 加算量
	 */
	void AddExperience(float Amount);

	void Move(const FVector2d& InputVector);

	int32 GetCurrentGearLevel() const;

	inline UPlayerGearComponent*			GetGearComponent() const		{ return GearComponent; }
	inline TObjectPtr<USuperGameInstance>	GetSuperGameInstance() const	{ return Instance; }


private:

	bool CanMove();

	void UpdateHUDHP();


	UPROPERTY()
	UPlayerHUDWidget* HUDWidget;

	
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere,Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Instanced, Category = "Attack")
	TObjectPtr<UAutoAttack> AutoAttack;
	
	UPROPERTY(VisibleAnywhere,Category = "Gear")
	UPlayerGearComponent* GearComponent;

	UPROPERTY()
	TObjectPtr<USuperGameInstance> Instance;

	/** Runtimeデータ 実行時に変更されるデータを計算、適用するクラス */
	UPROPERTY(EditAnywhere, Instanced, Category = "RuntimeData")
	TObjectPtr<UPlayerRuntimeData> RuntimeData;
};
