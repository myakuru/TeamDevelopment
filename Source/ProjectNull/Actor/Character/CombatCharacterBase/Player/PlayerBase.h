#pragma once

#include "CoreMinimal.h"
#include "../CombatCharacterBase.h"
#include "ProjectNull\System\Interface\CharacterInterface\CharacterInterface.h"
#include "PlayerBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerAttackComponent;
class UPlayerGearComponent;

/** スフィアコリジョンコンポーネント */
class USphereComponent;

/** 対象検索コンポーネント */
class UTargetSearchComponent;

class UAttackBase;
class UAutoAttack;
class USuperGameInstance;
class UPlayerAnimInstance;
class UPlayerMaterialCollectionUpdater;
class UCineCameraComponent;
class UPlayerCutsceneComponent;

UCLASS()
class PROJECTNULL_API APlayerBase : public ACombatCharacterBase
								  , public ICharacterInterface
{
	GENERATED_BODY()
public:

	APlayerBase();

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime)													override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * @brief ダメージを受ける
	 * @param Damage 受けるダメージ量
	 */
	virtual void ApplyDamaged(float InDamage = 1.f)override;

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

	/** Getter */
	inline UCameraComponent*				GetCameraComponent() const			{ return CameraComponent; }
	inline USpringArmComponent*				GetSpringArmComponent() const		{ return SpringArmComponent; }
	inline UPlayerGearComponent*			GetGearComponent() const			{ return GearComponent; }
	inline UTargetSearchComponent*			GetTargetSearchComponent() const	{ return TargetSearchComponent; }
	inline TObjectPtr<USuperGameInstance>	GetSuperGameInstance() const		{ return SuperGameInstance; }
	inline UCineCameraComponent*			GetCineCameraComponent() const		{ return CineCameraComponent; }
	inline UPlayerCutsceneComponent*		GetPlayerCutsceneComponent() const	{ return CutsceneComponent; }
	UPlayerAnimInstance*					GetPlayerAnimInstance() const;
	FPoseSnapshot&							GetPlayerPoseSnapshot();
	int32									GetCurrentGearLevel() const;
	bool									GetCurrentFloorNormal(FVector& OutCurrentFloorNormal);

private:

	/**
	 * @brief プレイヤーが動けるかどうか
	 * @return 動けるならtrue 動けないならfalse
	 */
	bool CanMove();

	void AlignFloor();

	FTimerHandle AlignFloorTimerHandle;

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

	/** 自動攻撃クラス */
	UPROPERTY(EditAnywhere, Instanced, Category = "Attack")
	TObjectPtr<UAutoAttack> AutoAttack;

	/** プレイヤークラスからMaterial Parameter Collectionへの更新処理クラス */
	UPROPERTY(EditAnywhere, Instanced, Category = "MaterialCollection")
	TObjectPtr<UPlayerMaterialCollectionUpdater> MaterialCollectionUpdater;

	/** カットシーンの再生用 */
	UPROPERTY(EditAnywhere, Instanced, Category = "Cutscene")
	TObjectPtr<UPlayerCutsceneComponent> CutsceneComponent;

	/** ゲーム全体で共有されるデータや機能を管理するクラス */
	UPROPERTY()
	TObjectPtr<USuperGameInstance> SuperGameInstance;

	UPROPERTY()
	FVector CurrentGroundNormal = FVector::UpVector;

	UPROPERTY(EditAnywhere)
	float GroundTraceLength = 200.0f;

	UPROPERTY(EditAnywhere)
	float NormalInterpSpeed = 10.0f;

	UPROPERTY(EditAnywhere)
	float RotationInterpSpeed = 8.0f;

	UPROPERTY(EditAnywhere)
	float MaxGroundAngle = 45.f;
	UPROPERTY(EditAnywhere)
	float EnterGroundAngle = 45.f;

	UPROPERTY(EditAnywhere)
	float ExitGroundAngle = 50.f;

	UPROPERTY(EditAnywhere)
	float SlideSpeed = 1500.f;

	UPROPERTY()
	float CurrentGroundTraceLength = 1500.f;
};
