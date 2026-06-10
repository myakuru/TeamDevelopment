#pragma once

// ------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EnemyBossAnimNotify_AttackHit.generated.h"

UCLASS()
class PROJECTNULL_API UEnemyBossAnimNotify_AttackHit : public UAnimNotifyState
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

private:

	// ------------------------------------------------------------------------------------
	// private variables
	// ------------------------------------------------------------------------------------
	/** 判定を出すボーン/ソケット名*/
	UPROPERTY(EditAnywhere, Category = "Attack")
	FName  SocketName = TEXT("Null");

	/** スフィアの半径*/
	UPROPERTY(EditAnywhere, Category = "Attack")
	float Radius = 50.0f;

	/** デバッグ表示*/
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool bDrawDebug = true;

	/** この攻撃で当たったアクター（二重ヒット防止）*/
	TSet<TWeakObjectPtr<AActor>> HitActors;

};