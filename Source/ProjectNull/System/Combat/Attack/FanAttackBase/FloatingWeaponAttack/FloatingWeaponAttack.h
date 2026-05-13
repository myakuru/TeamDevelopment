
#pragma once

#include "CoreMinimal.h"
#include "../FanAttackBase.h"
#include "FloatingWeaponAttack.generated.h"

/** 浮遊武器のエフェクト */
class UFloatingWeaponEffect;

/** 斬撃のエフェクト */
class USlashEffectBase;

/** 自動攻撃クラス */
class UAutoAttack;

/** 浮遊武器の攻撃クラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponAttack final : public UFanAttackBase
{
	GENERATED_BODY()
public:

	UFloatingWeaponAttack();

public:

	void Initialize(class AActor* Owner)						override;
	void Start()												override;
	void Update(float DeltaTime,
				AActor* Player = nullptr,
				UEnemyManagerSubsystem* EnemyManager = nullptr) override;
	bool UpdateAttack(float DeltaTime)							override;

	bool IsAttackStateStep();

	float TotalTransitionStateTime();

	float StandStateTime();

	/** セッター */
	inline void SetAutoAttack(UAutoAttack* Attack) { AutoAttack = Attack; }

	/** ゲッター */
	inline UAutoAttack* GetAutoAttack() const { return AutoAttack; }
	
private:

	/** 自動攻撃のポインタ */
	UPROPERTY()
	UAutoAttack* AutoAttack;

	/** 浮遊武器エフェクトクラス */
	UPROPERTY(EditAnywhere, Instanced)
	UFloatingWeaponEffect* FloatingWeaponEffect;

	/** 斬撃エフェクト */
	UPROPERTY(EditAnywhere, Instanced)
	TArray<USlashEffectBase*> SlashEffectArray;

	/** 待機状態の割合（0.0～1.0）*/
	UPROPERTY(EditAnywhere)
	float StandTimeRatio;
};
