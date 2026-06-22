// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "CollisionAttack.generated.h"

/**
 * コリジョンを持った攻撃の基底
 */
UCLASS()
class PROJECTNULL_API UCollisionAttack : public UAttackBase
{
	GENERATED_BODY()

public:

	UCollisionAttack();

public:

	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 */
	virtual void Update(const float InDeltaTime)override;

	/**
	 * @brief 有効化処理
	 */
	virtual void Execute();

	/**
	 * @brief 中止処理
	 */
	virtual void Cancel();


protected:

	/**
	 * @brief 当たり判定をオーバーラップした時の処理
	 * リストへ追加などを行う
	 */
	UFUNCTION()
	virtual void OnSphericalBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/**
	 * @brief 当たり判定を抜けた時の処理
	 * リストへ追加などを行う
	 */
	UFUNCTION()
	virtual void OnSphericalEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

protected:

	/*~Begin PrivateMember Getters*/

	/**
	 * @brief 判定させたいコリジョンチャンネルを取得
	 */
	TEnumAsByte<ECollisionChannel> GetTargetChannel()const { return TargetChannel; }

	/**
	 * @brief 指定したコリジョンチャンネルに対するレスポンスを取得 
	 */
	TEnumAsByte<ECollisionResponse> GetTargetResponse()const {return TargetResponse;}

	/**	攻撃の継続時間を取得 */
	float GetAttackDuration()const { return Duration; }

	/**	攻撃の経過時間を取得 */
	float GetElpsedTimer()const { return ElpsedTimer; }

	/*End PrivateMember Getters~ */

private:

	/**
	* @brief 判定させたいコリジョンチャンネル
	*/
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TargetChannel;

	/**
	 * @brief 指定したコリジョンチャンネルに対するレスポンス
	 */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionResponse> TargetResponse;

	/**
	 * @brief 攻撃の継続時間
	 */
	UPROPERTY(EditAnywhere)
	float Duration = 1.f;

	/**
	 * @brief 経過時間
	 */
	UPROPERTY(EditAnywhere)
	float ElpsedTimer = 0.f;
};
