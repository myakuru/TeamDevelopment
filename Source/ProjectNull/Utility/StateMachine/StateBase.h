#pragma once

/** 
* 前方宣言（循環参照を避けるため）
* こうすることでStateMachine.hをincludeせずに
* TStateMachine<OwnerType>*という型だけを使える
*/
template<typename OwnerType>
class TStateMachine;

/**
* 全Stateの基底クラス
* OwnerType = このStateを持つキャラクターの型（例：AEnemyBase）
* テンプレートにすることで、どんな型のオーナーにも対応できる
* 使う側はTStateBase<AEnemyBase>のように型を指定して継承する
*/
template<typename OwnerType>
class TStateBase
{
	/**
	* TStateMachineをfriendに指定
	* こうすることで、privateにあるCallStart/CallUpdate/CallExitを
	* TStateMachineだけが呼べるようになる
	* 外部から直接呼ばれると困る処理を守るための仕組み
	*/
	friend class TStateMachine<OwnerType>;

protected:

	/**
	* Stateが始まった時に一度だけ呼ばれる
	* アニメーション切り替えや変数初期化を行う
	*/
	virtual void OnStart(OwnerType* Owner) {}

	/**
	* 毎フレーム呼ばれる
	* 移動などの毎フレーム行いたい処理を行う
	*/
	virtual void OnUpdate(OwnerType* Owner, float DeltaTime) {}
	
	/**
	* Stateが終わるときに一度だけ呼ばれる
	*/
	virtual void OnExit(OwnerType* Owner) {}

	/** 
	* 各Stateから自分のStateMachineにアクセスするためのポインタ
	* OnUpdate()内でMachine->ChangeState<次のState>()のように使う
	*/
	TStateMachine<OwnerType>* Machine = nullptr;

private:

	/** 
	* StateMachineが自分自身のポインタをStateに渡すための関数
	* friendクラス（TStateMachine）だけが呼べる
	*/
	void SetMachine(TStateMachine<OwnerType>* InMachine)
	{
		Machine = InMachine;
	}

	/** TStateMachineがOnStartを安全に呼ぶためのラッパー*/
	void CallStart(OwnerType* Owner)
	{
		if (!Machine || !Owner)return;
		OnStart(Owner);
	}

	void CallUpdate(OwnerType* Owner, float DeltaTime)
	{
		if (!Machine || !Owner)return;
		OnUpdate(Owner, DeltaTime);
	}

	void CallExit(OwnerType* Owner)
	{
		if (!Machine || !Owner)return;
		OnExit(Owner);
	}

};