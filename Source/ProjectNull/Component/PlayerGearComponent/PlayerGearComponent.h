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

	/// <summary>
	/// ギアによって移動がブロックされているかどうかを判定する
	/// </summary>
	/// <returns>移動がブロックされている場合はtrue、そうでない場合はfalse</returns>
	bool IsMovementBlockedByGear() const;

	void ExecuteGear(int32 GearIndex);

	//　セッター
	//　プレイヤー（持ち主）をセットするメソッド
	inline void SetOwnerPlayer(APlayerBase* Player) { OwnerPlayer = Player; }

	//　ゲッター
	inline const TArray<UGearBase*>& GetPlayerGears() const { return PlayerGears; }
	inline int32 GetCurrentGearLevel() const { return CurrentGearLevel; }

private:

	//　プレイヤー（持ち主）のポインタ
	UPROPERTY()
	APlayerBase* OwnerPlayer;

	//　プレイヤーの装備ギアクラスの配列
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UGearBase*> PlayerGears;

	//　現在ギアのレベル
	UPROPERTY(EditAnywhere)

	int32 CurrentGearLevel;
};
