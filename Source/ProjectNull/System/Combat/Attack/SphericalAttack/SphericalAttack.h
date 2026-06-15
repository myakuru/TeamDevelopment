#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "SphericalAttack.generated.h"

/** 球状の当たり判定を持つアクタークラス */
class ASphereCollision;

/**
 * 球状の攻撃判定を行うクラス
 */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API USphericalAttack : public UAttackBase
{
	GENERATED_BODY()

public:

	USphericalAttack();

public:

	/**
	 * @brief 初期化処理
	 * @param Owner オーナーアクター
	 */
	virtual void Initialize(const TObjectPtr<AActor>& Owner)override;

	/**
	 * @brief 有効化処理
	 */
	virtual void Execute()override;

	/**
	 * @brief 中止処理
	 */
	virtual void Cancel()override;

	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 */
	virtual void Update(float DeltaTime)override;

private:

	/**
	 * @brief 当たり判定をオーバーラップした時の処理
	 * リストへ追加などを行う
	 */
	UFUNCTION()
	void OnSphericalBeginOverlap(
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
	void OnSphericalEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:

	/**
	 * @brief デフォルトエディタ側でされるようにする
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASphereCollision> SubSphereCollision;

	/**
	 * @brief 球状の当たり判定アクター
	 */
	UPROPERTY()
	TObjectPtr<ASphereCollision> SphereCollision;

	/**
	* @brief 判定させたいコリジョンチャンネル
	*/
	UPROPERTY(EditAnywhere)
	ECollisionChannel CollisionChannel;

	/**
	 * @brief 指定したコリジョンチャンネルに対するレスポンス
	 */
	UPROPERTY(EditAnywhere)
	ECollisionResponse CollisionResponse;

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
