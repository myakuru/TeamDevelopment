#pragma once

#include "CoreMinimal.h"
#include "../CombatCharacterBase.h"
#include "PlayerBase.generated.h"


// カメラスプリングアームコンポーネント
class USpringArmComponent;

// カメラコンポーネント
class UCameraComponent;

// 攻撃コンポーネント
class UPlayerAttackComponent;

// ギアコンポーネント
class UPlayerGearComponent;

USTRUCT(BlueprintType)
struct FExpSystem
{
	GENERATED_BODY()

public:

	FExpSystem():
		CurrentExp(0)
	{ }

public:

	void AddExp(int32 AddValue)
	{
		CurrentExp += AddValue;
	}

	UPROPERTY(EditAnywhere)
	int32 CurrentExp;
};


/// <summary>
/// プレイヤー（ワールド内の自身が操作するキャラクター）の中間基底クラス
/// </summary>
UCLASS()
class PROJECTNULL_API APlayerBase : public ACombatCharacterBase
{
	GENERATED_BODY()
public:

	APlayerBase();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime)													override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="InputVector">入力値に基づいてのベクトルXY</param>
	void Move(const FVector2d& InputVector);

	// ゲッター
	inline UPlayerGearComponent*	GetGearComponent() const { return GearComponent; }
	inline FExpSystem				GetEXPSystem() const { return ExpSystem; }


private:

	/// <summary>
	/// 移動できるかどうかを判定する
	/// </summary>
	/// <returns>移動できるならtrue,移動できないならfalse</returns>
	bool CanMove();

	// 経験値関連構造体
	FExpSystem ExpSystem;

	// ================================================================
	// プレイヤーのコンポーネント
	// ================================================================
	
	// カメラスプリングアームコンポーネント
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	// カメラコンポーネント
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	UCameraComponent* CameraComponent;

	// 攻撃コンポーネント
	UPROPERTY(VisibleAnywhere,Category = "Attack")
	UPlayerAttackComponent* AttackComponent;

	// ギアコンポーネント
	UPROPERTY(VisibleAnywhere,Category = "Gear")
	UPlayerGearComponent* GearComponent;
};
