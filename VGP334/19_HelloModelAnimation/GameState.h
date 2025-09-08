#pragma once

#include <SumEngine/Inc/SumEngine.h>

class GameState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime);
	void Render() override;

	void DebugUI() override;

protected:
	void UpdateCamera(float deltaTime);

	SumEngine::Graphics::Camera mCamera;

	SumEngine::Graphics::DirectionalLight mDirectionalLight;

	SumEngine::Graphics::RenderGroup mCharater;

	SumEngine::Graphics::Animator mCharacterAnimator;

	SumEngine::Graphics::StandardEffect mStandardEffect;

	bool useSkeleton = false;
	int mAnimationIndex = -1;

	SumEngine::Math::Vector3 mRotation = { 0.0f, 0.0f, 0.0f };
	SumEngine::Graphics::Transform mTempTransform;

	SumEngine::Math::Vector3 mCamLookAt = { 0.0f, 0.0f, 0.0f };
	SumEngine::Math::Vector3 mCamPosition = { 0.0f, 0.0f, 0.0f };
	SumEngine::Math::Vector3 mCamDirction = { 0.0f, 0.0f, 0.0f };
};
