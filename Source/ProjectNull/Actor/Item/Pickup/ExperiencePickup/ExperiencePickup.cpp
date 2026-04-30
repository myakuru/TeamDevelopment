#include "ExperiencePickup.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>

AExperiencePickup::AExperiencePickup()
{
		
}

void AExperiencePickup::OnUpdate(APawn* Player, float DeltaTime)
{
	if (!Player) { return; }

	UE_LOG(LogTemp, Warning, TEXT("ExperienceUpdate"));

	const FVector MyLocation = GetActorLocation();
	const FVector PlayerLocation = Player->GetActorLocation();
	
	const float DistanceSq = FVector::DistSquared(MyLocation, PlayerLocation);

	/** 未追尾なら近づいたときだけ開始*/
	if (!ExperienceParam.bChasing)
	{
		if (DistanceSq <= ExperienceParam.DetectRange * ExperienceParam.DetectRange)
		{
			ExperienceParam.bChasing = true;
		}
		else
		{
			return;
		}
	}

	/** 取得*/
	if (DistanceSq <= ExperienceParam.PickupRange * ExperienceParam.PickupRange)
	{
		Pickup(Player);
		return;
	}

	MoveToPlayer(Player->GetActorLocation(), DeltaTime);
}

void AExperiencePickup::MoveToPlayer(const FVector& PlayerLocation, float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Experience MoveToPlayer"));

	/** 方向を求める*/
	ExperienceParam.MoveDir = PlayerLocation - GetActorLocation();
	ExperienceParam.MoveDir.Normalize();

	/** 実際に動いた距離*/
	FVector MoveDistance = ExperienceParam.MoveDir* ExperienceParam.MoveSpeed * DeltaTime;
	FVector NextPos = GetActorLocation() + MoveDistance;

	/** 更新した座標をセット*/
	SetActorLocation(NextPos, true);
}

void AExperiencePickup::Pickup(APawn* Player)
{
	/** Playerをキャストして加算*/

	/** Managerのリストから削除*/
	if (UItemManagerSubsystem* ItemSubsystem = 
		GetWorld()->GetSubsystem<UItemManagerSubsystem>())
	{
		ItemSubsystem->RemovePickupItem(this);
	}

	UE_LOG(LogTemp, Warning, TEXT("Experience Destroy"));
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	Destroy();
}