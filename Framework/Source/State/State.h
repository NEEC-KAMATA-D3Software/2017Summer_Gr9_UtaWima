#pragma once
#include<memory>
#include<unordered_map>

enum class Trans
{
	Trans_Again, //�������܂��Ă΂ꂽ
	Trans_Occured,//��ԑJ�ڂ��N����
};

template<typename T>
class State
{
public:

	/**
	* @brief					�R���X�g���N�^
	*/
	State()
		:m_IsFirst(true)
	{
	}

	/**
	* @brief					�f�X�g���N�^
	*/
	virtual~State() {
	}
	 

	/**
	* @brief					�X�V
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
	* @brief					�؂�ւ���ď��߂ɌĂ΂��R�[���o�b�N���\�b�h
	*/
	virtual void entryAction(T* outerValue) {}

	/**
	* @brief					�}�C�t���[���A�b�v�f�[�g�ŌĂ΂��R�[���o�b�N���\�b�h
	*/
	virtual Trans inputAction(T* outerValue, std::shared_ptr<State<T>>* nextState) { return Trans::Trans_Again; }
	
	/**
	* @brief					���̃X�e�[�g�ɐ؂�ւ�钼�O�ɌĂ΂��R�[���o�b�N���\�b�h
	*/
	virtual void exitAction(T* outerValue) { m_IsFirst = true; }

private:
	//!�͂��߂��ǂ���
	bool m_IsFirst;
};
