
#pragma once

#include "CoreMinimal.h"
#include "../RobotController.h"
#include "DebugPlayerController.generated.h"

class ARobot;

UCLASS()
class PROJECTNULL_API ADebugPlayerController : public ARobotController
{
	GENERATED_BODY()
	
public:

	ADebugPlayerController();

private:

	virtual void BeginPlay()			override;
	virtual void SetupInputComponent()	override;
	virtual void GearExecute01(const FInputActionValue& GearActionValue01) override;

	// デバッグモードの切り替え関数
	void ChangeDebugMode(const FInputActionValue& ActionValue);

	/// <summary>
	/// 上方向への移動入力処理関数
	/// </summary>
	/// <param name="ActionValue">入力値</param>
	void MoveUp(const FInputActionValue& ActionValue);


	// 上方向への移動入力アクション
	UPROPERTY(EditAnywhere, Category = "DebugInput")
	TObjectPtr<UInputAction> MoveUpAction;

	// デバッグモード切り替え入力アクション
	UPROPERTY(EditAnywhere, Category = "DebugInput")
	TObjectPtr<UInputAction> ChangeDebugModeAction;

	// デバッグモードかどうかのフラグ
	bool bIsDebugMode;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARobot> GameClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARobot> DebugClass;
};
