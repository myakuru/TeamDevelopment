#pragma once

// ------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EnemyBossAnimNotify_BreathHit.generated.h"

UCLASS()
class PROJECTNULL_API UEnemyBossAnimNotify_BreathHit : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	// ------------------------------------------------------------------------------------
	// public method
	// ------------------------------------------------------------------------------------
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference)override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	// ------------------------------------------------------------------------------------
	// private variables
	// ------------------------------------------------------------------------------------
	/** 判定を出すボーン/ソケット名*/
	UPROPERTY(EditAnywhere, Category = "Breath")
	FName  SocketName = TEXT("Null");

	/** ブレスの最大射程*/
	UPROPERTY(EditAnywhere, Category = "Breath")
	float Range = 50.0f;

	/** 連続ヒットのインターバル*/
	UPROPERTY(EditAnywhere, Category = "Breath")
	float HitInterval = 0.7f;

	/** スフィアの座標オフセット*/
	UPROPERTY(EditAnywhere, Category = "Breath")
	FVector SphereOffset;

	/** デバッグ表示*/
	UPROPERTY(EditAnywhere, Category = "Breath")
	bool bDrawDebug = true;

private:

	/** この攻撃で当たったアクター（二重ヒット防止）*/
	TMap<TWeakObjectPtr<AActor>, float> HitCooldownMap;

	/** Niagaraコンポーネントへの参照*/
	TWeakObjectPtr<UNiagaraComponent> BreathNiagara;

};