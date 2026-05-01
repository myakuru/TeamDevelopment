#pragma once

/** 
* Stateが持つデータの基底クラス
* 新しいStateのデータを追加するときは
* 1.このクラスを継承してU○○StateData.hを作る
* 2.UPROPERTYでエディタから編集したい値を宣言する
* 3.EnemyDataAssetなどのStateDataMapにエディタからセットする
*/

#include "Engine/DataAsset.h"
#include "StateDataBase.generated.h"

/**
* Stateデータの基底クラス
* 各Stateの固有データはこれを継承して作る
* 
* PrimaryDataAssetを継承しているため
* ContentBorwserでアセットとして作成・管理できる
* 
* Abstract指定により、このクラス自体はエディタで直接操作できない
* （必ず派生クラスを作って使う）
*/
UCLASS(Abstract, BlueprintType, EditInlineNew,DefaultToInstanced)
class PROJECTNULL_API UStateDataBase : public UObject
{
	GENERATED_BODY()
};