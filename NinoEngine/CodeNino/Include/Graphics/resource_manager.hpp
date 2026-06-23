#pragma once
#include "Graphics/resource.hpp"
#include "service_locator.hpp"
#include <vector>
#include <memory>

namespace nino_engine {
	class ResourceManager : public Service {
	public :
		~ResourceManager() override;
		// TODO add Create that runs in parallel
		template <typename T, typename... Args>
		T* Create(Args&&... args) {
			static_assert(std::is_base_of<IResource, T>::value);
			std::unique_ptr<T> newResource = std::make_unique<T>();
			T* rawPtr = newResource.get();
			newResource->Create(std::forward<Args>(args)...);

			resources.push_back(std::move(newResource));
			return rawPtr;
		}
		template <typename T>
		T* Add(T object) {
			static_assert(std::is_base_of<IResource, T>::value);
			std::unique_ptr<T> newResource = std::make_unique<T>(object);
			T* rawPtr = newResource.get();
			

			resources.push_back(std::move(newResource));
			return rawPtr;
		}
		
	private :
		std::vector<std::unique_ptr<IResource>> resources;
	};
}