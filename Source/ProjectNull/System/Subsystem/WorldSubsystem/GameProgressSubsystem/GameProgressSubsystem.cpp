
#include "GameProgressSubsystem.h"


void UGameProgressSubsystem::AddKillCount(int32 Value)
{
	// 加算処理
	KillCount += Value;

	// フェーズ更新メソッドを呼ぶ
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

	// 倒した敵数に基づいてどのフェーズなのかを決定する
	for (int32 phaseNum = 0; phaseNum < PhaseThresholds.Num(); ++phaseNum) {

		killCount -= PhaseThresholds[phaseNum];
		// フェーズ閾値だった場合はフェーズを更新する
		if (killCount < 0) {
			Phase = phaseNum;
			break;
		}
	}
}