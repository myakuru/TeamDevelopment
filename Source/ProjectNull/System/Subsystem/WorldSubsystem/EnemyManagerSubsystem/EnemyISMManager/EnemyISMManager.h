#pragma once

#include "AnimUpdateShader.h"
#include "EnemyISMManager.generated.h"

class AEnemyBase;
class UStaticMesh;
class UInstancedStaticMeshComponent;
class UEnemyAnimDataAsset;
class UTextureRenderTarget2D;
class UTexture2D;

struct FAnimData;

UCLASS()
class PROJECTNULL_API AEnemyISMManager : public AActor
{
	GENERATED_BODY()

public:

	AEnemyISMManager();

	void DebugReadbackAnimStateRT();

	void UpdateEnemies(float DeltaTime);
	// ------------------------------------------------------------------
	// 敵の登録・解除
	// ------------------------------------------------------------------

	// 敵をISMに登録してインスタンスインデックスを割り当てる
	// BeginPlayのタイミングでEnemyから呼ばれる
	void RegisterEnemy(AEnemyBase* Enemy);

	// 敵をISMから解除してインデックスを解放する
	// EndPlayのタイミングでEnemyから呼ばれる
	void UnregisterEnemy(AEnemyBase* Enemy);

	/**
	 * @brief アニメ変更リクエストをキューに積む
	 * @param InstanceIndex  対象インスタンスのインデックス
	 * @param NextAnimIndex  次のアニメのインデックス
	 * @param bLooping       ループするか
	 * @param BlendSpeed     ブレンド速度
	 */
	void RequestAnimChange(int32 InstanceIndex, int32 NextAnimIndex, bool bLooping, float BlendSpeed = 0.3f);

	TObjectPtr<UEnemyAnimDataAsset> GetAnimDataAsset() { return AnimDataAsset; }

protected:

	virtual void BeginPlay() override;

	/** 管理する敵の種類のStaticMesh*/
	UPROPERTY(EditAnywhere, Category = "Setup")
	TObjectPtr<UStaticMesh> EnemyMesh;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TObjectPtr<UEnemyAnimDataAsset> AnimDataAsset;

	/** 最大インスタンス数（テクスチャサイズに影響する） */
	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 MaxInstances = 1024;

	/** 
	* AnimToTextureのCustomDataチャンネル数
	* マテリアル側のNumCustomDataFloatsと合わせる必要がある
	*/
	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 NumCustomDataFloats = 2;

private:

	/**
	* コンポーネント
	*/

	/** ISMComponentを保持するためのActor*/

	/** 同一メッシュのドローコールをまとめるISMコンポーネント*/
	UPROPERTY(VisibleAnywhere, Category = "ISM")
	TObjectPtr<UInstancedStaticMeshComponent> ISM;

	//----------------------------------------------------------
	// アニメーション状態テクスチャ（CS↔Materialの橋渡し）
	//
	// Width  = MaxInstances
	// Height = 2行
	// Row 0  : float4(AnimTime, PrevAnimTime, NextAnimTime, BlendWeight)
	// Row 1  : float4(AnimIndex, NextAnimIndex, Flags, BlendSpeed)
	//----------------------------------------------------------
	UPROPERTY()
	TObjectPtr<UTextureRenderTarget2D> AnimStateRT;

	//----------------------------------------------------------
	// アニメ情報テクスチャ（AnimDataAssetから作成、静的）
	//
	// Width  = アニメーション数
	// Height = 1行
	// 各テクセル : float4(StartFrame, NumFrames, 0, 0)
	//----------------------------------------------------------
	UPROPERTY()
	TObjectPtr<UTexture2D> AnimInfoTexture;

	/** FreeList（インデックス管理用）*/

	/** 削除で空いたインデックスを再利用するためのリスト
	* RemoveInstanceを呼ぶと後ろのインデックスがすべてずれて全Enemyのインデックス再マップが必要になるため
	* 削除するのではなく、スケール0で負荷しにしてFreeListに積む
	*/
	TArray<int32> FreeIndices;

	/** CS内の更新先をこのIndexの数値のものを行うためのリスト*/
	UPROPERTY()
	TArray<uint32> ActiveIndices;

	/** 現在のインスタンス総数（末尾追加の際のインデックスとして利用）*/
	int32 MaxInstanceCount = 0;

	/** 管理対象の数*/

	/** TSetにすることでRemoveを0(1)にする*/
	UPROPERTY()
	TSet<TObjectPtr<AEnemyBase>> Enemies;

	//----------------------------------------------------------
	// アニメ変更リクエスト（CPUが積んでGPUが消費する）
	//----------------------------------------------------------
	TArray<FGPUAnimChangeRequest> PendingChangeRequests;

	/**
	* CSMainでのアニメーション更新でのみ使うデータ
	*/
	TRefCountPtr<FRDGPooledBuffer> AnimStatePoolBuffer;

	//----------------------------------------------------------
	// 内部処理
	//----------------------------------------------------------
	void InitAnimStateTexture();
	void InitAnimInfoTexture();
	void InitAnimBuffer();
	void DispatchAnimUpdate(float DeltaTime);

	/** 内部処理*/
	
	/** 空きインデックスを返す
	* FreeListに空きがあればそこから返し、なければ末尾に追加して返す
	*/
	int AllocateIndex();

	/** インデックスをFreeListに積んで解放する
	*	RemoveInstanceを呼ぶと後ろのインデックスがすべてずれるため
	*	削除するのではなく、スケール0で負荷しにしてFreeListに積む
	*/
	void ReleaseIndex(int32 Index);

	int32 ActiveCount = 0;


};