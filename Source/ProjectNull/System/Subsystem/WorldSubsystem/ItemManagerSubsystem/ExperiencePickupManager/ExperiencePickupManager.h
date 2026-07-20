#pragma once

#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Data/ExperiencePickupData.h"

/**
 * 経験値オーブの論理管理とNiagara描画を担うマネージャー
 * 
 * 1.所有Actor側でNiagaraComponentを生成し、Initialize()に渡す
 * 2.毎TickでUpdate()を呼ぶ
 * 3.敵撃破時などにSpawnExperience()で生成
 * 4.Update()後にConsumeCollectedExp()取得済み経験値を受け取る
 * 
 * ナイアガラに必要なパラメーター
 * Position  : Array<Vector>        パーティクル位置
 * Color     : Array<LinearColor> パーティクル色
 * Sizes     : Array<Float>          パーティクルサイズ倍率
 */

class UNiagaraComponent;

class FExperiencePickupManager
{
public:

	/** NiagaraComponentをセットして使用開始*/
	void Initialize(UWorld* World);

	/**
	* プレイヤーへの吸引・取得判定
	* Niagaraへのデータ同期
	 */
	void Update(APawn* Player, float DeltaTime);

	/**
	 * 経験値オーブをスポーン
	 * @param Location		スポーン座標
	 * @param ExpValue		取得時に加算される経験値
	 * @param Color			Niagara描画色（レアリティ表現か経験値量表現）
	 * @param Size				Niagaraパーティクルのサイズ倍率
	 */
	void SpawnExperience(
		const FVector&			Location,
		float					ExpValue,
		const FLinearColor&	Color = FLinearColor::Green,
		float					Size = 1.0f
	);

	/** 全オーブを削除してNiagaraをクリア*/
	void Clear();

	/**
	* 今フレームまでに取得した経験値の合計を返し、内部カウンタをリセット
	* Update()の後に毎フレーム呼びだして経験値を消費する
	*/
	float ConsumeCollectedExp();

	/** 現在アクティブなオーブ数*/
	int32 GetActiveCount() const { return ExperienceList.Num(); }


	/** BPのClassDefaultsで編集する設定値*/

	/** 経験値オーブのNiagaraSystem */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Niagara")
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

	/** オーブのデフォルト設定値 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	FExperiencePickupSettings DefaultSettings;

private:

	/** ExperienceListの内容をNiagaraの配列データへ書き込む*/
	void SyncToNiagara();

	/** 楕円軌道で追尾*/
	FVector CalculateOrbit(const FVector& StartPos, const FVector& EndPos,float ElapsedTime, float BulgeWidth, float BulgeHeight);

	/** アクティブなオーブのデータ一覧*/
	TArray<FExperiencePickupData> ExperienceList;
	/** 取得済みだがまだ消費されていない経験値の合計*/
	float PendingExpValue = 0.0f;

	/**
	* 描画を担うNiagaraComponent（所有側はActor側）
	*/
	TWeakObjectPtr<AActor>			  NiagaraOwnerActor;
	TWeakObjectPtr<UNiagaraComponent> NiagaraComponent;

	TWeakObjectPtr<UWorld> spWorld;
};