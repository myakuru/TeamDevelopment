#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "CollisionAttack.generated.h"

/**
 * @brief C++専用デリゲート
 * 攻撃がHITした瞬間に呼び出す
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOverrlapInDelegate,const TObjectPtr<AActor>&		/* HitActor */);

/**
 * @brief C++専用デリゲート
 * HIT判定から抜け出した瞬間に呼び出す
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOverrlapOutDelegate,const TObjectPtr<AActor>&	/* HitActor */);

/**
 * @brief 点滅しながら当たり判定を行う際に必要なパラメータ群
 */
USTRUCT()
struct FBlinkingHitElemental
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief 点滅的に攻撃判定を行うか
	 */
	UPROPERTY(EditAnywhere)
	bool bIsBlinking = false;

	/**
	 * @brief 点滅する間隔
	 */
	UPROPERTY(EditAnywhere)
	float BlinkInterval = 0.1f;
};

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
	virtual void Update(const float InDeltaTime) override;

	/**
	 * @brief 有効化処理
	 */
	virtual void Execute(const FVector& InTargetLocation = FVector::ZeroVector)override;

	/**
	 * @brief 中止処理
	 */
	virtual void Cancel();

	// ~Begin Setters
	/**
	 * @brief コリジョンのトランスフォームに適応(攻撃判定にも親子関係で適応される)
	*/
	virtual void ApplyCollisionTransform(const FTransform& InTransform){ return; }
	
	// End Setters~
	
	
	// ~Begin Delegate Getters
	/** デリゲートを隠蔽しながら外部から扱える(登録しか出来ない)ようにする */
	/**	攻撃がHITした瞬間に発火されるデリゲートの取得 */
	FOnOverrlapInDelegate::RegistrationType& GetDelegateOnOverlapIn()
	{
		return OnOverlapInDelegate;
	}
	
	/**	HIT判定から抜け出した瞬間に発火されるデリゲートの取得 */
	FOnOverrlapInDelegate::RegistrationType& GetDelegateOnOverlapOut()
	{
		return OnOverlapOutDelegate;
	}
	// End Delegate Getters~
	
protected:
	/**
	 * @brief 現在が攻撃を有効化するタイミングかを調べる
	 * @return 当たり判定をONにするタイミングであればtrue
	 */
	virtual bool IsHitCheckActiveNow(const float InElapsedTime) const;
	
	/**
	 * @brief 当たり判定をオーバーラップした時の処理
	 * リストへ追加などを行う
	 */
	UFUNCTION()
	virtual void OnCollisionBeginOverlap(
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
	virtual void OnCollisionEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	/*~Begin PrivateMember Setters*/
	virtual void SetDuration(const float InDuration) { MaxDuration = InDuration; }
	/*End PrivateMember Setters~*/

	/*~Begin PrivateMember Getters*/
	/**
	 * @brief 判定させたいコリジョンチャンネルを取得
	 */
	virtual TEnumAsByte<ECollisionChannel> GetTargetChannel() const { return TargetChannel; }

	/**
	 * @brief 指定したコリジョンチャンネルに対するレスポンスを取得 
	 */
	virtual TEnumAsByte<ECollisionResponse> GetTargetResponse() const { return TargetResponse; }

	/**	攻撃の継続時間を取得 */
	virtual float GetAttackDuration() const { return MaxDuration; }

	/**	攻撃が有効化された瞬間の時間を取得 */
	virtual float GetJustExecuteTime() const { return JustExecuteTime; }

	/*End PrivateMember Getters~ */

private:
	
	/**
	 * @brief 攻撃がHITした時に呼び出される
	 */
	FOnOverrlapInDelegate	OnOverlapInDelegate;

	/**
	 * @brief HIT判定から抜け出した瞬間に呼び出される
	 */
	FOnOverrlapOutDelegate	OnOverlapOutDelegate;
	
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
	 * @brief 攻撃の最大継続時間
	 */
	UPROPERTY()
	float MaxDuration = 0.f;

	/**
	 * @brief 攻撃が有効化された瞬間の時間
	 */
	UPROPERTY()
	float JustExecuteTime = 0.f;

	/**
	 * @brief 攻撃に点滅処理を施す際に必要なパラメータ群
	 */
	UPROPERTY(EditAnywhere)
	FBlinkingHitElemental BlinkingElement;
};
