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

	void UpdateAnimation(float DeltaTime, float BlendSpeed);

	/** アニメーションブレンド完了*/
	void ComplateAnimBlend();

	void AnimationReset();

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

	/** アニメーションのループフラグ変更*/
	void ChangeAnimRoopFlg(bool Flg) { AnimRoopFlg = Flg; }

	/** Updateのインターバルを設定*/
	void ChangeUpdateInterval(int32 Interval) { UpdateInterval = Interval; }

	/** アニメーションチェンジ開始フラグ（これがtrueになるとアニメーションブレンドに入る）*/
	void SetAnimChangeFlg(bool Flg)				{ AnimChangeFlg = Flg; }

	/** 次のアニメーションに使用するデータを設定*/
	/*			次のアニメーションのインデックス, ループフラグ,アニメーションチェンジフラグ*/
	void SetNextAnimData(int32 NextIndex, bool AnimRoopFlg, bool AnimChangeFlg);

	/**
	 * @brief 最終的なHP
	 * @param InFinalHP 計算後の最終HP
	 */
	void SetFinalHP(float InFinalHP);

	/**
	 * @brief 最終的な攻撃力
	 * @param InFinalAttack 計算後の最終攻撃力
	 */
	void SetFinalAttack(float InFinalAttack);

	void NotifyAnimFinished() { bAnimFinished = true; }
	void ResetAnimFinished() { bAnimFinished = false; }

	//~ End Setter

	//~ Start Getter

	/** プレイヤーとの距離を返す*/
	float GetTargetDistanceSqr() { return TargetDistanceSqr; }

	/** アニメーションのループフラグを返す*/
	bool GetAnimRoopFlg()			{ return AnimRoopFlg; }

	/** AnimToTexture用：アニメーションの現在の再生時間*/
	float GetAnimTime()				const { return AnimTime; }
	float GetBeginAnimTime()		const { return PrevAnimTime; }
	/** AnimToTexture用：再生中のアニメーションのインデックス*/
	int32 GetAnimIndex()			const { return AnimIndex; }
	float GetAnimNumFrames()	const { return AnimNumFrames; }
	int32 GetNextAnimIndex()		const { return NextAnimIndex; }
	float GetNextAnimTime()		const { return NextAnimTime; }
	float GetAnimBlendWeight()	const { return AnimBlendWeight; }
	bool GetAnimChangeFlg()				 { return AnimChangeFlg; }

	/** アニメーション終了フラグ（非ループアニメが１週したらtrueにする）*/
	bool GetAnimFinished()			const { return bAnimFinished; }

	int32 GetUpdateInterval()		const { return UpdateInterval; }
	
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

	/**	アニメーション情報*/
	/** アニメーションループ判定*/
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntime")
	bool AnimRoopFlg = false;

	/** アニメーションチェンジフラグ*/
	bool AnimChangeFlg = false;

	/** 進行アニメーション時間*/
	UPROPERTY(EditAnywhere, Category = "AnimBlend")
	float AnimTime = 0.0f;
	/** １つ前のアニメーション時間*/
	UPROPERTY(EditAnywhere, Category = "AnimBlend")
	float PrevAnimTime = 0.0f;

	/** 現在のアニメーション番号*/
	UPROPERTY(EditAnywhere, Category = "AnimBlend")
	int32 AnimIndex = 1;
	/** 次のアニメーション番号*/
	UPROPERTY(EditAnywhere, Category = "AnimBlend")
	int32 NextAnimIndex = 0;

	/** 次のアニメーションの開始時間*/
	UPROPERTY(EditAnywhere, Category = "AnimBlend")
	float NextAnimTime = 0.0f;

	/** アニメーションの総フレーム数*/
	UPROPERTY(EditAnywhere, Category = "AnimBlend")
	float AnimNumFrames = 0.0f;

	/** アニメーションブレンドの進行率*/
	UPROPERTY(EditAnywhere, Category = "AnimBlend")
	float AnimBlendWeight = 0.0f;

	/** アニメーション終了確認フラグ*/
	bool bAnimFinished = false;

	/** Updateのインターバルに利用する変数*/
	int32 UpdateInterval = 1;
};
