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

	/**
	 * @brief 攻撃に必要なデータ(倍率・攻撃力)を取得
	 * @return 攻撃データ
	 */
	virtual FCharacterAttackData GetAttackData()const { return FCharacterAttackData(); }

	/**
	 * @brief ダメージを受ける処理
	 * @param Damage ダメージ量
	 */
	virtual void ApplyDamaged(float a_Damage = 1.f)PURE_VIRTUAL(ICharacterInterface::ApplyDamaged, );

	/**
	 * @brief ノックバックを受ける処理
	 * @param OwnerLocation 攻撃者の位置
	 * @param AttackPower	攻撃力
	 */
	virtual void ApplyKnockBack(const FVector& a_OwnerLocation, float a_AttackPower=1.f) { return; }
};
