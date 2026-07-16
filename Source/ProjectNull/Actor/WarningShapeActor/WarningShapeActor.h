#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/TransformCalculus3D.h"
#include "WarningShapeActor.generated.h"

/** 警告表示用形状アクター */
UCLASS()
class PROJECTNULL_API AWarningShapeActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarningShapeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief 無効化処理
	 */
	virtual void DeActivate();
	
public:
	
	virtual void Initialize(); 
	
	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム 
	 */
	virtual void Update(float DeltaTime);

	/**
	 * @brief 実行処理
	 */
	virtual void Execute(); 
	
	//~Begin Setters
	/**	目標半径値をセット */
	virtual void SetTargetRadius(const float InTargetRadius) { TargetRadius = InTargetRadius; } 
	
	/**	攻撃発動までの待ち時間のセット */
	virtual void SetActivationDelay(float InActivationDelay) { ActivationDelay = InActivationDelay; }
	
	// End Setters~

private:
	/**
	 * @brief 目標サイズのメッシュ
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TargetScaleMeshComponent;

	/**
	 * @brief 発動までの時間に沿ってサイズが可変するメッシュ
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> VariableScaleMeshComponent;
	
	/**
	 * @brief 目標半径値
	 */
	UPROPERTY()
	float TargetRadius=0.f;

	/**
	 * @brief 元の半径に対する目標半径値の割合
	 */
	UPROPERTY()
	float ScaleFactor=0.f;
	
	/**
	 * @brief 発動までの待ち時間
	 */
	UPROPERTY()
	float ActivationDelay = 0.f;

	/**
	 * @brief 経過時間
	 */
	UPROPERTY()
	float DurationTimer = 0.f;
};
