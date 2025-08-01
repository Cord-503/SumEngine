#include "Precompiled.h"
#include "AnimationUtil.h"
#include "Color.h"
#include "SimpleDraw.h"

using namespace SumEngine::Graphics;
using namespace SumEngine;

namespace
{
	void ComputeBoneTransformsRecursive(const Bone* bone, AnimationUtil::BoneTransforms& boneTransforms)
	{
		if (bone != nullptr)
		{
			boneTransforms[bone->index] = bone->toParentTransform;
		}
		if (bone->parent != nullptr)
		{
			boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
		}
		for (const Bone* child : bone->children)
		{
			ComputeBoneTransformsRecursive(child, boneTransforms);
		}
	}
}
void AnimationUtil::ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms)
{
	const Model* model = ModelCache::Get()->GetModel(modelId);
	if (model->skeleton != nullptr)
	{
		boneTransforms.resize(model->skeleton->bones.size(), Math::Matrix4::Identity);

		ComputeBoneTransformsRecursive(model->skeleton->root, boneTransforms);
	}
}

void AnimationUtil::DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms)
{
	const Model* model = ModelCache::Get()->GetModel(modelId);
	if (model->skeleton != nullptr)
	{
		for (const auto& bone : model->skeleton->bones)
		{
			if (bone->parent != nullptr)
			{
				const Math::Vector3 bonePos = Math::GetTranslation(boneTransforms[bone->index]);
				const Math::Vector3 boneParentPos = Math::GetTranslation(boneTransforms[bone->parentIndex]);
				SimpleDraw::AddLine(bonePos, boneParentPos, Colors::AliceBlue);
				SimpleDraw::AddSphere(10, 10, 0.03f, bonePos, Colors::Green);
			}
		}
	}
}
