//#pragma once
//
//// ------------------------------------------------------------------------------------
//// Includes
//// ------------------------------------------------------------------------------------
//#include "CoreMinimal.h"
//#include "GameFramework.h"
//#include "PhysicalMaterials/PhysicalMaterial.h"
//#include "Impact.generated.h"
//
//class UNiagaraSystem;
//
//UCLASS()
//class PROJECTNULL_API ImpactActor : public AActor
//{
//	GENERATED_BODY()
//
//public:
//
//	ImpactActor();
//
//protected:
//
//	// ------------------------------------------------------------------------------------
//	// protected variables
//	// ------------------------------------------------------------------------------------
//
//	// SurfaceTypeごとのNiagara
//	UPROPERTY(EditDefaultOnly, Category = "Effect")
//	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<UNiagaraSystem>> ImpactEffects;
//
//	// 対応するSurfaceTypeがなかったとき用
//	UPROPERTY(EditDefaultOnly, Category = "Effect")
//	TObjectPtr<UNiagaraSystem> DefaultImpactEffect;
//
//	// 地面判定の距離
//	UPROPERTY(EditDefaultOnly, Category = "Effect")
//	float TraceDistance = 300.0f;
//
//	// ------------------------------------------------------------------------------------
//	// protected Factory
//	// ------------------------------------------------------------------------------------
//	
//	void SpawnImpactEffectFromHit(const FHitResult& Hit);
//
//	bool TraceGroundAndSpawnImpactEffect(const FVector& StartLocation);
//
//	UNiagaraSystem* GetImpactEffectBySurface(EPhysicalSurface SurfaceType) const;
//};
//
