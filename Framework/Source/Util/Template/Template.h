#pragma once
#include<functional>
#include<list>
#include<vector>
#include<algorithm>


namespace util {

	/**
	* @brief			vector��copyif���b�v
	*/
	template<typename T>
	std::vector<T> vectorCopyif(std::vector<T>& vec, std::function<bool(T obj)> action) {

		std::vector<T> copyList;

		std::copy_if(vec.begin(), vec.end(), std::back_inserter(copyList),
			[&](T obj) {
			return action(obj);
		});

		return copyList;
	}

	/**
	* @brief			 list �Ɋ܂܂�Ă��邩�ǂ�������
	* @param list		 �Ώۂ̃��X�g
	* @param value		 ��r����l	 
	*/
	template<typename T>
	bool isContains(const std::list<T>& list, std::function<bool(T obj)> action) {
		auto find = std::find_if(list.begin(), list.end(), action);
		return find != list.end();
	}
	
}
