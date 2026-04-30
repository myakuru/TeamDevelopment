#include "STE_Test.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

void USTE_Test::TreeStart(FStateTreeExecutionContext& Context)
{
	Super::TreeStart(Context);

	Context.GetOwner();

	// 親取得(操作されるPawn)
	UE_LOG(LogTemp, Warning, TEXT("Start!!"));
}

void USTE_Test::TreeStop(FStateTreeExecutionContext& Context)
{
	Super::TreeStop(Context);

	// 親取得(操作されるPawn)
	UE_LOG(LogTemp, Warning, TEXT("Stop!!"));
}

void USTE_Test::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);
	
	Timer += DeltaTime;

	// 親取得(操作されるPawn)
	UE_LOG(LogTemp, Warning, TEXT("Tick!!"));
}
