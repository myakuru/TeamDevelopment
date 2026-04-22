// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectInstance.h"
#include"../Data/EffectData.h"
#include"../Effect/EffectBase/UEffectBase.h"

void FEffectInstance::Initialize(UObject* a_Outer)
{
	if (Data && Data->EffectClass) {
		RuntimeEffect = NewObject<UUEffectBase>(a_Outer, Data->EffectClass);
	}
}
