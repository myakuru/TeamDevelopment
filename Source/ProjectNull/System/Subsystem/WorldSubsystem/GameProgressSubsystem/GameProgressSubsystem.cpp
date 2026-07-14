
#include "GameProgressSubsystem.h"


void UGameProgressSubsystem::AddKillCount(int32 Value)
{
	// ���Z����
	KillCount += Value;

	// �t�F�[�Y�X�V���\�b�h��Ă�
	UpdatePhase();
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
	if (KillCount >= PhaseThresholds)
	{
		SetPhase(Phase + 1);
		KillCount = 0;

	}
}