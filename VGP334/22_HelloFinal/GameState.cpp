#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;
using namespace SumEngine::Audio;

void GameState::Initialize()
{
	mCamera.SetPosition({ -0.225f, 1.076f, -0.953f });
	mCamera.SetDirection({ 0.004f, -0.057f, 0.998f });

	mDirectionalLight.direction = Normalize({ -0.6, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mBgmId = SoundEffectManager::Get()->Load("../../Assets/Sounds/PlayOfTheGame.wav");
	PlayBGM();

	Mesh groundMesh = MeshBuilder::CreatePlane(20, 20, 1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

	mCharater.Initialize("../../Assets/Models/Amy/Amy.model", &mCharacterAnimator);
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Amy/Arm_Stretching.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Amy/Animations/TakingPunchL.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Amy/Animations/TakingPunchR.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Amy/Animations/Martelo1.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Amy/Animations/Martelo2.model");

	mCharacterAnimator.Initialize(mCharater.modelId);

	mCharacter2.Initialize("../../Assets/Models/Vegas/Vegas.model", &mCharacterAnimator2);
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vegas/VegasJabCross.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vegas/ReceiveUppercutToTheFace.model");

	mCharacterAnimator2.Initialize(mCharacter2.modelId);
	mCharacter2.transform.position = { -0.300f, 0.000f, 0.500f };
	mCharacter2.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, -5.400f, 0.000f);
	mCharacterAnimator2.PlayAnimation(0, true);

	mFirework.Initialize();
	mFirework2.Initialize();
	mFirework3.Initialize();

	mCameraAnimation = AnimationBuilder()
		.AddPositionKey({ -0.225f, 1.076f, -0.953f }, 0.0f)
		.AddPositionKey({ -0.225f, 1.076f, -0.953f }, 1.0f)
		.AddPositionKey({ 0.257f, 0.651f, -1.353f }, 3.5f, EaseType::EaseInOut)
		.Build();

	mCharacterAnimator.PlayAnimation(0, true);
	mCurrentScene = SceneState::Scene1;

	mSceneTime = 0.0f;
	mScene2Timer = 0.0f;
	mScene3Timer = 0.0f;
	mScene4Timer = 0.0f;

	mFireworkTriggered = false;
	mFightAnimation2Played = false;
	mIsCharacter2Visible = false;
	mIsPaused = false;
}

void GameState::Terminate()
{
	mFirework.Terminate();
	mFirework2.Terminate();
	mFirework3.Terminate();

	mStandardEffect.Terminate();
	mCharater.Terminate();
	mCharacter2.Terminate();
	mGround.Terminate();
}

void GameState::Update(float deltaTime)
{
	if (mIsPaused)
	{
		UpdateCamera(deltaTime);
	}
	else
	{
		mCharacterAnimator.Update(deltaTime);
		mCharacterAnimator2.Update(deltaTime);
		mFirework.Update(deltaTime);
		mFirework2.Update(deltaTime);
		mFirework3.Update(deltaTime);

		switch (mCurrentScene)
		{
		case SceneState::Scene1:
			UpdateScene1(deltaTime);
			break;
		case SceneState::Scene2:
			UpdateScene2(deltaTime);
			break;
		case SceneState::Scene3:
			UpdateScene3(deltaTime);
			break;
		case SceneState::Scene4:
			UpdateScene4(deltaTime);
			break;
		case SceneState::Scene5:
			UpdateScene5(deltaTime);
			break;
		}
	}

	mCamPosition = mCamera.GetPosition();
	mCamDirection = mCamera.GetDirection();

	if (!mIsPaused)
	{
		mTempTransform.position = mCharater.transform.position;
		mTempTransform2.position = mCharacter2.transform.position;
	}
}

void GameState::UpdateScene1(float deltaTime)
{
	mSceneTime += deltaTime;

	if (mSceneTime >= 1.0f && !mFireworkTriggered)
	{
		mFirework.Start({ -5.0f, 0.3f, 8.0f });
		mFirework2.Start({ 0.0f, 0.3f, 10.0f });
		mFirework3.Start({ 5.0f, 0.3f, 8.0f });
		mFireworkTriggered = true;
	}

	Vector3 startDir = Normalize({ 0.004f, -0.057f, 0.998f });
	Vector3 endDir = Normalize({ -0.355f, 0.356f, 0.864f });
	float t = Clamp((mSceneTime - 1.0f) / (mSceneDuration - 1.0f), 0.0f, 1.0f);
	Vector3 currentPos = mCameraAnimation.GetTransform(mSceneTime).position;
	Vector3 currentDir = Normalize(Lerp(startDir, endDir, t));
	mCamera.SetPosition(currentPos);
	mCamera.SetDirection(currentDir);

	if (mSceneTime >= 5.0f)
	{
		mCurrentScene = SceneState::Scene2;
		mScene2Timer = 0.0f;

		mCharater.transform.position = { -1.600f, 0.000f, 0.000f };
		mCharater.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, -1.900f, 0.000f);

		mCharacterAnimator.PlayAnimation(1, false);

		mIsCharacter2Visible = true;
	}
}

void GameState::UpdateScene2(float deltaTime)
{
	mCamera.SetPosition({ 0.636f, 1.061f, -2.046f });
	mCamera.SetDirection(Normalize({ -0.527f, -0.207f, 0.824f }));

	mScene2Timer += deltaTime;
	mScene3Timer == 0.0;

	if (mScene2Timer >= 0.5f && !mFightAnimation2Played)
	{
		mCharacterAnimator.PlayAnimation(2, false);
		mFightAnimation2Played = true;
	}

	if (mScene2Timer >= 1.5f && mFightAnimation2Played)
	{
		mCurrentScene = SceneState::Scene3;

		mCharacterAnimator.PlayAnimation(3, true);
		mCharacterAnimator2.PlayAnimation(1, true);
	}
}

void GameState::UpdateScene3(float deltaTime)
{
	mCamera.SetPosition({ -2.329f, 0.925f, -0.665f });
	mCamera.SetDirection({ 0.870f, 0.085f, 0.486f });

	mScene3Timer += deltaTime;
	mScene4Timer == 0.0f;
	if (mScene3Timer >= 1.3f)
	{
		mCurrentScene = SceneState::Scene4;
		mCharacterAnimator.PlayAnimation(4, true);
		mCharacterAnimator2.PlayAnimation(1, true);
	}

}

void GameState::UpdateScene4(float deltaTime)
{
	mCamera.SetPosition({ -0.306, 1.614, 1.724 });
	mCamera.SetDirection({ -0.132, -0.319, -0.938 });
	if (mScene3Timer >= 1.3f)
	{
		mCurrentScene = SceneState::Scene5;
		mCharacterAnimator.PlayAnimation(4, true);
		mCharacterAnimator2.PlayAnimation(1, true);
	}
}

void GameState::UpdateScene5(float deltaTime)
{
	mCamera.SetPosition({ -0.306, 1.614, 1.724 });
	mCamera.SetDirection({ -0.132, -0.319, -0.938 });

}

void GameState::Render()
{
	mStandardEffect.SetCamera(mCamera);

	mStandardEffect.Begin();
	mStandardEffect.Render(mCharater);
	if (mIsCharacter2Visible)
	{
		mStandardEffect.Render(mCharacter2);
	}
	mStandardEffect.Render(mGround);
	mStandardEffect.End();

	mFirework.Render(mCamera);
	mFirework2.Render(mCamera);
	mFirework3.Render(mCamera);

	SimpleDraw::AddTransform(mTempTransform.GetMatrix4());
	SimpleDraw::AddTransform(mTempTransform2.GetMatrix4());
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Scene Time: %.2f / %.2f", mSceneTime + mScene2Timer, mSceneDuration);
	if (ImGui::Button("Restart Scene"))
	{
		mCurrentScene = SceneState::Scene1;
		mSceneTime = 0.0f;
		mScene2Timer = 0.0f;
		mFireworkTriggered = false;
		mFightAnimation2Played = false;
		mIsCharacter2Visible = false;

		mCharacterAnimator.PlayAnimation(0, true);
		mCharacterAnimator2.PlayAnimation(0, true);
		mCharater.transform.position = Vector3::Zero;
		mCharater.transform.rotation = Quaternion::Identity;
		mCharacter2.transform.position = { -0.300f, 0.000f, 0.500f };
		mCharacter2.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, -5.400f, 0.000f);
	}
	ImGui::SameLine();
	ImGui::Checkbox("Pause Scene", &mIsPaused);

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Audio", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Play BGM")) { PlayBGM(); }
		ImGui::SameLine();
		if (ImGui::Button("Stop BGM")) { StopBGM(); }
	}

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction##Light", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}

	if (ImGui::CollapsingHeader("Camera Info", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("CamPos: %.3f, %.3f, %.3f", mCamPosition.x, mCamPosition.y, mCamPosition.z);
		ImGui::Text("CamDir: %.3f, %.3f, %.3f", mCamDirection.x, mCamDirection.y, mCamDirection.z);

		if (mIsPaused)
		{
			if (ImGui::DragFloat3("Edit CamPos", &mCamPosition.x, 0.1f))
			{
				mCamera.SetPosition(mCamPosition);
			}
			if (ImGui::DragFloat3("Edit LookAt", &mCamLookAt.x, 0.1f))
			{
				mCamera.SetLookAt(mCamLookAt);
			}
		}
	}

	if (ImGui::CollapsingHeader("Character 1 Info (Amy)", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Debug Pos##Amy", &mTempTransform.position.x, 0.1f);
		if (ImGui::DragFloat3("Debug Rot##Amy", &mRotation.x, 0.1f))
		{
			mTempTransform.rotation = Math::Quaternion::CreateFromYawPitchRoll(mRotation.y, mRotation.x, mRotation.z);
		}
		if (ImGui::Button("Move Character To Transform##Amy"))
		{
			mCharater.transform = mTempTransform;
		}
	}
	if (ImGui::CollapsingHeader("Character 2 Info (Vegas)", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Debug Pos##Vegas", &mTempTransform2.position.x, 0.1f);
		if (ImGui::DragFloat3("Debug Rot##Vegas", &mRotation2.x, 0.1f))
		{
			mTempTransform2.rotation = Math::Quaternion::CreateFromYawPitchRoll(mRotation2.y, mRotation2.x, mRotation2.z);
		}
		if (ImGui::Button("Move Character To Transform##Vegas"))
		{
			mCharacter2.transform = mTempTransform2;
		}
	}

	mStandardEffect.DebugUI();

	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	auto& io = ImGui::GetIO();
	if (io.WantCaptureMouse || io.WantCaptureKeyboard)
	{
		return;
	}

	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
	const float turnSpeed = 0.1f * deltaTime;
	if (input->IsKeyDown(KeyCode::W)) { mCamera.Walk(moveSpeed); }
	else if (input->IsKeyDown(KeyCode::S)) { mCamera.Walk(-moveSpeed); }
	if (input->IsKeyDown(KeyCode::D)) { mCamera.Strafe(moveSpeed); }
	else if (input->IsKeyDown(KeyCode::A)) { mCamera.Strafe(-moveSpeed); }
	if (input->IsKeyDown(KeyCode::E)) { mCamera.Rise(moveSpeed); }
	else if (input->IsKeyDown(KeyCode::Q)) { mCamera.Rise(-moveSpeed); }

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
	}
}

void GameState::PlayBGM() { SoundEffectManager::Get()->Play(mBgmId, true); }
void GameState::StopBGM() { SoundEffectManager::Get()->Stop(mBgmId); }