#include "Precompiled.h"
#include "TextureManager.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

namespace
{
    std::unique_ptr<TextureManager> sInstance;
}

void TextureManager::StaticInitialize(const std::filesystem::path& root)
{
    ASSERT((sInstance == nullptr), "TextureManager is aldready initiliazed!");
    sInstance = std::make_unique<TextureManager>();
    sInstance->SetRootDirectory(root);
}

void TextureManager::StaticTerminate()
{
    sInstance.reset();
}

 TextureManager* TextureManager::Get()
{
    ASSERT((sInstance != nullptr), "TextureManager is not initiliazed yet!");
    return sInstance.get();
}

TextureManager::~TextureManager()
{
    for (auto& [id, texture] : mInventory)
    {
        texture->Terminate();
    }
    mInventory.clear();
}

TextureId TextureManager::LoadTexture(const std::filesystem::path& fileName, bool useRootDir)
{
    auto textureId = std::filesystem::hash_value(fileName);
    auto [iter, success] = mInventory.insert({ textureId, nullptr });
    if (success)
    {
        auto& texturePtr = iter->second;
        texturePtr = std::make_unique<Texture>();
        texturePtr->Initialize((useRootDir) ? mRootDirectory / fileName : fileName);
    }

    return textureId;
}

void TextureManager::BindVS(TextureId id, uint32_t slot) const
{
    if (id == 0)
    {
        return;
    }

    auto iter = mInventory.find(id);
    if (iter != mInventory.end())
    {
        iter->second->BindVS(slot);
    }
}

void TextureManager::BindPS(TextureId id, uint32_t slot) const
{
    if (id == 0)
    {
        return;
    }

    auto iter = mInventory.find(id);
    if (iter != mInventory.end())
    {
        iter->second->BindPS(slot);
    }
}


