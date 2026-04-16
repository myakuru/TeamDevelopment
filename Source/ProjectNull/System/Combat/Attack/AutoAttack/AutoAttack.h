#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "AutoAttack.generated.h"


//　敵管理クラス
class UEnemyManagerSubsystem;

//　円型の斬撃攻撃クラス
class UFanAttackBase;

// Niagaraエフェクトクラス
class UFloatingWeaponEffect;

//　自動攻撃種類
UENUM(BlueprintType)
enum class EAutoAttackType : uint8
{
	Front,
	Ring,
	Count UMETA(Hidden)
};

/// <summary>
/// 自動攻撃クラス
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UAutoAttack final : public UAttackBase
{
	GENERATED_BODY()

public:

	UAutoAttack();

public:

	void Initialize(AActor* Owner)	override;
	void Execute()					override;
	void Update(float DeltaTime,
				APlayerBase* Player = nullptr,
				UEnemyManagerSubsystem* EnemyManager = nullptr)	override;

private:

	/// <summary>
	/// 自動攻撃の開始
	/// </summary>
	void StartAutoAttack();

	/// <summary>
	/// 周囲2段階（リング状に移動）自動斬撃攻撃の開始
	/// </summary>
	void StartAutoRingAttack();
	
	//　自動攻撃の間隔時間
	UPROPERTY(EditAnywhere)
	float AutoAttackInterval;

	//　前方扇状自動攻撃からの周囲攻撃遅延時間
	UPROPERTY(EditAnywhere)
	float FrontToRingDelay;

	//　自動攻撃のパラメータマップ配列
	UPROPERTY(EditAnywhere, Instanced)
	TMap<EAutoAttackType, UFanAttackBase*> AutoAttackParamsMap;

	// ================================================================
	// 
	// ================================================================

	UPROPERTY(EditAnywhere, Instanced)
	TMap<EAutoAttackType, UFloatingWeaponEffect*> FloatingWeaponMap;

	//　================================================================
	//　タイマー関連
	//　================================================================

	//　前方扇状自動攻撃タイマー
	FTimerHandle AutoFrontConeAttackTimerHandle;

	//　前方扇状自動攻撃からの周囲攻撃遅延タイマー
	FTimerHandle FrontToRingDelayTimerHandle;

};
