#pragma once

// ------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))	/** ブループリントを作らない*/
class PROJECTNULL_API UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/** 
* ダメージを受けられるものが実装するInterface
* 攻撃側は相手の具体型を知らずにReceiveDamageを呼べる
*/
class PROJECTNULL_API IDamageableInterface
{
	GENERATED_BODY()

public:

	// ------------------------------------------------------------------------------------
	// public method
	// ------------------------------------------------------------------------------------

	/** ダメージを受ける。Causerは攻撃を出したアクター*/
	virtual void ReceiveDamage(float Damage, AActor* Causer) = 0;

	/** 生存しているかどうか（攻撃側が死亡判定に利用）*/
	virtual bool IsAlive() const = 0;

};
