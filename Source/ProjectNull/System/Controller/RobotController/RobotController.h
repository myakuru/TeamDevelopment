#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RobotController.generated.h"

//　入力アクションの値を表す構造体
struct FInputActionValue;

//　入力マッピングコンテキストクラス
class UInputMappingContext;

//　入力アクションクラス
class UInputAction;


class UPlayerHUDWidget;

/// <summary>
/// ロボット（プレイヤー）のコントローラークラス
/// ロボットの操作や入力処理を担当するクラス
/// </summary>
UCLASS()
class PROJECTNULL_API ARobotController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ARobotController();

protected:

	virtual void BeginPlay()			override;
	virtual void SetupInputComponent()	override;

private:

	/// <summary>
	/// 入力マッピングコンテキスト関連の初期化
	/// </summary>
	void InitializeInputContext();

	/// <summary>
	/// 移動入力処理関数
	/// </summary>
	/// <param name="MoveActionValue">入力値</param>
	void Move(const FInputActionValue& MoveActionValue);

	/// <summary>
	/// 視点入力処理関数
	/// </summary>
	/// <param name="LookActionValue">入力値</param>
	void Look(const FInputActionValue& LookActionValue);

	/// <summary>
	/// ジャンプ入力処理関数
	/// </summary>
	/// <param name="LookActionValue">入力値</param>
	void Jump(const FInputActionValue& JumpActionValue);

	/// <summary>
	/// ギアの実行関数　01
	/// </summary>
	/// <param name="GearActionValue01">入力値</param>
	void GearExecute01(const FInputActionValue& GearActionValue01);

	//　入力マッピングコンテキスト
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	//　移動入力アクション
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	//　視点入力アクション
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	//　ジャンプ入力アクション
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	//　ギア入力アクション　01
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> GearAction01;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;

	UPROPERTY()
	UPlayerHUDWidget* PlayerHUD;


};
