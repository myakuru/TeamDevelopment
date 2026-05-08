// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerParameterData.h"

UPlayerParameterData::UPlayerParameterData()
	: SkillCooldownTime({0.0f, 0.0f, 0.0f})
	, SkillCooldownElapsed({ 0.0f, 0.0f, 0.0f })
{
	
}

void UPlayerParameterData::UpdateSkillCooldown(int32 SkillIndex, float DeltaTime)
{
	if (!SkillCooldownElapsed.IsValidIndex(SkillIndex)) return;
	if (!SkillCooldownTime.IsValidIndex(SkillIndex)) return;

	const float MaxTime = SkillCooldownTime[SkillIndex];
	if (MaxTime <= 0.0f) return;

	// 経過時間を加算（最大時間でクランプ）
	SkillCooldownElapsed[SkillIndex] = FMath::Min(SkillCooldownElapsed[SkillIndex] + DeltaTime, MaxTime);

	// 0.0（開始）→ 1.0（完了）に変換してBroadcast
	const float Rate = SkillCooldownElapsed[SkillIndex] / MaxTime;

	// クールダウンの残り時間を計算
	const float CooldownRunTime = MaxTime - SkillCooldownElapsed[SkillIndex];

	// PlayerHUDWidgetでスキルのクールダウン用のUIとテキストの変数が渡される
	OnSkillCooldownChanged.Broadcast(SkillIndex, Rate, CooldownRunTime);
}

void UPlayerParameterData::ResetSkillCooldown(int32 SkillIndex)
{
	if (!SkillCooldownElapsed.IsValidIndex(SkillIndex)) return;

	// 経過時間を0にリセット
	SkillCooldownElapsed[SkillIndex] = 0.0f;

	// 0.0（開始）→ 1.0（完了）に変換してBroadcast
	OnSkillCooldownChanged.Broadcast(SkillIndex, 0.0f, 0.0f);
}


