// Fill out your copyright notice in the Description page of Project Settings.


#include "DashGear.h"

#include "../../../Actor/Character/Player/PlayerBase.h" 
#include "../../../Component/PlayerGearComponent/PlayerGearComponent.h"

UDashGear::UDashGear()
{
}

void UDashGear::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent)
{
	UGearBase::Initialize(Player, GearComponent);
	bCanExecute = true;
	
}

void UDashGear::Update(float DeltaTime)
{
	/*const bool bShouldUpdate = !bCanExecute;
	if (!bShouldUpdate) { return; }
	return;

	UGearBase::Update(DeltaTime);*/

	

}

void UDashGear::Execute()
{
	bCanExecute = false;

	GetWorld()->GetTimerManager().SetTimer(
		DurationTimerHandle,
		this,
		&UDashGear::ResetParams,
		Duration,
		false);
}

void UDashGear::ApplyLv1()
{
	FrontDash();
}

void UDashGear::ApplyLv2()
{

}

void UDashGear::ApplyLv3()
{

}

void UDashGear::ApplyLv4()
{

}

void UDashGear::ResetParams()
{
	bCanExecute = true;

}

bool UDashGear::FrontDash()
{
	if (!OwnerPlayer) { return false; }

	FVector Dir = OwnerPlayer->GetActorForwardVector();
	const float DashSpeed = 2000.f;
	OwnerPlayer->LaunchCharacter(Dir * DashSpeed, true, true);

	return false;
}
