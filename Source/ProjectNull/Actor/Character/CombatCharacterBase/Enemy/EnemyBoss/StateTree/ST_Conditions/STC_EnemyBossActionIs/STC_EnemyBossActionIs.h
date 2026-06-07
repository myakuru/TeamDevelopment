#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossDataStruct.h"
#include "STC_EnemyBossActionIs.generated.h"

class AEnemyBossBase;

/*
* ボスのNextActionが指定の値かをチェック
* Decideから出る各遷移に条件をTargetAction違いで付ける
*/
UCLASS()
class PROJECTNULL_API USTC_ActionIs : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

protected:

	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

public:

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<ACharacter> OwnerBoss = nullptr;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	EBossActionType TargetAction = EBossActionType::None;

};
