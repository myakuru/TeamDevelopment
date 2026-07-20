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
	float Range = 500.0f;

	/** 連続ヒットのインターバル*/
	UPROPERTY(EditAnywhere, Category = "Breath")
	float HitInterval = 0.7f;

	/** スフィアの座標オフセット*/
	UPROPERTY(EditAnywhere, Category = "Breath")
	FVector SphereOffset;

	/** デバッグ表示*/
	UPROPERTY(EditAnywhere, Category = "Breath")
	bool bDrawDebug = true;

	/** ナイアガラを出す時間*/
	UPROPERTY(EditAnywhere, Category = "Breath")
	float BreathDuration = 5.0f;

	/** ナイアガラを出す時間のカウンタ*/
	float BreathCount = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Owner")
	TSubclassOf<AActor> HitOwnerActor;

	// 攻撃ごとの固有の倍率補正, 1.0が攻撃力そのまま、倍率でかけていくので、1.2などを設定するとよい
	UPROPERTY(EditAnywhere, Category = "Power")
	float UniqueAttackPower = 1.0f;

private:

	/** この攻撃で当たったアクター（二重ヒット防止）*/
	TMap<TWeakObjectPtr<AActor>, float> HitCooldownMap;

	/** Niagaraコンポーネントへの参照*/
	TWeakObjectPtr<UNiagaraComponent> BreathNiagara;

};
