#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyBossTaskBase.generated.h"

class AEnemyBossBase;

/** ボス用StateTreeタスクの共通基底。Owner/Target/AICのバインドとキャストを集約する */
UCLASS(Abstract)
class PROJECTNULL_API USTT_EnemyBossTaskBase : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:

	USTT_EnemyBossTaskBase(const FObjectInitializer& a_ObjInit);

	/** ボス（Character型）。EvaluatorのOwnerEnemyBossをバインド */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<ACharacter> OwnerBoss = nullptr;

	/** ターゲット。EvaluatorのTargetActorをバインド */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<AActor> TargetActor = nullptr;

	/** AIController。EvaluatorのAICをバインド*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<AController> AIC = nullptr;

protected:

	/** OwnerBossをボス型にキャストして返す（失敗でnullptr） */
	AEnemyBossBase* GetBoss() const;

	/** Owner/Target/AICが全部有効か */
	bool HasValidRefs() const;

};