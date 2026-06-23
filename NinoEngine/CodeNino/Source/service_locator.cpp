#include "service_locator.hpp"
#include <map>
namespace nino_engine {
	std::unordered_map<std::type_index, ServiceContainer> ServiceLocator::services;
	void ServiceLocator::Clear() {
		std::map<int, std::vector<Service*>> servicePriorities;

		for (auto& pair : services) {
			servicePriorities[pair.second.priority].push_back(pair.second.service);
		}
		for (auto& pair : servicePriorities) {
			for (Service* service : pair.second) {
				delete service;
			}
		}
	}
}