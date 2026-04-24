
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

void UGameProgressSubsystem::UpdatePhase() 
{
	int32 killCount = KillCount;

	// �|�����G���Ɋ�Â��Ăǂ̃t�F�[�Y�Ȃ̂�����肷��
	for (int32 phaseNum = 0; phaseNum < PhaseThresholds.Num(); ++phaseNum) {

		killCount -= PhaseThresholds[phaseNum];
		// �t�F�[�Y臒l�������ꍇ�̓t�F�[�Y��X�V����
		if (killCount < 0) {
			Phase = phaseNum;
			break;
		}
	}
}