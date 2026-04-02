#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerGearComponent.generated.h"

//　ギアの基底クラス
class UGearBase;

//　プレイヤーの中間基底クラス
class APlayerBase;


/// <summary>
/// プレイヤーの装備ギアコンポーネントクラス
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UPlayerGearComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerGearComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//　プレイヤー（持ち主）をセットするメソッド
	inline void SetOwnerPlayer(APlayerBase* Player) { OwnerPlayer = Player; }

	inline int32 GetCurrentGearLevel() const { return CurrentGearLevel; }
private:

	//　プレイヤー（持ち主）のポインタ
	UPROPERTY()
	APlayerBase* OwnerPlayer;

	//　プレイヤーの装備ギアクラスの配列
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UGearBase*> PlayerGears;

	//　現在ギアのレベル
	int32 CurrentGearLevel;
};
