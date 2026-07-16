
#include "CharacterRuntimeData.h"

#include "ProjectNull/Data/ExpUpgradeDataTable/ExpUpgradeDataTable.h"

float FAttackRuntimeData::GetFinalPower()
{
	Final = Base * Scaling;
	return Final;
}

UCharacterRuntimeData::UCharacterRuntimeData()
{
	EffectMultipliers = {};
}

float UCharacterRuntimeData::GetCharacterAttackPower()
{
	// 最終的な攻撃力に強化画面でUpされる攻撃力を掛け算
	return Attack.GetFinalPower() * GetEffectMultiplier(EUpgradeEffectType::AttackDamage);
}

float UCharacterRuntimeData::GetEffectMultiplier(EUpgradeEffectType Type) const
{
	const float* Found = EffectMultipliers.Find(Type);
	return Found ? *Found : 1.0f;
}
