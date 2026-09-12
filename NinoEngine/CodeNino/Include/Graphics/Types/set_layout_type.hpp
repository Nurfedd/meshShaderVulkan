#pragma once
#include <vector>
#include "Interface/set_layout.hpp"

namespace nino_engine {
    class GlobalSetLayout;
    

    class SetLayoutType {
    public:
        using ConstructFunc = GlobalSetLayout * (*)();

        SetLayoutType(
            std::vector<rhi::SetInfo> setInfos,
            ConstructFunc constructFunc)
            : setInfos(setInfos), constructFunc(constructFunc)
        {
            GetTypeList().push_back(this);
        }

        GlobalSetLayout* ConstructInstance() const { return constructFunc(); }
        const std::vector<rhi::SetInfo> GetSetInfos() const { return setInfos; }
        
        static std::vector<SetLayoutType*>& GetTypeList() {
            static std::vector<SetLayoutType*> types;
            return types;
        }

    private:
        std::vector<rhi::SetInfo> setInfos;
        ConstructFunc constructFunc;
    };
}