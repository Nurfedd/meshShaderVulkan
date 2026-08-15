#pragma once
namespace nino_engine {
	class Service {
	public :
		virtual ~Service() = default;
		virtual void OnPreDestroy() {};
		virtual void OnPostDestroy() {};
	};
}