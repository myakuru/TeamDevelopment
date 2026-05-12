#pragma once

#include "CoreMinimal.h"
#include "../CharacterRuntimeData.h"
#include "EnemyRuntimeData.generated.h"

/** 進行ベクトルが変更された時に呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveDirChanged,		const FVector& /*MoveDir*/);

/** ターゲットとの距離が変更された時にに呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetDistChanged,	float /*DistSqr*/);

/** ノックバック状態が変更された時に呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnIsKnockBackChanged,	bool /*IsKnockBack*/);

/** 敵のHPが0を下回った時に呼び出される*/
DECLARE_MULTICAST_DELEGATE_OneParam(FOnIsAliveChanged,		bool /*IsKnockBack*/);

/**
 * 敵のランタイムな値を管理する
 */
UCLASS()
class PROJECTNULL_API UEnemyRuntimeData : public UCharacterRuntimeData
{
	GENERATED_BODY()
	
public:

	UEnemyRuntimeData();

	//~ Begin Setter

	/**
	 * @brief				ターゲットとの距離計算処理
	 * @param a_TargetPos	ターゲット座標
	 * @param a_OwnerPos	原点座標
	 */
	UFUNCTION(BlueprintCallable, Category = "EnemyPara")
	void CalcDistanceToTarget(const FVector& a_TargetPos, const FVector& a_OwnerPos);

	/**
	 * @brief				ノックバック中かどうかを切り替える
	 * @param a_IsKnockBack trueでノックバック中
	 */
	UFUNCTION(BlueprintCallable, Category = "EnemyPara")
	void ChangedIsKnockBack(const bool a_IsKnockBack);

	/** 死亡判定*/
	UFUNCTION(BlueprintCallable, Category = "EnemyPara")
	void ChangedIsAlive(const bool a_IsAlive);

	//~ End Setter

	//~ Start Getter
	
	/**
	 * EnemyBaseを弄れないので一旦コメント化
	 * デリゲートをprivateに隠蔽して使う
	 * @brief 進行方向変更デリゲートを登録用で公開
	 * @return TMulticastDelegateRegistration 型として公開
	*/
	FOnMoveDirChanged::RegistrationType& GetOnMoveDirChanged()
	{
		return OnMoveDirChanged;
	}
	
	//~ End Getter

	/** 進行方向が変更された時に呼び出される */
	FOnMoveDirChanged		OnMoveDirChanged;

	/** ターゲットとの距離が変更された時に呼び出される */
	FOnTargetDistChanged	OnTargetDistChanged;

	/**	ノックバック状態が変更された時に呼び出される */
	FOnIsKnockBackChanged	OnIsKnockBackChanged;

	/** 敵の生存フラグが変更されたときに呼び出される*/
	FOnIsAliveChanged		OnIsAliveChanged;

private:

	/** 進行ベクトル */
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	FVector	MoveDir = FVector::ZeroVector;

	/** ターゲットとの距離の二乗値 */
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	float	TargetDistanceSqr = 0.0f;

	/**	ノックバック中か */
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	bool	IsKnockBack = false;

	/** 死亡判定*/
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	bool	IsAlive = true;
};
