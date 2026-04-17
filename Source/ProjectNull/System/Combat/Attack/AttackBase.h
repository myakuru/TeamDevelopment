#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AttackBase.generated.h"

// 敵管理クラス
class UEnemyManagerSubsystem;

// プレイヤー基底クラス
class APlayerBase;

/// <summary>
/// 攻撃の基底クラス
/// プレイヤーや敵の攻撃コンポーネントクラスで使用される攻撃クラスの基底クラス
/// </summary>
UCLASS()
class PROJECTNULL_API UAttackBase : public UObject
{
	GENERATED_BODY()

public:

	UAttackBase();

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(class AActor* Owner);

	/// <summary>
	/// 攻撃の実行
	/// ※純粋仮想関数であり、派生クラスで必ず実装する必要がある
	/// </summary>
	virtual void Execute() PURE_VIRTUAL(UAttackBase::Execute, );
	
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="DeltaTime		">デルタタイム				</param>
	/// <param name="Player			">プレイヤークラスのアドレス	</param>
	/// <param name="EnemyManager	">敵管理クラスのアドレス		</param>
	virtual void Update(float DeltaTime,APlayerBase* Player = nullptr,UEnemyManagerSubsystem* EnemyManager = nullptr) { return; }

	/// <summary>
	/// 攻撃判定
	/// </summary>
	/// <param name="Player			">プレイヤークラスのアドレス	</param>
	/// <param name="EnemyManager	">敵管理クラスのアドレス		</param>
	virtual void AttackJudge(APlayerBase* Player = nullptr, UEnemyManagerSubsystem* EnemyManager = nullptr);


	/// <summary>
	/// 攻撃範囲内にターゲットがいるかどうかの判定
	/// </summary>
	/// <param name="Target			">ターゲット	</param>
	/// <param name="OwnerLocation	">攻撃者の位置	</param>
	/// <param name="AttackDir		">攻撃方向	</param>
	/// <returns>ターゲットが攻撃範囲内かどうか</returns>
	virtual bool IsTargetInRange(AActor* Target, const FVector& OwnerLocation) { return false; }

	/// <summary>
	/// 持ち主の前方ベクトルから攻撃方向を計算する
	/// </summary>
	/// <param name="forwardVector">前方ベクトル</param>
	/// <returns></returns>
	virtual FVector CalcAttackDir(const FVector& forwardVector)const;

	// ゲッター
	bool CanExecute() const { return bCanExecute; }

protected:

	/// <summary>
	/// プレイヤーに対する攻撃判定
	/// </summary>
	/// <param name="Player">プレイヤークラスのアドレス</param>
	virtual void AttackJudgePlayer(APlayerBase* Player) { return; };
	
	/// <summary>
	/// 敵リストに対する攻撃判定
	/// </summary>
	/// <param name="EnemyManager">敵管理クラスのアドレス</param>
	virtual void AttackJudgeEnemys(UEnemyManagerSubsystem* EnemyManager) { return; };

	/// <summary>
	/// 持ち主のアクターのポインタ
	/// </summary>
	UPROPERTY()
	AActor* OwnerActor;
	
	// 攻撃を実行できるかどうか
	bool bCanExecute;

	// 実行中かどうか
	bool bIsActive;
};
