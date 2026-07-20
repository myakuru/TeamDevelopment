
#include "GameProgressSubsystem.h"
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Stage/Manager/StageManager.h>
#include <ProjectNull/System/WorldSystem/EnemySpawner/EnemySpawner.h>
#include "Kismet/GameplayStatics.h"

void UGameProgressSubsystem::Initialize(
	FSubsystemCollectionBase& Collection
)
{
	Super::Initialize(Collection);

	KillCount	= 0;
	Phase		= 0;
}

void UGameProgressSubsystem::Deinitialize()
{
	OnPhaseChanged.Clear();

	Super::Deinitialize();
}

void UGameProgressSubsystem::Tick(float DeltaTime)
{
	if (!GameClearFlg)return;

	ClearCountTime += DeltaTime;
	if (ClearCountTime >= ClearCountDuration)
	{
		if (USuperGameInstance* GameInstance = GetWorld()->GetGameInstance<USuperGameInstance>())
		{
			GameInstance->GetStageManagerSubsystem()->InGameFinalize();
		}
	}
}

void UGameProgressSubsystem::AddKillCount(int32 Value)
{
	if (FinalWave || BossWave) { return; }
	// ���Z����
	KillCount += Value;

	// �t�F�[�Y�X�V���\�b�h��Ă�
	UpdatePhase();
	OnKillCountChanged.Broadcast(KillCount, PhaseThresholds);
}

void UGameProgressSubsystem::AddTyuuBossCount(int32 Value)
{
	KillCount += Value;
	UpdatePhase();
	OnKillCountChanged.Broadcast(KillCount, PhaseThresholds);
}

void UGameProgressSubsystem::AddKillBossCount(int32 Value)
{
	KillCount += Value;
	UpdatePhase();
	OnKillCountChanged.Broadcast(KillCount, PhaseThresholds);
}

void UGameProgressSubsystem::SetPhase(int NewPhase)
{
	if (Phase == NewPhase) { return; }
	Phase = NewPhase;
	OnPhaseChanged.Broadcast(Phase);
}

void UGameProgressSubsystem::SetPhaseThresholds(int32 Num)
{
	PhaseThresholds = Num;
}

void UGameProgressSubsystem::UpdatePhase() 
{
	if (FinalWave)
	{
		SetPhase(Phase + 1);
		KillCount = 0;
	}

	if (BossWave)
	{
		GameClearFlg = true;

		AActor* FoundActor = UGameplayStatics::GetActorOfClass(
			GetWorld(), AEnemySpawner::StaticClass());

		AEnemySpawner* EnemySpawner = Cast<AEnemySpawner>(FoundActor);

		if (IsValid(EnemySpawner))
		{
			EnemySpawner->SetSpawnStop(true);
		}
	}


	if (KillCount >= PhaseThresholds)
	{
		SetPhase(Phase + 1);
		KillCount = 0;
	}
}

