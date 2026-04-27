#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameProgressSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, int);

/// <summary>
/// ゲームの進行管理クラス
/// </summary>
UCLASS()
class PROJECTNULL_API UGameProgressSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// 敵を倒した際に倒した敵数を加算
	/// </summary>
	/// <param name="Value">加算したい敵数</param>
	void AddKillCount(int32 Value = 1);

	int32 GetKillCount() const { return KillCount; }

	int32 GetPhase() const { return Phase; }

	/// <summary>
	/// フェーズを通知する
	/// </summary>
	FOnPhaseChanged OnPhaseChanged;

	void SetPhase(int NewPhase);

	/// <summary>
	/// フェーズの境界を定義する倒した敵数閾値
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<int32> PhaseThresholds;

private:

	/// <summary>
	/// 敵を倒した数
	/// </summary>
	int32 KillCount = 0;

	/// <summary>
	///	ゲームのフェーズ数
	/// </summary>
	int32 Phase = 0;

	/// <summary>
	/// ゲームのフェーズ更新
	/// ※毎フレーム呼ぶメソッドではない
	/// </summary>
	void UpdatePhase();
};
