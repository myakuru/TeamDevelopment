#pragma once

// ------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "EnemyBossAnimNotify_AttackHit.generated.h"

// ------------------------------------------------------------------------------------
// 前方宣言
// ------------------------------------------------------------------------------------
class UNiagaraSystem;
class USkeletalMeshComponent;

UCLASS()
class PROJECTNULL_API UEnemyBossAnimNotify_AttackHit : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	// ------------------------------------------------------------------------------------
	// public method
	// ------------------------------------------------------------------------------------
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference)override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	// ------------------------------------------------------------------------------------
	// private variables
	// ------------------------------------------------------------------------------------
	/** 判定を出すボーン/ソケット名*/
	UPROPERTY(EditAnywhere, Category = "Attack")
	FName  SocketName = TEXT("Null");

	/** スフィアの半径*/
	UPROPERTY(EditAnywhere, Category = "Attack")
	float Radius = 50.0f;

	/** スフィアの座標オフセット*/
	UPROPERTY(EditAnywhere, Category = "Attack")
	FVector SphereOffset;

	/** デバッグ表示*/
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool bDrawDebug = true;

	// ------------------------------------------------------------------------------------
	/** 地面ヒットエフェクト*/
	// ------------------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Effect")
	bool bSpawnGroundImpactEffect = true;

	// Notify開始から何秒後にエフェクトを出すか
	UPROPERTY(EditAnywhere, Category = "Effect")
	float GroundImpactDelay = 0.0f;

	// 地面Traceの上方向距離
	UPROPERTY(EditAnywhere, Category = "Effect")
	float GroundTraceUpDistance = 100.0f;

	// 地面Traceの下方向距離
	UPROPERTY(EditAnywhere, Category = "Effect")
	float GroundTraceDownDistance = 500.0f;

	// SurfaceTypeごとのNiagara
	UPROPERTY(EditAnywhere, Category = "Effect")
	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<UNiagaraSystem>> GroundImpactEffects;

	// 対応Surfaceがなかった時の対応
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> DefaultGroundImpactEffect;

	// デバッグ表示用
	UPROPERTY(EditAnywhere, Category = "Effect")
	bool bDrawGroundImpactDebug = false;

private:

	/** この攻撃で当たったアクター（二重ヒット防止）*/
	TSet<TWeakObjectPtr<AActor>> HitActors;

	bool bGroundImpactEffect = false;
	float ElapsedTime = 0.0f;

	bool TrySpawnGroundImpactEffect(USkeletalMeshComponent* MeshComp, const FVector& TraceCenter);
	UNiagaraSystem* GetGroundImpactEffect(EPhysicalSurface SurfaceType) const;

};