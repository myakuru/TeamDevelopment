#include "AIC_EnemyBoss.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

AAIC_EnemyBoss::AAIC_EnemyBoss(const class FObjectInitializer& ObjectInitializer)
{
	// BTとBBのコンポーネントの作成だけ行う
	// 実際にどれを使うのかはBPで指定
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("UBehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("UBlackboardComp"));
}

void AAIC_EnemyBoss::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Blackboardの初期化だけ行いBTはまだ起動しない
	// いつ戦闘を始めるかの判断はStateTreeに任せ、StartBossBehaviorTree()で起動する
	// BBを先に初期化しておくと、敵を発見した瞬間でもSetPlayerKeyが起動する
	if (BossBehaviorTree && BossBehaviorTree->BlackboardAsset)
	{
		BlackboardComp->InitializeBlackboard(*BossBehaviorTree->BlackboardAsset);
	}
}

void AAIC_EnemyBoss::OnUnPossess()
{
	Super::OnUnPossess();
	if (BehaviorComp)
	{
		BehaviorComp->StopTree();
	}
}

void AAIC_EnemyBoss::StartBossBehaviorTree()
{
	if (BehaviorComp && BossBehaviorTree)
	{
		// Combatフェーズに入るときStateTreeのタスクから呼ばれる
		BehaviorComp->StartTree(*BossBehaviorTree);
	}
}

void AAIC_EnemyBoss::StopBossBehaviorTree()
{
	if (BehaviorComp)
	{
		BehaviorComp->StopTree();
	}
}

void AAIC_EnemyBoss::NotifyBossBTFinished()
{
	// BTの終了タスクから呼ばれる。購読中のStateTreeタスクへ終わったことを通知し、遷移を促す
	OnBossBTFinished.Broadcast();
}

void AAIC_EnemyBoss::SetPlayerKey(APawn* player)
{
	if (!BlackboardComp) { return; }
	//ensure(BlackboardComp);

	// ブラックボードで作成したPlayerというキーにプレイヤー情報を入れる
	// BT側はこのキーを参照して追跡/攻撃対象を決める
	BlackboardComp->SetValueAsObject(PlayerKeyName, player);
}

APlayerBase* AAIC_EnemyBoss::GetPlayerKey() const
{
	if (!BlackboardComp) { return nullptr; }
	//ensure(BlackboardComp);

	return Cast<APlayerBase>(BlackboardComp->GetValueAsObject(PlayerKeyName));
}