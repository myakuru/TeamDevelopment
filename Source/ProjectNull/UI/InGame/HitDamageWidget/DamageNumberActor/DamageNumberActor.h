#pragma once

//----------------------------------------------------------
// Includes
//----------------------------------------------------------
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberActor.generated.h"

//----------------------------------------------------------
// Forward Declaration
//----------------------------------------------------------
class UWidgetComponent;
class UDamageNumberWidget;
class UDamageNumberPoolSubsystem;

//----------------------------------------------------------
// ADamageActorWidget class
//----------------------------------------------------------
UCLASS()
class PROJECTNULL_API ADamageNumberActor : public AActor
{
	GENERATED_BODY()

public:

	ADamageNumberActor();

	virtual void Tick(float DeltaTime)override;

	// プールから呼ばれる表示開始処理
	void ActivateDamageNumber(
		const FVector& WorldLocation,
		int32 Damage,
		bool bCritical,
		UDamageNumberPoolSubsystem* InOwnerPool);

	// プールへ戻るときの非表示処理
	void DeactivateDamageNumber();

	bool IsUsing() const { return bUsing; }

protected:

	virtual void BeginPlay()override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> WidgetComponent = nullptr;

	// Widget Blueprintをここに指定する
	UPROPERTY(EditAnywhere, Category = "Damage UI")
	TSubclassOf<UDamageNumberWidget> DamageWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Damage UI")
	float LifeTime = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Damage UI")
	float FloatSpeed = 80.0f;

	UPROPERTY(EditAnywhere, Category = "Damage UI")
	float RandomOffsetRadius = 25.0f;

	UPROPERTY()
	TObjectPtr<UDamageNumberPoolSubsystem> OwnerPool = nullptr;

	float CurrentTime = 0.0f;

	bool bUsing = false;

};