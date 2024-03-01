#pragma once

namespace NULLEngine::Graphics
{
    class PixelShader final  // Final - means no inheritance allowed
    {
    public:
        void Initiliaze(const std::filesystem::path& filePath);
        void Terminate();

        void Bind();
    private:
        ID3D11PixelShader* mPixelShader = nullptr;
    };
}