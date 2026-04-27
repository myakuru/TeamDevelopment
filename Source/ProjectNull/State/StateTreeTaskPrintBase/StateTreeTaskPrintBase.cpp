#include "StateTreeTaskPrintBase.h"

EStateTreeRunStatus UStateTreeTaskPrintBase::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	// eæ“¾(‘€ì‚³‚ê‚éPawn)
	UE_LOG(LogTemp, Warning, TEXT("Tick2!!"));

	Timer += DeltaTime;

	if(Timer<5.0f)return EStateTreeRunStatus::Running;
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("End!! : Minuts%f"),Timer);
		return EStateTreeRunStatus::Succeeded;
	}
}

EStateTreeRunStatus UStateTreeTaskPrintBase::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	// eæ“¾(‘€ì‚³‚ê‚éPawn)
	UE_LOG(LogTemp, Warning, TEXT("EnterStart!!"));
	
	return EStateTreeRunStatus::Running;
}

void UStateTreeTaskPrintBase::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	// eæ“¾(‘€ì‚³‚ê‚éPawn)
	UE_LOG(LogTemp, Warning, TEXT("Exit!!"));
}
