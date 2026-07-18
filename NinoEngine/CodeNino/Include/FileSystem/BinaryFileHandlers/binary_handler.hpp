#pragma once
#include <filesystem>
#include <fstream>
/*
* binary reader class uses to read binary from files
* should calls adds your own Read and Write functions with own parameters.
* Use OpenRead and OpenWrite helpers
*/
namespace nino_engine {
	class BinaryFileHandler {
	public :
        virtual ~BinaryFileHandler() = default;
	protected :
        bool OpenRead(const std::filesystem::path& path);
        bool OpenWrite(const std::filesystem::path& path,bool clearContent = true);
        template<typename T>
        void Write(const T& value)
        {
            static_assert(std::is_trivially_copyable_v<T>);

            stream.write(
                reinterpret_cast<const char*>(&value),
                sizeof(T));
        }

        template<typename T>
        void Read(T& value)
        {
            static_assert(std::is_trivially_copyable_v<T>);

            stream.read(
                reinterpret_cast<char*>(&value),
                sizeof(T));
        }

        template<typename T>
        void WriteVector(const std::vector<T>& vector)
        {
            static_assert(std::is_trivially_copyable_v<T>);

            uint64_t size = vector.size();
            if (size > 0)
            {
                stream.write(
                    reinterpret_cast<const char*>(vector.data()),
                    sizeof(T) * size);
            }
        }

        template<typename T>
        void ReadVector(std::vector<T>& vector,uint64_t size)
        {
            static_assert(std::is_trivially_copyable_v<T>);
            vector.resize(size);

            if (size > 0)
            {
                stream.read(
                    reinterpret_cast<char*>(vector.data()),
                    sizeof(T) * size);
            }
        }
    private:

        std::fstream stream;
	};
}