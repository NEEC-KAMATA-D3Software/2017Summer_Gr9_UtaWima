#pragma once

#include<unordered_map>
#include<functional>

namespace util {
	
	template<typename T, typename ...Param>
	class Event
	{
	public:
		Event() {

		}
		virtual ~Event() {

		}

		void addEvent(const T& key, std::function<void(Param...)> func) {
			m_Funcs[key].emplace_back(func);
		}

		void onEvent(const T& key, Param...) {
			for (auto& func : m_Funcs[key]) {
				func(Param...);
			}
		}

		void clear() {
			m_Funcs.clear();
		}

	private:
		using FuncContainer = std::unordered_map<T, std::vector<std::function<void(Param...)>>>;
		FuncContainer m_Funcs;
	};
}