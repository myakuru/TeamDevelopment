
#include "EnemyManagerSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectNull/Actor/Character/Enemy/EnemyBase.h"


void UEnemyManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UWorldSubsystem::Initialize(Collection);
}

void UEnemyManagerSubsystem::RegisterEnemy(AEnemyBase* Enemy)
{
	if (!Enemy) { return; }

	EnemyGruntList.Add(Enemy);
}

void UEnemyManagerSubsystem::RemoveEnemy(AEnemyBase* Enemy)
{
	if (!Enemy) { return; }

	EnemyGruntList.Remove(Enemy);
}

void UEnemyManagerSubsystem::UpdateEnemies(float DeltaTime)
{
	// プレイヤーの情報を取得する（0番:1P）
	APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!pPlayerPawn) { return; }

	// すべて敵の更新メソッドを呼ぶ
	for (AEnemyBase* enemy : EnemyGruntList) 
	{
		if (enemy) 
		{
			enemy->OnUpdate(pPlayerPawn, DeltaTime);
		}
	}
}

