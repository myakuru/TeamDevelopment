
#include "FloatingWeaponStateBase.h"
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>

UFloatingWeaponStateBase::UFloatingWeaponStateBase()
{
}
void UFloatingWeaponStateBase::Update(AActor* OwnerActor, float DeltaTime)
{
	if (!Owner) { return; }
	Owner->SetTransform(Transform);
}
