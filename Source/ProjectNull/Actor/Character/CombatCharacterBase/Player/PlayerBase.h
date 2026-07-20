#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/CombatCharacterBase.h>
#include <ProjectNull/System/Interface/CharacterInterface/CharacterInterface.h>

#include "PlayerBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerAttackComponent;
class UPlayerGearComponent;

/** スフィアコリジョンコンポーネント */
class USphereComponent;

/** 対象検索コンポーネント */
class UTargetSearchComponent;

/** ヒットストップを行う用のコンポーネント*/
class UHitStopComponent;

/** 地面の法線に合わせてRootComponentの姿勢を更新するコンポーネント */
class UGroundAlignmentComponent;

class UAttackBase;
class UAutoAttack;
class USuperGameInstance;
class UPlayerAnimInstance;
class UPlayerMaterialCollectionUpdater;
class UCineCameraComponent;
class UPlayerCutsceneComponent;

/** モデル残像エフェクトクラス
	残像を連続的に描画するエフェクトクラス */
class UModelAfterimageTrailEffect;

UCLASS()
class PROJECTNULL_API APlayerBase : public ACombatCharacterBase
								  , public ICharacterInterface
{
	GENERATED_BODY()
public:
	APlayerBase();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime)													override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Begin Character Interface.*/

	/**	最終的を取得 */
	float GetFinalAttackPower()const override;

	/**
	* @brief ダメージを受ける処理
	* @param InDamage ダメージ量
	*/
	virtual void ApplyDamaged(float InDamage)override;
	/* End Character Interface.*/

	/**
	 * @brief 移動処理
	 * @param InputVector 入力値
	 */
	void Move(const FVector2d& InputVector);

	/**
	 * @brief ギアのレベルチェンジ処理
	 */
	void ChangeGear();

	void StartCutscene();

	void ResetTargetCameraLagSpeed();

	/** Getter */
	inline UCameraComponent*				GetCameraComponent()			const	{ return CameraComponent; }
	inline USpringArmComponent*				GetSpringArmComponent()			const	{ return SpringArmComponent; }
	inline UPlayerGearComponent*			GetGearComponent()				const	{ return GearComponent; }
	inline UTargetSearchComponent*			GetTargetSearchComponent()		const	{ return TargetSearchComponent; }
	inline UAutoAttack*						GetAutoAttack()					const	{ return AutoAttack; }
	inline UHitStopComponent*				GetHitStopComponent()			const	{ return HitStopComponent; }
	inline UGroundAlignmentComponent*		GetGroundAlignmentComponent()	const	{ return GroundAlignmentComponent; }
	inline TObjectPtr<USuperGameInstance>	GetSuperGameInstance()			const	{ return SuperGameInstance; }
	inline UCineCameraComponent*			GetCineCameraComponent()		const	{ return CineCameraComponent; }
	inline UModelAfterimageTrailEffect*		GetModelAfterimageTrailEffect()	const	{ return ModelAfterimageTrailEffect; }
	inline UPlayerCutsceneComponent*		GetPlayerCutsceneComponent()	const	{ return CutsceneComponent; }
	UPlayerAnimInstance*					GetPlayerAnimInstance() const;
	FPoseSnapshot&							GetPlayerPoseSnapshot();
	int32									GetCurrentGearLevel() const;
	bool									GetCurrentFloorNormal(FVector& OutCurrentFloorNormal);
	USceneComponent*						GetGroundAlignmentRootComponent()	const;


	/** Setter */
	inline void SetTargetCameraLagSpeed(float InTargetCameraLagSpeed) { TargetCameraLagSpeed = InTargetCameraLagSpeed; }

private:

	/**
	 * @brief プレイヤーが動けるかどうか
	 * @return 動けるならtrue 動けないならfalse
	 */
	bool CanMove();

	void UpdateCameraData(float DeltaTime);

	void UpdateModelAfterimageTrailEffect(float DeltaTime);


	/** スプリングアームコンポーネント */
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	/** カメラコンポーネント */
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	/** シネマティックカメラコンポーネント */
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCineCameraComponent> CineCameraComponent;

	/** ギアコンポーネント */
	UPROPERTY(VisibleAnywhere, Category = "Gear")
	TObjectPtr<UPlayerGearComponent> GearComponent;

	/** 対象検索コンポーネント */
	UPROPERTY(VisibleAnywhere, Category = "TargetSearch")
	TObjectPtr<UTargetSearchComponent> TargetSearchComponent;

	/** ヒットストップを行う用のコンポーネント*/
	UPROPERTY(VisibleAnywhere, Category = "HitStopComponent")
	TObjectPtr<UHitStopComponent> HitStopComponent;

	/** 地面の法線に合わせてRootComponentの姿勢を更新するコンポーネント */
	UPROPERTY(VisibleAnywhere, Category = "GroundAlignment")
	TObjectPtr<UGroundAlignmentComponent> GroundAlignmentComponent;

	/** 自動攻撃クラス */
	UPROPERTY(EditAnywhere, Instanced, Category = "Attack")
	TObjectPtr<UAutoAttack> AutoAttack;

	/** プレイヤークラスからMaterial Parameter Collectionへの更新処理クラス */
	UPROPERTY(EditAnywhere, Instanced, Category = "MaterialCollection")
	TObjectPtr<UPlayerMaterialCollectionUpdater> MaterialCollectionUpdater;

	/** モデル残像エフェクトクラス
		残像を連続的に描画するエフェクトクラス */
	UPROPERTY(EditAnywhere, Instanced, Category = "ModelAfterimageTrailEffect")
	TObjectPtr<UModelAfterimageTrailEffect> ModelAfterimageTrailEffect;

	/** カットシーンの再生用 */
	UPROPERTY(EditAnywhere, Instanced, Category = "Cutscene")
	TObjectPtr<UPlayerCutsceneComponent> CutsceneComponent;

	/** ゲーム全体で共有されるデータや機能を管理するクラス */
	UPROPERTY()
	TObjectPtr<USuperGameInstance> SuperGameInstance;

	float NormalStateCameraLagSpeed;

	float TargetCameraLagSpeed;

	UPROPERTY(EditAnywhere)
	float CameraLagInterpSpeed;
};
