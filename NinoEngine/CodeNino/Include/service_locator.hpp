#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include "service.hpp"
#include <type_traits>
namespace nino_engine {
	
	class ServiceLocator {
	public :
		template <typename T>
		static T* GetOrRegister() {
			return Get<T>() || RegisterService<T>();
		}
		template <typename T>
		static T* RegisterService() {
			static_assert(std::is_base_of<Service, T>::value);
			std::type_index typeIndex = typeid(T);
			if (services.contains(typeIndex))
				return nullptr;
			std::unique_ptr<T> newService = std::make_unique<T>();
			T* rawPtr = newService.get();
			services[typeIndex] = std::move(newService);
			return rawPtr;
		}
		template <typename T>
		static T* Get() {
			std::type_index typeIndex = typeid(T);
			return dynamic_cast<T*>(services[typeIndex].get());
		}
		template <typename T>
		static bool Exists() {
			return Get<T>() != nullptr;
		}
		template <typename T>
		static void UnregisterService() {
			std::type_index typeIndex = typeid(T);
			T* service = Get<T>();
			if (service) {
				services.erase(typeIndex);
			}
		}
		static void Clear();
	private :
		static std::unordered_map<std::type_index, std::unique_ptr<Service>> services;
		
	};
}