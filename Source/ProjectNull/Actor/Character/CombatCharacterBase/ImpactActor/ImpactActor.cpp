//// ------------------------------------------------------------------------------------
////		Includes
//// ------------------------------------------------------------------------------------
//#include "ImpactActor.h"
//
//#include "NiagaraFunctionLibrary.h"
//#include "NiagaraSystem.h"
//#include "DrawDebugHelpers.h"
//#include "Engine/World.h"
//
//// ------------------------------------------------------------------------------------
////		コンストラクタ
//// ------------------------------------------------------------------------------------
//ImpactActor::ImpactActor()
//{
//	PrimaryActorTick.bCanEverTick = true;
//}
//
//// ------------------------------------------------------------------------------------
////		エフェクトを出すかの判定
//// ------------------------------------------------------------------------------------
//bool ImpactActor::TraceGroundAndSpawnImpactEffect(const FVector& StartLocation)
//{
//	if (!GetWorld()) 
//	{ return false; }
//
//	const FVector Start = StartLocation;
//	const FVector End = StartLocation - FVector(0.0f, 0.0f, TraceDistance);
//
//	FHitResult Hit;
//
//	FCollisionQueryParams Params(SCENE_QUERY_STAT(ImpactEffectTrace), true);
//	Params.bReturnPhysicalMaterial = true;
//	Params.AddIgnoredActor(this);
//
//	const bool bHit = GetWorld()->LineTraceSingleByChannel(
//		Hit, Start, End, ECC_Visibility, Params);
//
//#if WITH_EDITOR
//	DrawDebugLine(
//		GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red,
//		false, 2.0f, 0, 2.0f);
//#endif
//
//	if (!bHit)
//	{
//		return false;
//	}
//
//	SpawnImpactEffectHit(Hit);
//	return true;
//}
//
//void ImpactActor::SpawnImpactEffectFromHit(const FHitResult& Hit)
//{
//	EPhysicalSurface SurfaceType = SurfaceType_Default;
//
//	if (Hit.PhysMaterial.IsValid())
//	{
//		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
//	}
//
//	UNiagaraSystem* Effect = GetImpactEffectBySurface(SurfaceType);
//
//	if (!Effect)
//	{
//		return;
//	}
//
//	const FVector SpawnLocation = Hit.ImpactPoint + Hit.ImpactNormal * 2.0f;
//
//	// Niagaraの上方向、ローカルZを地面の法線に合わせる
//	const FRotator SpawnRotation = FRotationMatrix::MakeFromZ(Hit.ImpactNormal).Rotator();
//
//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
//		GetWorld(), Effect, SpawnLocation, SpawnRotation,
//		FVector(1.0f), true, true, ENCPOOLMethod::AutoRelease, true);
//
//#if WITH_EDITOR
//	UE_LOG(LogTemp, Warning, TEXT("Impact Surface: %d, PhysMat: %s, HitActor: %s"),
//		static_cast<int32>(SurfaceType),
//		*GetNameSafe(Hit.PhysMaterial.Get()),
//		*GetNameSafe(Hit.GetActor())
//	);
//#endif
//}
//
//UNiagaraSystem* ImpactActor::GetImpactEffectBySurface(EPhysicalSurface SurfaceType) const
//{
//	if (const TObjectPtr<UNiagaraSystem>* FoundEffect = ImpactEffect.Find(SurfaceType))
//	{
//		return FoundEffect->Get();
//	}
//
//	return DefaultImpactEffect.Get();
//}
//
