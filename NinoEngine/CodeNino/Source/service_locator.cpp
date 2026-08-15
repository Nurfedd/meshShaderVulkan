#include "service_locator.hpp"
#include <map>
namespace nino_engine {
	std::unordered_map<std::type_index, std::unique_ptr<Service>> ServiceLocator::services;
	void ServiceLocator::Clear() {
		for (auto& pair : services) {
			std::unique_ptr<Service>& service = pair.second;
			service->OnPreDestroy();
		}
		for (auto& pair : services) {
			std::unique_ptr<Service>& service = pair.second;
			service->OnPostDestroy();
		}
		services.clear();// call destructor but prefer to use PostDestroy and PreDestroy
	}
}