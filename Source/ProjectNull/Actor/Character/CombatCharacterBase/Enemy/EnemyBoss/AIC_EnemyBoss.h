#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_EnemyBoss.generated.h"

class UBehaviorTree;
class UBehaviorTreeComponent;
class UBehaviorComponent;
class APlayerBase;

/**
* BehaviorTreeが一区切りついたことをStateTree側へ伝えるためのデリゲート
* StateTreeのカスタムタスクがこれをBind（購読）して状態遷移のトリガーとして使う
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossBTFinished);

/*
* ボス専用のAIController
* StateTree（AIComponent側）「いつ戦闘に入るか」「今がどのフェーズか」などの
* 大きな状態を管理
* BehaviorTree（このクラスが起動）「接近して攻撃する」などの戦術の実行ループを担当
* このクラスはその２つを繋ぐ橋渡し役。StateTreeから関数で叩かれてBehaviorTreeを起動、停止し
* BehaviorTreeの終了をデリゲートでStateTreeへ帰す
*/

UCLASS()
class PROJECTNULL_API AAIC_EnemyBoss : public AAIController
{
	GENERATED_BODY()

public:

	AAIC_EnemyBoss(const class FObjectInitializer& ObjectInitializer);

	/*
	* StateTreeからBehaviorTreeの橋渡し
	*/

	/** BehaviorTreeを起動する。Combatフェーズに入った時にStateTreeのタスクから呼ぶ*/
	UFUNCTION(BlueprintCallable, Category = "AIBoss")
	void StartBossBehaviorTree();

	/** 起動中のBehaviorTreeを停止する。Combatフェーズが終了するときにStateTreeのタスクから呼ぶ*/
	UFUNCTION(BlueprintCallable, Category = "AIBoss")
	void StopBossBehaviorTree();

	/** 
	* BehaviorTree側の終了タスクから呼ぶOnBossBTFinishedを発火し、
	* 購読しているStateTreeのタスクへ「BTが終わった」と伝える
	*/
	UFUNCTION(BlueprintCallable, Category = "AIBoss")
	void NotifyBossBTFinished();

	/** StateTreeのカスタムタスクがBindする終了通知デリゲート*/
	UPROPERTY(BlueprintAssignable, Category = "AIBoss")
	FOnBossBTFinished OnBossBTFinished;

	/** Blackboard（プレイヤー情報）の出し入れ*/
	
	/** 見つけたプレイヤーをBlackboardのキーに書き込む（PawnSensing側から呼ばれる）*/
	UFUNCTION(BlueprintCallable, Category = "AIBoss")
	void SetPlayerKey(APawn* InPawn);

	/** Blackboardからプレイヤー参照を取り出す*/
	UFUNCTION(BlueprintCallable, Category = "AIBoss")
	APlayerBase* GetPlayerKey() const;

public:

	// AIControllerのPawn所持
	virtual void OnPossess(class APawn* InPawn)override;
	// AIControllerのPawn所持解除
	virtual void OnUnPossess() override;

	/** 
	* BehaviorTreeを動かすコンポーネント。手動で持つことでStart/Stopのタイミングを自分で制御できる
	* （StateTreeに起動の主導権を渡すため、OnPossessでの自動起動はやめている）
	*/
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorComp;

	/** Blackboard。OnPossessで先に初期化しておくので、戦闘開始前でもSetPlayerKeyが機能する*/
	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	/**
	* 起動するBehaviorTreeアセット。
	* フェーズごとに別BTを使うことになるが、その時にここをTArrayへ拡張する
	*/
	UPROPERTY(EditDefaultsOnly, Category = "AI|Boss")
	TObjectPtr<UBehaviorTree> BossBehaviorTree;

	/** Blackboard上のプレイヤーキー名。BBアセット側のキー名と一致させる*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PlayerKeyName = TEXT("Player");
};
