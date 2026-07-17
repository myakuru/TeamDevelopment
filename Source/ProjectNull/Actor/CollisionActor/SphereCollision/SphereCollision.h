#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SphereCollision.generated.h"

/** 前方宣言 */
class UTextRenderComponent;
class USphereComponent;

/**
 * @brief 点滅しながら当たり判定を行う際に必要なパラメータ群
 */
USTRUCT()
struct FBlinkingHitElemental
{
	GENERATED_BODY()

public:

	/**
	 * @brief 前フレーム時の点滅がONかOFFか
	 */
	UPROPERTY()
	bool bPrevBlinking = true;
	
	/**
	 * @brief 点滅的に攻撃判定を行うか
	 */
	UPROPERTY(EditAnywhere)
	bool bIsBlinking = false;

	/**
	 * @brief 点滅する間隔
	 */
	UPROPERTY(EditAnywhere)
	float BlinkInterval = 0.1f;
};

/**
 * @brief 攻撃に使用する球のパラメータ群
 */
USTRUCT()
struct FSphereElemental
{
	GENERATED_BODY()

public:
	/**
	 * @brief 点滅に使用するパラメータ群
	 */
	UPROPERTY(EditAnywhere)
	FBlinkingHitElemental BlinkingHitElemental;
	
	/**
	 * @brief トランスフォーム系
	 */
	UPROPERTY(EditAnywhere)
	FTransform Transform;

	/**
	 * @brief 半径
	 */
	UPROPERTY(EditAnywhere)
	float Radius = 50.f;

	/**
	 * @brief 攻撃発動までの時間
	 */
	UPROPERTY(EditAnywhere)
	float ActivationDelay = 0.f;

	/**
	 * @brief 発動してからの攻撃有効時間
	 */
	UPROPERTY(EditAnywhere)
	float Duration = 1.f;
};

/**
* 球コンポーネントを配列として持つ攻撃クラス
*/
UCLASS()
class PROJECTNULL_API ASphereCollision : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASphereCollision();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Update(float InDeltaTime);

public:
	/**
	 * @brief ルートの親子関係を解除
	 */
	void DetachRootComponent()
	{
		if (!RootComponent) { return; }

		RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	}

	/**
	 * @brief コリジョンチャンネルとそれに対するレスポンスをセット
	 * @param InCollisionChannel	判定したいチャンネル(PawnやEnemy..)
	 * @param InCollisionResponse	それに対するレスポンス
	 */
	void SetAllCollisionResponseToChannel(
		const ECollisionChannel InCollisionChannel,
		const ECollisionResponse InCollisionResponse);

	/**
	 * @brief コリジョン判定の有効無効の切り替え
	 * @param InEnabled NoCollisionで無効
	 */
	void SetAllCollisionEnabled(const ECollisionEnabled::Type InEnabled);

	/** 攻撃が有効化された瞬間の時間をセット */
	void SetJustExecuteTime(const float InJustExecuteTime) { JustExecuteTime = InJustExecuteTime; }
	/* End Setters~ */

	/* ~Begin Getters */
	/**	この攻撃本来の発動から消滅までの最大時間を取得 */
	float GetBaseAttackDuration() const { return MaxDuration + MaxActivationDelay; }

	/**	発動してからの最大有効時間を取得 */
	float GetMaxDuration() const { return MaxDuration; }

	/**	球コンポーネント配列の取得 */
	const TArray<TObjectPtr<USphereComponent>>& GetSphereComponents() const
	{
		return SphereComponents;
	}

	/** 球情報を持つ構造体配列の取得 */
	const TArray<FSphereElemental>& GetSphereEntries() const
	{
		return SphereEntries;
	}

	/* End Getters~ */

private:
	/**
	 * @brief 点滅処理有効時のコリジョン有効無効切り替え更新処理
	 * @param InSphereComponent 球コンポーネント 
	 * @param OutBlinkingHitElemental 球に対応する点滅パラメータ群
	 * @param InCurrentTime 現在の時間
	 */
	void UpdateBlinkingHitCheck(
				const TObjectPtr<USphereComponent>& InSphereComponent
			,	FBlinkingHitElemental& OutBlinkingHitElemental
			,	const float InCurrentTime
		);

	/**
	 * @brief 点滅処理無効時のコリジョン有効無効切り替え更新処理
	 * @param InSphereComponent 球コンポーネント
	 * @param InSphereElemental 球に対応するパラメータ群
	 * @param InCurrentTime 現在の時間
	 */
	void UpdateDefaultHitCheck(
			const TObjectPtr<USphereComponent>& InSphereComponent
		,	const FSphereElemental& InSphereElemental
		,	const float InCurrentTime
		);
	
	/**
	 * @brief 構造体配列から球コンポーネントを生成
	 */
	void GeneratedSphereComponents();

	/**
	 * @brief BPで値が変更されたときやコンパイル時に自動で回る処理 
	 * @param Transform トランスフォーム
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	/**
	 * @brief 球の情報を持つ構造体配列
	 */
	UPROPERTY(EditAnywhere)
	TArray<FSphereElemental> SphereEntries;

	UPROPERTY()
	TArray<TObjectPtr<USphereComponent>> SphereComponents;

	/**
	 * @brief BP側で表示される番号ラベル
	 */
	UPROPERTY()
	TArray<TObjectPtr<UTextRenderComponent>> IndexLabels;

	/**
	 * @brief 発動するまでの攻撃の最大待ち時間
	 */
	UPROPERTY()
	float MaxActivationDelay = 0.f;

	/**
	 * @brief 発動してからの攻撃の最大有効時間
	 */
	UPROPERTY()
	float MaxDuration = 0.f;

	/**
	 * @brief 実行された瞬間の時間
	 */
	UPROPERTY()
	float JustExecuteTime = 0.f;
};
