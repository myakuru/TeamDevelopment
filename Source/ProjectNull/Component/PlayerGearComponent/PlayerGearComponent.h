#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerGearComponent.generated.h"

class UGearBase;

class APlayerBase;


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

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsMovementBlockedByGear() const;

	void ExecuteGear(int32 GearIndex);

	void ChangeGear();

	/** セッター */

	/** ゲッター */
	inline const TArray<UGearBase*>& GetPlayerGears() const { return PlayerGears; }
	inline int32 GetCurrentGearLevel() const				{ return CurrentGearLevel; }

private:

	void SetIsInvincible(bool SetFlg);

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

	void UpdateGearWidget(float DeltaTime);

	UPROPERTY()
	TObjectPtr<APlayerBase> OwnerPlayer;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UGearBase>> PlayerGears;

	UPROPERTY(EditAnywhere)
	int32 CurrentGearLevel;

	/** ギアチェンジによる無敵時間ハンドル */
	FTimerHandle InvincibilityTimerHandle;
};
