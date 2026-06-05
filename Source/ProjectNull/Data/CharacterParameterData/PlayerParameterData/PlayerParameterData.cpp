
#include "PlayerParameterData.h"

UPlayerParameterData::UPlayerParameterData()
	: SkillCooldownTime({0.0f, 0.0f, 0.0f})
	, SkillCooldownElapsed({ 0.0f, 0.0f, 0.0f })
{
	
}

void UPlayerParameterData::UpdateSkillCooldown(int32 InSkillIndex, float InCooldownTime, float InMaxCooldown)
{
	if (!SkillCooldownElapsed.IsValidIndex(InSkillIndex)) return;

	// PlayerHUDWidgetでスキルのクールダウン用のUIとテキストの変数が渡される
	OnSkillCooldownChanged.Broadcast(InSkillIndex, InCooldownTime, InMaxCooldown);
}

void UPlayerParameterData::ResetSkillCooldown(int32 SkillIndex)
{
	if (!SkillCooldownElapsed.IsValidIndex(SkillIndex)) return;
	// 経過時間を0にリセット
	SkillCooldownElapsed[SkillIndex] = 0.0f;
}


