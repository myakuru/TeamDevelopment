#pragma once

#include "EnemyISMManager.generated.h"

class AEnemyBase;
class UStaticMesh;
class UInstancedStaticMeshComponent;

UCLASS()
class PROJECTNULL_API AEnemyISMManager : public AActor
{
	GENERATED_BODY()

public:

	AEnemyISMManager();

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

protected:

	virtual void BeginPlay() override;

	/** 管理する敵の種類のStaticMesh*/
	UPROPERTY(EditAnywhere, Category = "Setup")
	TObjectPtr<UStaticMesh> EnemyMesh;

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

	/** FreeList（インデックス管理用）*/

	/** 削除で空いたインデックスを再利用するためのリスト
	* RemoveInstanceを呼ぶと後ろのインデックスがすべてずれて全Enemyのインデックス再マップが必要になるため
	* 削除するのではなく、スケール0で負荷しにしてFreeListに積む
	*/
	TArray<int32> FreeIndices;

	/** 現在のインスタンス総数（末尾追加の際のインデックスとして利用）*/
	int32 MaxInstanceCount = 0;

	/** 管理対象の数*/

	/** TSetにすることでRemoveを0(1)にする*/
	UPROPERTY()
	TSet<TObjectPtr<AEnemyBase>> Enemies;


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


};