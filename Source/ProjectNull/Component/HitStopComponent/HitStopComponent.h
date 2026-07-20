#pragma once

#include "HitStopComponent.generated.h"

/** ヒットストップを行う用のコンポーネント*/
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class PROJECTNULL_API UHitStopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHitStopComponent();
protected:
	virtual void BeginPlay() override;
public:
	/** 引数１が発生する時間、引数２がヒットストップの強さ*/
	void StartHitStop(
		float Duration,
		float TimeDilation = 0.1f);

private:

	void EndHitStop();

	FTimerHandle HitStopTimerHandle;
	
	float OriginalDilation;

};