// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/StateTree/ST_Tasks/STT_EnemyKnockBack/STT_EnemyKnockBack.h"
#include "STT_EnemyMidBossKnockBack.generated.h"

/**
 * 中ボスのノックバック処理
 * 雑魚敵と同じ挙動をとる
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyMidBossKnockBack : public USTT_EnemyKnockBack
{
	GENERATED_BODY()
	
public:

	USTT_EnemyMidBossKnockBack(const FObjectInitializer& InObjInit);

};
