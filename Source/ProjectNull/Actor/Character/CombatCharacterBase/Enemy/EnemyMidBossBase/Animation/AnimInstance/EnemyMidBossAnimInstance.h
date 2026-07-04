// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/Animation/EnemyAnimInstance.h"
#include "EnemyMidBossAnimInstance.generated.h"

/**
 * 中ボスのアニメーションインスタンス
 */
UCLASS()
class PROJECTNULL_API UEnemyMidBossAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
	
public:

	/** 初期化（ゲームスレッド）。Pawnをキャッシュしておく*/
	virtual void NativeInitializeAnimation()override;

	/** 毎フレームの値更新（ワーカースレッドで並列更新）*/
	virtual void NativeUpdateAnimation(float InDeltaTime)override;

private:


};
