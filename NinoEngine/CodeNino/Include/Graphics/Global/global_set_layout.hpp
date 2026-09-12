#pragma once
#include "Graphics/Types/set_layout_type.hpp"
#include "Interface/device.hpp"
#include "Interface/set_layout.hpp"


namespace nino_engine {

    
    class GlobalSetLayout {
    public:
        
        virtual ~GlobalSetLayout() = default;

        GlobalSetLayout(std::vector<rhi::SetInfo> setInfos);

        rhi::SetLayout* GetLayout() const { return setLayout; }
        void Create(rhi::Device* Device);
        void Destroy(rhi::Device* Device);
    private:
        std::vector<rhi::SetInfo> setInfos;
        rhi::SetLayout* setLayout = nullptr;
    };
}

#define BEGIN_SET_LAYOUT(SetLayoutClass) \
public: \
    SetLayoutClass() : nino_engine::GlobalSetLayout(StaticType.GetSetInfos()) {} \
    static nino_engine::GlobalSetLayout* ConstructInstance() { return new SetLayoutClass(); } \
    static nino_engine::SetLayoutType StaticType;

#define SET_INFO(Binding,SetDescriptorType,DescriptorCount,ShaderStages,IsVariable) \
   rhi::SetInfo(Binding,DescriptorCount,ShaderStages,SetDescriptorType,IsVariable)

#define IMPLEMENT_SET_LAYOUT(SetLayoutClass,...)\
    nino_engine::SetLayoutType SetLayoutClass::StaticType( \
        std::vector<rhi::SetInfo>{__VA_ARGS__}, \
        &SetLayoutClass::ConstructInstance\
    );\
