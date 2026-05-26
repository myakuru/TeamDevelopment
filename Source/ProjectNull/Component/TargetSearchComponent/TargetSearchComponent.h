
#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include <ProjectNull/Utility/Common/GameTypes/GameTypes.h>

#include "TargetSearchComponent.generated.h"

/** 敵管理クラス */
class UEnemyManagerSubsystem;

/** 対象検索コンポーネントクラス */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UTargetSearchComponent final : public UActorComponent
{
	GENERATED_BODY()
public:	
	UTargetSearchComponent();
protected:

	void BeginPlay() override;

public:	
	void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** 距離順の敵一覧を取得 */
	TArray<FEnemyDistanceData> FindEnemiesSortedByDistance(float Range);

private:

	UPROPERTY()
	TObjectPtr<UEnemyManagerSubsystem> EnemyManager;
};
