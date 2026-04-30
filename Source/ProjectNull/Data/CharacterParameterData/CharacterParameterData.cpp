// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterParameterData.h"

UCharacterParameterData::UCharacterParameterData()
	: Health			(0.0f)
	, MaxHealth			(0.0f)
	, GearEnergy		(0.0f)
	, MaxGearEnergy		(0.0f)
	, Experience		(0.0f)
	, MaxExperience		(0.0f)
	, SkillCooldownTime({0.0f, 0.0f, 0.0f})
	, SkillCooldownElapsed({ 0.0f, 0.0f, 0.0f })
{
}

void UCharacterParameterData::DecreaseHealth(float Amount)
{
	// Maxはエディターで変更される
	Health = FMath::Clamp(Health - Amount, 0.0f, MaxHealth);

	// 変更があれば、HPのバーが短くなる
	OnHealthChanged.Broadcast(Health, MaxHealth);
}

void UCharacterParameterData::AddGearEnergy(float Amount)
{
	// Maxはエディターで変更される
	GearEnergy = FMath::Clamp(GearEnergy + Amount, 0.0f, MaxGearEnergy);

	// 変更があれば、ギアのテキストが更新される
	OnGearEnergyChanged.Broadcast(GearEnergy);
}

void UCharacterParameterData::AddExperience(float Amount)
{
	// Maxはエディターで変更される
	Experience = FMath::Clamp(Experience + Amount, 0.0f, MaxExperience);

	// 変更があれば、経験値のバーが伸びていく
	OnExperienceChanged.Broadcast(Experience, MaxExperience);
}

void UCharacterParameterData::UpdateSkillCooldown(int32 SkillIndex, float DeltaTime)
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

void UCharacterParameterData::ResetSkillCooldown(int32 SkillIndex)
{
	if (!SkillCooldownElapsed.IsValidIndex(SkillIndex)) return;

	// 経過時間を0にリセット
	SkillCooldownElapsed[SkillIndex] = 0.0f;

	// 0.0（開始）→ 1.0（完了）に変換してBroadcast
	OnSkillCooldownChanged.Broadcast(SkillIndex, 0.0f, 0.0f);
}

