#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AttackBase.generated.h"

class UEnemyManagerSubsystem;

class APlayerBase;

class USceneComponent;

UCLASS()
class PROJECTNULL_API UAttackBase : public UObject
{
	GENERATED_BODY()

public:

	UAttackBase();

public:

	virtual void Initialize(class AActor* Owner);

	virtual void Execute() PURE_VIRTUAL(UAttackBase::Execute, );
	
	virtual void Update(
		float DeltaTime,
		AActor* Player = nullptr,
		UEnemyManagerSubsystem* EnemyManager = nullptr)
	{ 
		return;
	}

	virtual void AttackJudge(
		AActor* Player = nullptr,
		UEnemyManagerSubsystem* EnemyManager = nullptr);


	virtual bool IsTargetInRange(AActor* Target) { return false; }

	virtual FVector CalcAttackDir(const FVector& forwardVector)const;

	/**
	 * @brief 攻撃実行可能か
	 * @return 攻撃実行可能ならtrue
	 */
	bool CanExecute()	const { return bCanExecute; }

	/**
	 * @brief 攻撃が有効かどうか
	 * @return 有効であればtrue
	 */
	bool IsActive()		const { return bIsActive; }

protected:

	virtual void AttackJudgePlayer(AActor* Player) { return; };
	
	virtual void AttackJudgeEnemys(UEnemyManagerSubsystem* EnemyManager) { return; };

	/**	持ち主のアクタークラス */
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;
	
	/** アタッチ用のルート(位置・回転管理) */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComponent;

	/**	攻撃可能フラグ */
	bool bCanExecute = true;

	/**	攻撃有効フラグ */
	bool bIsActive = false;

	FTransform Transform;
};
