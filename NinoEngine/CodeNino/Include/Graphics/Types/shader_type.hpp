#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "Interface/device.hpp"

namespace nino_engine {

    class GlobalShader;
    class ShaderType {
    public:
        using ConstructFunc = GlobalShader * (*)();

        ShaderType(const char* name,
            std::filesystem::path filePath,
            std::string entryPoint,
            ConstructFunc constructFunc)
            : name(name)
            , filePath(std::move(filePath))
            , entryPoint(std::move(entryPoint))
            , constructFunc(constructFunc)
        {
            
            GetTypeList().push_back(this);
        }

        GlobalShader* ConstructInstance() const { return constructFunc(); }

        const std::filesystem::path& GetFilePath()  const { return filePath; }
        const std::string& GetEntryPoint() const { return entryPoint; }
        const char* GetName()       const { return name; }

        static std::vector<ShaderType*>& GetTypeList() {
            static std::vector<ShaderType*> types;
            return types;
        }

    private:
        const char* name;
        std::filesystem::path   filePath;
        std::string             entryPoint;
        ConstructFunc           constructFunc;
    };
}