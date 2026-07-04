#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.generated.h"

/**
 * @brief 敵の攻撃タイプを表すEnum
 * 敵の攻撃コンポーネントの配列キーに使用する
 */
UENUM(BlueprintType)
enum class EEnemyAttackType :uint8
{
	Attack1			UMETA(DisplayName = "攻撃1"),
	Attack2			UMETA(DisplayName = "攻撃2"),
	Attack3			UMETA(DisplayName = "攻撃3"),
	Attack4			UMETA(DisplayName = "攻撃4"),
};