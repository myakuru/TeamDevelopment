#pragma once

#include "CoreMinimal.h"
#include "../CombatCharacterBase.h"
#include "PlayerBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerAttackComponent;
class UPlayerGearComponent;
class UAttackBase;
class UAutoAttack;
class USuperGameInstance;
class UPlayerAnimInstance;
class UPlayerMaterialCollectionUpdater;

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
	


	/** Getter */
	inline UCameraComponent*				GetCameraComponent() const			{ return CameraComponent; }
	inline USpringArmComponent*				GetSpringArmComponent() const		{ return SpringArmComponent; }
	inline UPlayerGearComponent*			GetGearComponent() const			{ return GearComponent; }
	inline TObjectPtr<USuperGameInstance>	GetSuperGameInstance() const		{ return SuperGameInstance; }
	UPlayerAnimInstance*					GetPlayerAnimInstance() const;
	FPoseSnapshot&							GetPlayerPoseSnapshot();

private:

	bool CanMove();


	/** スプリングアームコンポーネント */
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	/** カメラコンポーネント */
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	/** ギアコンポーネント */
	UPROPERTY(VisibleAnywhere, Category = "Gear")
	TObjectPtr<UPlayerGearComponent> GearComponent;

	/** 自動攻撃クラス */
	UPROPERTY(EditAnywhere, Instanced, Category = "Attack")
	TObjectPtr<UAutoAttack> AutoAttack;

	/** プレイヤークラスからMaterial Parameter Collectionへの更新処理クラス */
	UPROPERTY(VisibleAnywhere, Category = "MaterialCollection")
	TObjectPtr<UPlayerMaterialCollectionUpdater> MaterialCollectionUpdater;

	/** ゲーム全体で共有されるデータや機能を管理するクラス */
	UPROPERTY()
	TObjectPtr<USuperGameInstance> SuperGameInstance;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> RadialBlurMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> RadialBlurMID;
};
