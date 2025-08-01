#include "Precompiled.h"
#include "ModelCache.h"

#include "ModelIO.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

namespace
{
	std::unique_ptr<ModelCache> sModelCache;
}

void ModelCache::StaticInitialize()
{
	ASSERT(sModelCache == nullptr, "ModelCache: is already initialized");
	sModelCache = std::make_unique<ModelCache>();
}

void ModelCache::StaticTerminate()
{
	sModelCache.reset();
}

ModelCache* ModelCache::Get()
{
	ASSERT(sModelCache != nullptr, "ModelCache: was not initialized");
	return sModelCache.get();
}

ModelId ModelCache::GetModelId(const std::filesystem::path& filePath)
{
	return std::filesystem::hash_value(filePath);
}

ModelId ModelCache::LoadModel(const std::filesystem::path& filePath)
{
	const ModelId modelId = GetModelId(filePath);
	auto [iter, success] = mInventory.insert({ modelId, nullptr });
	if (success)
	{
		auto& modelPtr = iter->second;
		modelPtr = std::make_unique<Model>();
		ModelIO::LoadModel(filePath, *modelPtr);
		ModelIO::LoadMaterial(filePath, *modelPtr);
		ModelIO::LoadSkeleton(filePath, *modelPtr);
	}
	return modelId;
}

const Model* ModelCache::GetModel(ModelId id)
{
	auto model = mInventory.find(id);
	if (model != mInventory.end())
	{
		return model->second.get();
	}
	return nullptr;
}

void ModelCache::AddAnimation(ModelId id, const std::filesystem::path& filePath)
{
	auto model = mInventory.find(id);
	ASSERT(model != mInventory.end(), "ModelCache: need to load the model before add it");
	ModelIO::LoadAnimation(filePath, *model->second);
}
