#pragma once

#include "StateBase.h"

/**
* Stateを管理するクラス
* OwnerType = このStateMachineを持つキャラクターの型
* 【スタック構造について】
* Stateをリスト（スタック）で管理する
* リストの一番後ろが「今のState」
* 
* ChangeState：今のStateを破棄して新しいStateに切り替え
* PushState    ：今のStateを残したまま新しいStateを追加（前のStateに戻れる）
* PopState      ：今のStateを破棄して1つ前のStateに戻る
*/

template<typename OwnerType>
class TStateMachine
{
public:

	TStateMachine()
		: Owner(nullptr)
		, PendingChange([]() {})	// 最初は何もしない空の命令で初期化
	{}

	/** 
	* StateMachineの初期化
	* キャラクターのBeginPlay等で呼ぶ
	* InOwner このStateMachineを持つキャラクター
	*/
	void Start(OwnerType* InOwner)
	{
		Owner = InOwner;
		PendingChange = []() {};
		StateStack.Empty();
	}

	/** 
	* Stateを切り替える（履歴を残さない）
	* 今のStateを終了して、新しいStateを始める
	* 使い方：Machine->ChangeState<FEnemyStateChase>();
	*				Machine->ChangeState<FEnemyStateAttack>(10.0f);		// 引き数ありのコンストラクタにも渡せる
	*/
	template<typename StateType, typename... ArgType>
	void ChangeState(ArgType... Args)
	{
		StateChanger<StateType>(/*bReplace*/true, Args...);
	}

	/** 
	* Stateを積む（履歴を残す・前のStateに戻れる）
	* 今のStateを一時停止して、新しいStateを始めるイメージ
	* PopState()を呼ぶと１つ前のStateに戻れる
	* 使うかはわからない
	*/
	template<typename StateType, typename... ArgType>
	void PushState(ArgType... Args)
	{
		StateChanger<StateType>(/*bReplace*/false, Args...);
	}

	/** 
	* 今のStateを終了して、１つ前のStateに戻る
	* PushStateと組み合わせて使うかもしれない
	*/
	void PopState()
	{
		PendingChange = [this]()
			{
				TSharedPtr<TStateBase<OwnerType>> Current = GetCurrentState();
				if (!Current)return;

				Current->CallExit(Owner);
				StateStack.Pop();
				// スタックに残っているStateがあれば、それが新しい今のStateになる
			};
	}

	/** 
	* 全Stateを終了してスタックを空にする
	* 敵が死んだときやPoolに返却するときに呼ぶ
	*/
	void ClearState()
	{
		PendingChange = [this]()
			{
				TSharedPtr<TStateBase<OwnerType>> Current = GetCurrentState();
				if (Current)Current->CallExit(Owner);
				StateStack.Empty();
			};
	}

	/** 毎フレーム呼ぶ*/
	void Update(float DeltaTime)
	{
		/**
		*保留中のState変更をUpdateの最初に実行
		* OnUpdateの中でChangeState()を呼んだ時
		* 即座にStateを切り替えると、処理中のStateのメモリが解放される状態になるため
		* 変更命令を一旦保存して、次のフレームの最初に実行することで防ぐ
		*/
		PendingChange();
		PendingChange = []() {};

		TSharedPtr<TStateBase<OwnerType>> Current = GetCurrentState();
		if (Current)
		{
			Current->CallUpdate(Owner, DeltaTime);
		}
	}

	/** スタックが空かどうかを返す*/
	bool IsEmpty() const { return StateStack.IsEmpty(); }

	/** 今のStateを返す、デバッグや状態確認に使う*/
	TSharedPtr<TStateBase<OwnerType>> GetCurrentState() const
	{
		if (StateStack.IsEmpty())return nullptr;
		return StateStack.Last();
	}

private:

	/**
	* ChangeStateとPushState共通の内部処理
	* bReplaceが
	* trueなら今のStateをスタックから消す（ChangeState）
	* falseなら残したまま積む（PushState）
	*/
	template<typename StateType, typename... ArgType>
	void StateChanger(bool bReplace, ArgType... Args)
	{
		/** 変更命令をラムダに閉じ込めて保存する*/
		PendingChange = [this, bReplace, Args...]()
			{
				if (!Owner)return;

				/** 現在のStateを終了*/
				TSharedPtr<TStateBase<OwnerType>> Current = GetCurrentState();
				if (Current)
				{
					Current->CallExit(Owner);
					if (bReplace)
					{
						/** ChangeStateなら履歴も消す*/
						StateStack.Pop();
					}
					/** PushStateなら取り除かず残す*/
				}

				/** 新しいStateを作成してスタックに積む*/
				TSharedPtr<StateType> NewState = MakeShared<StateType>(Args...);
				NewState->SetMachine(this);		/** StateMachineのポインタをStateに渡す*/
				StateStack.Add(NewState);
				NewState->CallStart(Owner);
			};
	}

	/** このStateMachineを持つキャラクター*/
	OwnerType* Owner = nullptr;

	/** Stateのスタック（末尾が現在のState）*/
	TArray<TSharedPtr<TStateBase<OwnerType>>> StateStack;

	/** 保留中のState変更命令*/
	TFunction<void()> PendingChange;
};
