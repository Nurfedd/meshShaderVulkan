#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include "service.hpp"
#include <type_traits>
namespace nino_engine {
	struct ServiceContainer {
		Service* service = nullptr;
		int priority = 0;
	};
	class ServiceLocator {
	public :
		template <typename T, typename... Args>
		static T* RegisterService(int priority, Args&&... args) {
			static_assert(std::is_base_of<Service, T>::value);
			std::type_index typeIndex = typeid(T);
			if (services.contains(typeIndex))
				return nullptr;
			T* newService = new T(std::forward<Args>(args)...);

			ServiceContainer serviceContainer;
			serviceContainer.priority = priority;
			serviceContainer.service = newService;

			services[typeIndex] = serviceContainer;
			return newService;
		}
		template <typename T>
		static T* Get() {
			std::type_index typeIndex = typeid(T);
			if (services.contains(typeIndex))
				return dynamic_cast<T*>(services[typeIndex].service);
			return nullptr;
		}
		template <typename T>
		static void UnregisterService() {
			std::type_index typeIndex = typeid(T);
			T* service = Get<T>();
			if (service) {
				services.erase(typeIndex);
				delete service;
			}
		}
		static void Clear();
	private :
		static std::unordered_map<std::type_index, ServiceContainer> services;
		
	};
}