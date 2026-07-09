#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "SphereCollision.generated.h"

/** 前方宣言 */
class UTextRenderComponent;

/**
 * @brief 攻撃に使用する球のパラメータ群
 */
USTRUCT()
struct FSphereElemental
{
	GENERATED_BODY()

public:
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
	float Duration = 0.f;

	/**
	 * @brief トランスフォーム系
	 */
	UPROPERTY(EditAnywhere)
	FTransform Transform;
	
	/**
	 * @brief 球コンポーネント
	 */
	UPROPERTY()
	TObjectPtr<USphereComponent> SphereComponent;
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
	 * @brief オーバーラップを検知した時の処理をセット
	 * @tparam TOwner オーナークラス名
	 * @param Owner オーナークラス
	 * @param BeginFunc オーバーラップした瞬間に回す処理 
	 */
	template<typename TOwner >
	void BindSphereBeginOverlapEvents(
			TOwner* Owner
		,	void (TOwner::*BeginFunc)(UPrimitiveComponent*,AActor*,UPrimitiveComponent*,int32,bool,const FHitResult&)
		)
	{
		for (auto& Entry : SphereEntries)
		{
			if (auto Sphere=Entry.SphereComponent)
			{
				Sphere->OnComponentBeginOverlap.AddDynamic(Owner,BeginFunc);
			}
		}
	}
	
	/**
	 * @brief オーバーラップを検知した時の処理をセット
	 * @tparam TOwner オーナークラス名
	 * @param Owner オーナークラス
	 * @param EndFunc オーバーラップを抜けた瞬間に回す処理
	 */
	template<typename TOwner >
	void BindSphereEndOverlapEvents(
			TOwner* Owner
		,	void (TOwner::*EndFunc)(UPrimitiveComponent*,AActor*,UPrimitiveComponent*,int32)
		)
	{
		for (auto& Entry : SphereEntries)
		{
			if (auto Sphere=Entry.SphereComponent)
			{
				Sphere->OnComponentEndOverlap.AddDynamic(Owner,EndFunc);	
			}
		}
	}
	
	
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
		const ECollisionChannel		InCollisionChannel,
		const ECollisionResponse	InCollisionResponse);

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
	float GetBaseAttackDuration()	const { return MaxDuration + MaxActivationDelay; }
	
	/**	発動してからの最大有効時間を取得 */
	float GetMaxDuration()			const { return MaxDuration; }

	/**	発動するまでの最大待ち時間を取得 */
	float GetMaxActivationDelay()	const { return MaxActivationDelay; }
	/* End Getters~ */

private:
	
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
