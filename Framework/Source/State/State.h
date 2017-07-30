#pragma once
#include<memory>
#include<unordered_map>

enum class Trans
{
	Trans_Again, //自分がまた呼ばれた
	Trans_Occured,//状態遷移が起きた
};

template<typename T>
class State
{
public:

	/**
	* @brief					コンストラクタ
	*/
	State()
		:m_IsFirst(true)
	{
	}

	/**
	* @brief					デストラクタ
	*/
	virtual~State() {
	}
	 

	/**
	* @brief					更新
	*/
	virtual std::shared_ptr<State<T>> update(T* outerValue) {

		if (m_IsFirst) {
			entryAction(outerValue);
			m_IsFirst = false;
		}
		std::shared_ptr<State<T>> outerState;
		Trans trans = inputAction(outerValue,&outerState);
		if (trans == Trans::Trans_Occured)
			exitAction(outerValue);

		return outerState;
	}

protected:

	/**
	* @brief					切り替わって初めに呼ばれるコールバックメソッド
	*/
	virtual void entryAction(T* outerValue) {}

	/**
	* @brief					マイフレームアップデートで呼ばれるコールバックメソッド
	*/
	virtual Trans inputAction(T* outerValue, std::shared_ptr<State<T>>* nextState) { return Trans::Trans_Again; }
	
	/**
	* @brief					次のステートに切り替わる直前に呼ばれるコールバックメソッド
	*/
	virtual void exitAction(T* outerValue) { m_IsFirst = true; }

private:
	//!はじめかどうか
	bool m_IsFirst;
};
