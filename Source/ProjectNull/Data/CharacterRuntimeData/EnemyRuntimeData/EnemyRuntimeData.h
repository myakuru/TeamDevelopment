#pragma once

#include "CoreMinimal.h"
#include "../CharacterRuntimeData.h"
#include "../../../Actor/Character/CombatCharacterBase/Enemy/EnemyDataStruct.h"
#include "EnemyRuntimeData.generated.h"

/** 進行ベクトルが変更された時に呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveDirChanged,		const FVector&	/*MoveDir*/);

/** ステートEnumが変更された時に呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateEnumChanged,	EEnemyState		/*StateEnum*/);

/** ターゲットとの距離が変更された時にに呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetDistChanged,	float			/*DistSqr*/);

/** 敵のHPが0を下回った時に呼び出される*/
DECLARE_MULTICAST_DELEGATE_OneParam(FOnIsAliveChanged,		bool			/*IsKnockBack*/);

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
	 * @brief				ステートタイプを切り替える
	 * @param a_StateEnum 切り替え先のステートへ変更
	 */
	UFUNCTION(BlueprintCallable, Category = "EnemyPara")
	void ChangedEnemyState(EEnemyState a_StateEnum);

	/** 死亡判定*/
	UFUNCTION(BlueprintCallable, Category = "EnemyPara")
	void ChangedIsAlive(const bool a_IsAlive);

	//~ End Setter

	//~ Start Getter

	/** プレイヤーとの距離を返す*/
	float GetTargetDistanceSqr() { return TargetDistanceSqr; }
	
	/**
	 * EnemyBaseを弄れないので一旦コメント化
	 * デリゲートをprivateに隠蔽して使う
	 * @brief 進行方向変更デリゲートを登録用で公開
	 * @return TMulticastDelegateRegistration 型として公開
	*/
	/*FOnMoveDirChanged::RegistrationType& GetOnMoveDirChanged()
	{
		return OnMoveDirChanged;
	}*/
	
	//~ End Getter

	/** 進行方向が変更された時に呼び出される */
	FOnMoveDirChanged		OnMoveDirChanged;

	/** ターゲットとの距離が変更された時に呼び出される */
	FOnTargetDistChanged	OnTargetDistChanged;

	/**	ノックバック状態が変更された時に呼び出される */
	FOnStateEnumChanged		OnStateEnumChanged;

	/** 敵の生存フラグが変更されたときに呼び出される*/
	FOnIsAliveChanged		OnIsAliveChanged;

private:

	/** 進行ベクトル */
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	FVector	MoveDir = FVector::ZeroVector;

	/** ターゲットとの距離の二乗値 */
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	float	TargetDistanceSqr = 0.0f;

	/**	ステートEnum */
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	EEnemyState StateEnum = EEnemyState::Idle;

	/** 死亡判定*/
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	bool	IsAlive = true;
};
