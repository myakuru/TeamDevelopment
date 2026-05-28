#pragma once

#include "CoreMinimal.h"
#include "../CombatCharacterBase.h"
#include "PlayerBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerAttackComponent;
class UPlayerGearComponent;

/** 対象検索コンポーネント */
class UTargetSearchComponent;

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

	/**
	 * @brief 移動処理
	 * @param InputVector 入力値
	 */
	void Move(const FVector2d& InputVector);

	/**
	 * @brief ギアのレベルチェンジ処理
	 */
	void ChangeGear();

	/** Getter */
	inline UCameraComponent*				GetCameraComponent() const			{ return CameraComponent; }
	inline USpringArmComponent*				GetSpringArmComponent() const		{ return SpringArmComponent; }
	inline UPlayerGearComponent*			GetGearComponent() const			{ return GearComponent; }
	inline UTargetSearchComponent*			GetTargetSearchComponent() const	{ return TargetSearchComponent; }
	inline TObjectPtr<USuperGameInstance>	GetSuperGameInstance() const		{ return SuperGameInstance; }
	UPlayerAnimInstance*					GetPlayerAnimInstance() const;
	FPoseSnapshot&							GetPlayerPoseSnapshot();
	int32									GetCurrentGearLevel() const;

private:

	/**
	 * @brief プレイヤーが動けるかどうか
	 * @return 動けるならtrue 動けないならfalse
	 */
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

	/** 対象検索コンポーネント */
	UPROPERTY(VisibleAnywhere, Category = "TargetSearch")
	TObjectPtr<UTargetSearchComponent> TargetSearchComponent;

	/** 自動攻撃クラス */
	UPROPERTY(EditAnywhere, Instanced, Category = "Attack")
	TObjectPtr<UAutoAttack> AutoAttack;

	/** プレイヤークラスからMaterial Parameter Collectionへの更新処理クラス */
	UPROPERTY(VisibleAnywhere, Category = "MaterialCollection")
	TObjectPtr<UPlayerMaterialCollectionUpdater> MaterialCollectionUpdater;

	/** ゲーム全体で共有されるデータや機能を管理するクラス */
	UPROPERTY()
	TObjectPtr<USuperGameInstance> SuperGameInstance;

};
