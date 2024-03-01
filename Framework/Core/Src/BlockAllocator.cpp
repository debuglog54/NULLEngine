#include "Precompiled.h"
#include "BlockAllocator.h"
#include "DebugUtil.h"

using namespace NULLEngine;
using namespace NULLEngine::Core;

BlockAllocator::BlockAllocator(const char* name, size_t blockSize, size_t capacity)
    : mName(name)
    , mBlockSize(blockSize)
    , mCapacity(capacity)
    , mFreeBlocks(capacity)
{
    ASSERT(blockSize > 0, "BlockAllocator: ivalid block size.");
    ASSERT(capacity > 0, "BlockAllocator: ivalid capacity.");

    mData = std::malloc(blockSize * capacity);

    for (size_t i = 0; i < capacity; ++i)
    {
        mFreeBlocks[i] = static_cast<uint8_t*>(mData) + (i * mBlockSize);
    }

    LOG("%s allocated %zu blocks, blocs size: %zu", mName.c_str(), capacity, blockSize);
}

BlockAllocator::~BlockAllocator()
{
    ASSERT(mFreeBlocks.size() == mCapacity, "BlockAllocator: not all blocks are freed, potential memory leak.");
    ASSERT(mBlocksAllocatedTotal == mBlocksFreed, "BlockAllocator: not all blocks are freed, potential memory leak.");
    std::free(mData);

    LOG("%s destructed: %zu, Freed: %zu, HighestCount: %zu", mName.c_str(), mBlocksAllocatedCurrent, mBlocksFreed, mBlocksHighest);
}

void* BlockAllocator::Allocate()
{
    if (mFreeBlocks.empty())
    {
        LOG("%s no free blocks available", mName.c_str());
        return nullptr;
    }

    void* freeBlock = mFreeBlocks.back();
    mFreeBlocks.pop_back();

    ++mBlocksAllocatedTotal;
    ++mBlocksAllocatedCurrent;
    mBlocksHighest = std::max(mBlocksHighest, mBlocksAllocatedCurrent);

    LOG("%s allocated blocks at %p, Allocated: %zu, HighestCount: %zu",
        mName.c_str(),
        freeBlock,
        mBlocksAllocatedCurrent,
        mBlocksHighest);

    return freeBlock;
}

void BlockAllocator::Free(void* ptr)
{
    if (ptr == nullptr)
    {
        return;
    }

    const auto start = static_cast<uint8_t*>(mData);
    const auto end = static_cast<uint8_t*>(mData) + (mBlockSize * mCapacity);
    const auto current = static_cast<uint8_t*>(ptr);
    const auto diff = current - start;
    ASSERT(current >= start && current < end&& static_cast<size_t>(diff) % mBlockSize == 0,
        "BlockAllocator: invalid adress being freed!");

    LOG("%s free %p", mName.c_str(), ptr);
    --mBlocksAllocatedCurrent;
    ++mBlocksFreed;
    mFreeBlocks.emplace_back(ptr);
}