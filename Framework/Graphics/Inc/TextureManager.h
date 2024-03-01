#pragma once
#include "Texture.h"

namespace NULLEngine::Graphics
{
    using TextureId = std::size_t;

    class TextureManager
    {
    public:
        static void StaticInitialize(const std::filesystem::path& root);
        static void StaticTerminate();
        static TextureManager* Get();

    public:
        TextureManager() = default;
        ~TextureManager();

        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        TextureManager(const TextureManager&&) = delete;
        TextureManager& operator=(const TextureManager&&) = delete;

        void SetRootDirectory(std::filesystem::path root) { mRootDirectory = std::move(root); };
        TextureId LoadTexture(const std::filesystem::path& fileName, bool useRootDir = true);
        void BindVS(TextureId id, uint32_t slot) const;
        void BindPS(TextureId id, uint32_t slot) const;

    private:
        using Inventory = std::unordered_map < TextureId, std::unique_ptr<Texture> >;
        Inventory mInventory;

        std::filesystem::path mRootDirectory;
    };
}