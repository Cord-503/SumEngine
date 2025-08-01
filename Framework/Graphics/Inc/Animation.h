#pragma once
#include "Common.h"
#include "Keyframe.h"
#include "Transform.h"

namespace SumEngine::Graphics
{
	class Animation
	{
	public:
		Transform GetTransform(float time) const;
		float GetDuration() const;

	private:
		Math::Vector3 GetPosition(float time) const;
		Math::Quaternion GetRotation(float time) const;
		Math::Vector3 GetScale(float time) const;

		friend class AnimationBuilder;
		friend class AnimationIO;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		float mDuration = 0.0f;

	};
}