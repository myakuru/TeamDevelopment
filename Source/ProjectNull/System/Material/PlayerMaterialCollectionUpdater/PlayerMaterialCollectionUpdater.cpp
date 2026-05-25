
#include "PlayerMaterialCollectionUpdater.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

UPlayerMaterialCollectionUpdater::UPlayerMaterialCollectionUpdater()
{
}

void UPlayerMaterialCollectionUpdater::Initialize(APlayerBase* InOwnerPlayer)
{
	OwnerPlayer = InOwnerPlayer;
}

void UPlayerMaterialCollectionUpdater::Update(float DeltaTime)
{
	UpdateMapMaterialCollection(DeltaTime);

}

void UPlayerMaterialCollectionUpdater::UpdateMapMaterialCollection(float DeltaTime)
{
	if (!OwnerPlayer) { return; }

	// プレイヤーの座標取得
	const FVector PlayerLocation = OwnerPlayer->GetActorTransform().GetLocation();

	UKismetMaterialLibrary::SetVectorParameterValue(
		GetWorld(),
		MapMaterialCollection,
		TEXT("Position"),
		FLinearColor(
			PlayerLocation.X,
			PlayerLocation.Y,
			PlayerLocation.Z,
			1.0f));

}
