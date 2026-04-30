#pragma once

#include "CoreMinimal.h"
#include "../PickupBase.h"
#include "ExperiencePickup.generated.h"

class Player;

/** パラメータ*/
USTRUCT(BlueprintType)
struct FExperienceParam
{
	GENERATED_BODY()

public:

	/** プレイヤーを追いかける速度*/
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 0.0f;

	/** 追いかける方向*/
	UPROPERTY(EditAnywhere)
	FVector MoveDir;

	/** エネミーからもらい、プレイヤーに渡す経験値量*/
	UPROPERTY(EditAnywhere)
	int ExperiencePoint = 1;

	/** 吸い込み開始距離*/
	UPROPERTY(EditAnywhere)
	float DetectRange = 0.0f;
	
	/** 取得距離*/
	UPROPERTY(EditAnywhere)
	float PickupRange = 0.0f;

	/** 追尾中か*/
	bool bChasing = false;

	/** 生存時間*/
	UPROPERTY(EditAnywhere)
	float Life = 10.0f;

};

UCLASS()
class PROJECTNULL_API AExperiencePickup : public APickupBase
{
	GENERATED_BODY()

public:

	AExperiencePickup();

	void SetExpValue(int32 InExp) { ExperienceParam.ExperiencePoint = InExp; }

	/** 更新*/
	void OnUpdate(APawn* Player, float DeltaTime) override;

protected:

	/** プレイヤーを追いかける*/
	void MoveToPlayer(const FVector& PlayerLocation, float DeltaTime);

	/** */
	void Pickup(APawn* Player);

protected:
	
	UPROPERTY(EditAnywhere)
	FExperienceParam ExperienceParam;

};
