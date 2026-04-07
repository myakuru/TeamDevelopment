#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBase.generated.h"


//　カメラスプリングアームコンポーネント
class USpringArmComponent;

//　カメラコンポーネント
class UCameraComponent;

//　攻撃コンポーネント
class UPlayerAttackComponent;

//　ギアコンポーネント
class UPlayerGearComponent;

/// <summary>
///　プレイヤー（ワールド内の自身が操作するキャラクター）の中間基底クラス
/// </summary>
UCLASS()
class PROJECTNULL_API APlayerBase : public ACharacter
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


	/// <summary>
	/// 移動関連のパラメータをリセットする関数
	/// </summary>
	void ResetMovementParameters();
	
	//　ゲッター
	inline UPlayerGearComponent* GetGearComponent() const { return GearComponent; }


private:

	/// <summary>
	/// 移動できるかどうかを判定する
	/// </summary>
	/// <returns>移動できるならtrue,移動できないならfalse</returns>
	bool CanMove();

	//　最大加速度
	UPROPERTY(EditAnywhere)
	float MaxAcceleration;

	//　================================================================
	//　プレイヤーのコンポーネント
	//　================================================================
	
	//　カメラスプリングアームコンポーネント
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	//　カメラコンポーネント
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	UCameraComponent* CameraComponent;

	//　攻撃コンポーネント
	UPROPERTY(VisibleAnywhere,Category = "Attack")
	UPlayerAttackComponent* AttackComponent;

	//　ギアコンポーネント
	UPROPERTY(VisibleAnywhere,Category = "Gear")
	UPlayerGearComponent* GearComponent;
};
