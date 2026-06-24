#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectNull\Utility\Common\GameTypes\GameTypes.h"
#include "CharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * キャラクター関連のインターフェース
 */
class PROJECTNULL_API ICharacterInterface
{
	GENERATED_BODY()

public:

	/**~Begin Getters */
	/** 最終的な攻撃力を取得 */
	virtual float GetFinalAttackPower()const { return 1.f; }
	/** End Getters~*/

	/**~Begin Setters */
	/**
	 * @brief ダメージを受ける処理
	 * @param Damage ダメージ量
	 */
	virtual void ApplyDamaged(float InDamage = 1.f)PURE_VIRTUAL(ICharacterInterface::ApplyDamaged, );

	/**
	 * @brief ノックバックを受ける処理
	 * @param OwnerLocation 攻撃者の位置
	 */
	virtual void ApplyKnockBack(const FVector& InOwnerLocation) { return; }
	/** End Setters~*/
};
