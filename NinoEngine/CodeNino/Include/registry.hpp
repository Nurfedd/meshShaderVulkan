#pragma once
#include <vector>
#include <functional>

namespace nino_engine {
	template <typename T>
	class Registry {
	public :
		void Register(const T& entry) {
			registryVec.push_back(entry);
		}
		const std::vector<T>& GetEntries() {
			return registryVec;
		}
		void ForEachEntry(std::function<void(T&)> func) {
			for (T& entry : registryVec) {
				func(entry);
			}
		}
	private :
		std::vector<T> registryVec;
	};
}