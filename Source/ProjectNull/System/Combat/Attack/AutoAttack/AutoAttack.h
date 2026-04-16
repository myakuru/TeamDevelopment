#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "AutoAttack.generated.h"


// 敵管理クラス
class UEnemyManagerSubsystem;

// 円型の斬撃攻撃クラス
class URingPulseSlashAttack;

// 浮遊武器エフェクトクラス
class UFloatingWeaponEffect;


// 自動攻撃種類
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
	void Update(float DeltaTime)	override;

private:

	/// <summary>
	/// 自動攻撃の開始
	/// </summary>
	void StartAutoAttack();

	/// <summary>
	/// 周囲2段階（リング状に移動）自動斬撃攻撃の開始
	/// </summary>
	void StartAutoRingAttack();

	/// <summary>
	/// 自動攻撃の共通更新処理
	/// </summary>
	/// <param name="DeltaTime">デルタタイム</param>
	/// <param name="ConeSlashParams">扇状斬撃攻撃のパラメータ</param>
	/// <param name="EnemyManager">敵管理クラス</param>
	void UpdateAutoAttack(float DeltaTime, URingPulseSlashAttack& RingPulseSlashAttack, class UEnemyManagerSubsystem* EnemyManager);

	/// <summary>
	/// 敵が扇範囲内にいるか判定
	/// </summary>
	/// <param name="Enemy">敵のポインタ</param>
	/// <param name="PlayerLocation">プレイヤーの座標</param>
	/// <param name="AttackDir">プレイヤーの攻撃方向</param>
	/// <param name="ConeSlashParams">扇状斬撃攻撃のパラメータ</param>
	/// <returns>敵が扇範囲かどうか</returns>
	bool IsEnemyInConeRange(AActor* Enemy, const FVector& PlayerLocation, const FVector& AttackDir, const URingPulseSlashAttack& RingPulseSlashAttack) const;

	
	// 自動攻撃の間隔時間
	UPROPERTY(EditAnywhere)
	float AutoAttackInterval;

	// 前方扇状自動攻撃からの周囲攻撃遅延時間
	UPROPERTY(EditAnywhere)
	float FrontToRingDelay;

	// 自動攻撃のパラメータマップ配列
	UPROPERTY(EditAnywhere, Instanced)
	TMap<EAutoAttackType,URingPulseSlashAttack*> AutoAttackParamsMap;

	// ================================================================
	// エフェクト関連
	// ================================================================

	// 浮遊武器エフェクトクラスのマップ配列
	UPROPERTY(EditAnywhere, Instanced)
	TMap<EAutoAttackType, UFloatingWeaponEffect*> FloatingWeaponMap;

	// ================================================================
	// タイマー関連
	// ================================================================

	// 前方扇状自動攻撃タイマー
	FTimerHandle AutoFrontConeAttackTimerHandle;

	// 前方扇状自動攻撃からの周囲攻撃遅延タイマー
	FTimerHandle FrontToRingDelayTimerHandle;

};
