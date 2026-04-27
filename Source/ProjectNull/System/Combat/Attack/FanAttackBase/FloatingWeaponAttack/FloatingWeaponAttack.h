
#pragma once

#include "CoreMinimal.h"
#include "../FanAttackBase.h"
#include "FloatingWeaponAttack.generated.h"

// 浮遊武器のエフェクト
class UFloatingWeaponEffect;

// 自動攻撃クラス
class UAutoAttack;


/// <summary>
/// 浮遊武器の攻撃クラス
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponAttack final : public UFanAttackBase
{
	GENERATED_BODY()
public:

	UFloatingWeaponAttack();

public:

	void Initialize(class AActor* Owner)						override;
	void Update(float DeltaTime,
				AActor* Player = nullptr,
				UEnemyManagerSubsystem* EnemyManager = nullptr) override;
	bool UpdateAttack(float DeltaTime)							override;

	bool IsAttackStateStep();

	float TransitionStateTime();

	// セッター
	inline void SetAutoAttack(UAutoAttack* Attack) { AutoAttack = Attack; }

	// ゲッター
	inline UAutoAttack* GetAutoAttack() const { return AutoAttack; }
	
private:

	// 自動攻撃のポインタ
	UPROPERTY()
	UAutoAttack* AutoAttack;

	UPROPERTY(EditAnywhere, Instanced)
	UFloatingWeaponEffect* FloatingWeaponEffect;
};
