#pragma once

#include "Graphics/Types/shader_type.hpp"
#include <filesystem>
#include <string>

namespace rhi {
    class Device;
    class Shader;
}

namespace nino_engine {

    class GlobalShader {
    public:
        virtual ~GlobalShader() = default;

        GlobalShader(const std::filesystem::path& filePath, const std::string& entryPoint);

        void Create(rhi::Device* Device);
        void Destroy(rhi::Device* Device);

    protected:
        std::string entryPoint;
        std::filesystem::path filePath;

    private:
        rhi::Shader* shaderModule = nullptr;
    };

}


#define DECLARE_SHADER(ShaderClass) \
public: \
    ShaderClass() : nino_engine::GlobalShader(StaticType.GetFilePath(), StaticType.GetEntryPoint()) {} \
    static nino_engine::GlobalShader* ConstructInstance() { return new ShaderClass(); } \
    static nino_engine::ShaderType StaticType;

#define IMPLEMENT_SHADER(ShaderClass, FilePath, EntryPoint) \
    nino_engine::ShaderType ShaderClass::StaticType( \
        #ShaderClass, FilePath, EntryPoint, &ShaderClass::ConstructInstance);