#pragma once

#include <SumEngine/Inc/SumEngine.h>
#include "Firework.h"

enum class SceneState
{
	Scene1,
	Scene2,
	Scene3,
	Scene4,
	Scene5
};

class GameState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime);
	void Render() override;
	void DebugUI() override;

private:
	void PlayBGM();
	void StopBGM();
	void UpdateCamera(float deltaTime);

	void UpdateScene1(float deltaTime);
	void UpdateScene2(float deltaTime);
	void UpdateScene3(float deltaTime);
	void UpdateScene4(float deltaTime);
	void UpdateScene5(float deltaTime);

	SumEngine::Graphics::Camera mCamera;
	SumEngine::Graphics::DirectionalLight mDirectionalLight;
	SumEngine::Graphics::StandardEffect mStandardEffect;
	SumEngine::Graphics::RenderObject mGround;

	SumEngine::Graphics::RenderGroup mCharater;
	SumEngine::Graphics::Animator mCharacterAnimator;
	SumEngine::Math::Vector3 mRotation = { 0.0f, 0.0f, 0.0f };
	SumEngine::Graphics::Transform mTempTransform;

	SumEngine::Graphics::RenderGroup mCharacter2;
	SumEngine::Graphics::Animator mCharacterAnimator2;
	SumEngine::Math::Vector3 mRotation2 = { 0.0f, 0.0f, 0.0f };
	SumEngine::Graphics::Transform mTempTransform2;

	Firework mFirework;
	Firework mFirework2;
	Firework mFirework3;

	SumEngine::Graphics::Animation mCameraAnimation;

	SceneState mCurrentScene = SceneState::Scene1;
	float mSceneTime = 0.0f;
	float mScene2Timer = 0.0f;
	float mScene3Timer = 0.0f;
	float mScene4Timer = 0.0f;



	bool mFireworkTriggered = false;
	bool mFightAnimation2Played = false;
	bool mIsCharacter2Visible = false;

	const float mSceneDuration = 10.0f;
	float mTimer = 0.0f;
	SumEngine::Audio::SoundId mBgmId;
	bool mIsPaused = false;

	SumEngine::Math::Vector3 mCamPosition = { 0.0f, 0.0f, 0.0f };
	SumEngine::Math::Vector3 mCamDirection = { 0.0f, 0.0f, 1.0f };
	SumEngine::Math::Vector3 mCamLookAt = { 0.0f, 0.0f, 0.0f };
};