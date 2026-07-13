#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "PlayerGearComponent.generated.h"

class UGearBase;
class APlayerBase;
class UPlayerRuntimeData;
class UPlayerParameterData;
class ASphereCollision;
class UEffectBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UPlayerGearComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UPlayerGearComponent();
protected:
	virtual void BeginPlay() override;
public:	
	/** 最大ギアレベル */
	static constexpr int32 kMaxGearLevel = 4;

	static constexpr int32 kMaxGearNum = 3;

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	bool IsMovementBlockedByGear() const;

	void ExecuteGear(int32 GearIndex);

	void ChangeGear();

	/** セッター */

	/** ゲッター */
	inline const TArray<UGearBase*>& GetPlayerGears() const { return PlayerGears; }
	inline int32 GetCurrentGearLevel() const				{ return CurrentGearLevel; }

private:

	UFUNCTION()
	void OnGearBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/**
	 * @brief 無敵時間用スフィア判定初期化
	 */
	void InitializeSphereCollision();


	void UpdateSkillCooldown(
		int32 Index,
		UGearBase* Gear);

	void SetIsInvincible(bool bInIsInvincible);

	bool CanChangeGear() const;

	/**
	 * @brief 無敵開始時に呼ばれる処理
	 */
	void OnInvincibilityStart();

	/**
	 * @brief 無敵終了時に呼ばれる処理
	 */
	void OnInvincibilityEnd();
	
	/**
	 * @brief 無敵状態に応じて当たり判定を更新
	 */
	void UpdateCollisionByInvincibility();

	/**
	 * @brief ギアのWidget更新
	 * @param DeltaTime デルタタイム
	 */
	void UpdateGearWidget(float DeltaTime);

	void StartInvincibleEffect();

	void DeactivateEffect();

	/** 持ち主のプレイヤークラス */
	UPROPERTY()
	TObjectPtr<APlayerBase> OwnerPlayer;

	/** プレイヤーのRuntimeDataクラス */
	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData;

	/** プレイヤーのParameterDataクラス */
	UPROPERTY()
	TObjectPtr<UPlayerParameterData> PlayerParameterData;

	/** ギアを管理する配列 */
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UGearBase>> PlayerGears;

	/** ギアチェンジによる無敵用スフィアコリジョン */
	UPROPERTY()
	TObjectPtr<ASphereCollision> SphereCollision;

	/** ギアチェンジによる無敵用スフィアスフィアコリジョンクラス */
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASphereCollision> SphereCollisionClass;

	/** 無敵状態を表現するエフェクトActor */
	UPROPERTY(EditAnywhere, Instanced, Category = "Effect")
	TObjectPtr<UEffectBase> InvincibleEffect;

	/** 現在のギアレベル */
	UPROPERTY(EditAnywhere)
	int32 CurrentGearLevel;

	/** ヒットストップの長さ */
	UPROPERTY(EditAnywhere)
	float HitStopDuration;

	/** ヒットストップのTimeScale */
	UPROPERTY(EditAnywhere)
	float HitStopTimeDilation;

	/** ギアチェンジによる無敵時間ハンドル */
	FTimerHandle InvincibilityTimerHandle;

	UPROPERTY(EditAnywhere)
	float InvincibilityAttackPowerScale;
};
