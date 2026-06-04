
#include "PlayerParameterData.h"

UPlayerParameterData::UPlayerParameterData()
	: SkillCooldownTime({0.0f, 0.0f, 0.0f})
	, SkillCooldownElapsed({ 0.0f, 0.0f, 0.0f })
{
	
}

void UPlayerParameterData::UpdateSkillCooldown(int32 InSkillIndex, float InCooldownTime)
{
	if (!SkillCooldownElapsed.IsValidIndex(InSkillIndex)) return;
	if (!SkillCooldownTime.IsValidIndex(InSkillIndex)) return;

	float Rate = FMath::Clamp(InCooldownTime, 0.0f, 1.0f);

	// PlayerHUDWidgetでスキルのクールダウン用のUIとテキストの変数が渡される
	OnSkillCooldownChanged.Broadcast(InSkillIndex, Rate, InCooldownTime);
}

void UPlayerParameterData::ResetSkillCooldown(int32 SkillIndex)
{
	if (!SkillCooldownElapsed.IsValidIndex(SkillIndex)) return;

	// 経過時間を0にリセット
	SkillCooldownElapsed[SkillIndex] = 0.0f;

	// 0.0（開始）→ 1.0（完了）に変換してBroadcast
	OnSkillCooldownChanged.Broadcast(SkillIndex, 0.0f, 0.0f);
}


