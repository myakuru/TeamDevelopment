#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameProgressSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, int);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnKillCount, int,int);

/// <summary>
/// ゲームの進行管理クラス
/// </summary>
UCLASS()
class PROJECTNULL_API UGameProgressSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(
		FSubsystemCollectionBase& Collection
	) override;

	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(
			UGameProgressSubsystem,
			STATGROUP_Tickables
		);
	}

	/// <summary>
	/// 敵を倒した際に倒した敵数を加算
	/// </summary>
	/// <param name="Value">加算したい敵数</param>
	void AddKillCount(int32 Value = 1);
	void AddTyuuBossCount(int32 Value = 1);
	void AddKillBossCount(int32 Value = 1);

	int32 GetKillCount() const { return KillCount; }

	int32 GetPhase() const { return Phase; }
	int32 GetPhaseThresholds() { return PhaseThresholds; }

	/// <summary>
	/// フェーズを通知する
	/// </summary>
	FOnPhaseChanged OnPhaseChanged;

	FOnKillCount OnKillCountChanged;

	void SetPhase(int NewPhase);

	void SetPhaseThresholds(int32 Num);

	void SetFinalWave(bool _flg) { FinalWave = _flg; }
	void SetBossWave(bool _flg) { BossWave = _flg; FinalWave = false; }

private:

	/// <summary>
	/// 敵を倒した数
	/// </summary>
	int32 KillCount = 0;

	/// <summary>
	///	ゲームのフェーズ数
	/// </summary>
	int32 Phase = 0;

	// フェーズの境界を定義する倒した敵数閾値（Spawnerからフェーズに変更があった際にデータを渡す）
	int32 PhaseThresholds = 1;

	bool FinalWave = false;

	bool BossWave = false;

	/// <summary>
	/// ゲームのフェーズ更新
	/// ※毎フレーム呼ぶメソッドではない
	/// </summary>
	void UpdatePhase();

	bool GameClearFlg = false;
	float ClearCountDuration	= 3.0f;
	float ClearCountTime		= 0.0f;

};
