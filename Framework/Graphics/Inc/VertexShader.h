#pragma once

namespace NULLEngine::Graphics
{
    class VertexShader final  // Final - means no inheritance allowed
    {
    public:
        template <class VertexType >
        void Initiliaze(const std::filesystem::path& filePath)
        {
            Initiliaze(filePath, VertexType::Format);
        }
        void Initiliaze(const std::filesystem::path& filePath, uint32_t format);
        void Terminate();

        void Bind();
    private:
        ID3D11VertexShader* mVertexShader = nullptr;
        ID3D11InputLayout* mInputLayout = nullptr;
    };
}