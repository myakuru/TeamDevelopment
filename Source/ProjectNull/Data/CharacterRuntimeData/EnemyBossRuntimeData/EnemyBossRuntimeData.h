#pragma once

#include "CoreMinimal.h"
#include "../CharacterRuntimeData.h"
#include "../../../Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossDataStruct.h"
#include "EnemyBossRuntimeData.generated.h"

/** 敵のHPが0を下回った時に呼び出される*/
DECLARE_MULTICAST_DELEGATE_OneParam(FOnIsAliveChanged,		bool			/*IsKnockBack*/);

/**
 * 敵のランタイムな値を管理する
 */
UCLASS()
class PROJECTNULL_API UEnemyBossRuntimeData : public UCharacterRuntimeData
{
	GENERATED_BODY()
	
public:

	UEnemyBossRuntimeData();

	/** 死亡判定*/
	UFUNCTION(BlueprintCallable, Category = "EnemyPara")
	void ChangedIsAlive(const bool a_IsAlive);

	/** 敵の生存フラグが変更されたときに呼び出される*/
	FOnIsAliveChanged		OnIsAliveChanged;

//private:

	/** 進行ベクトル */
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	FVector	MoveDir = FVector::ZeroVector;

	/** ターゲットとの距離の二乗値 */
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	float	TargetDistanceSqr = 0.0f;

	/** 死亡判定*/
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	bool	IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	int32	HitIndex = 0;												/** 攻撃が何連撃目か*/


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool	bShouldFastFallOnNotify = false;							/** 重力加速フラグ*/

	EBossActionType		CurrentAction	= EBossActionType::None;		/** Decideが決めたアクション*/
	FBossAttackPattern	CurrentAttack;									/** 現在選択中の攻撃と、連撃の何撃目か */

	EBossActionType		ActionPriority	= EBossActionType::None;		/** Decideがアクションを決める際に優先度が高いものを優先するようにする*/
};
