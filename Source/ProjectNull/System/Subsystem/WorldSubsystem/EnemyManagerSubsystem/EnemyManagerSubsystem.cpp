
#include "EnemyManagerSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>


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
	APawn* PPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PPlayerPawn) { return; }
	
	// すべて敵の更新メソッドを呼ぶ
	for (AEnemyBase* Enemy : EnemyGruntList) 
	{
		if (Enemy) 
		{
			Enemy->OnUpdate(PPlayerPawn, DeltaTime);
		}
	}
}

