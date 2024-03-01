#pragma once

namespace NULLEngine::Graphics
{
    class Sampler
    {
    public:
        enum class Filter { Point, Linear, Anisotropic };
        enum class AdressMode { Border, Clamp, Mirror, Wrap };

        Sampler() = default;
        ~Sampler();

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;

        void Initialize(Filter filter, AdressMode adressMode);
        void Terminate();

        void BindVS(uint32_t slot) const;
        void BindPS(uint32_t slot) const;

    private:
        ID3D11SamplerState* mSampler = nullptr;

    };
}