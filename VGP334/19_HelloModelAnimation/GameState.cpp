#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;


void GameState::Initialize()
{

	mCamera.SetPosition({ 0.0f, 2.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mCamPosition = { 0.0f, 2.0f, -5.0f };
	mCamLookAt = { 0.0f, 0.0f, 0.0f };
	mCamDirction = mCamera.GetDirection();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	TextureCache* tc = TextureCache::Get();
	
	//mCharater.Initialize("../../Assets/Models/Ortiz/Ortiz.model");
	mCharater.Initialize("../../Assets/Models/Amy/Amy.model", &mCharacterAnimator);
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Amy/Arm_Stretching.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Amy/Run_Look_Back.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Amy/Breakdance_Freeze_Var3.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Amy/Amy_Firing_Rifle.model");
	mCharacterAnimator.Initialize(mCharater.modelId);
	mCharacterAnimator.PlayAnimation(0, true); // Start with the first animation

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mCharater.Terminate();
	//mRenderObject2.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	mCharacterAnimator.Update(deltaTime);
	mCamPosition = mCamera.GetPosition();
	mCamDirction = mCamera.GetDirection();
}

void GameState::UpdateCamera(float deltaTime)
{
	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
	const float turnSpeed = 0.1f * deltaTime;
	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
	}
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::AddTransform(mTempTransform.GetMatrix4());
	SimpleDraw::Render (mCamera);

	if (!useSkeleton)
	{
		mStandardEffect.Begin();
			mStandardEffect.Render(mCharater);
		mStandardEffect.End();
	}
	else
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::ComputeBoneTransforms(mCharater.modelId, boneTransforms, &mCharacterAnimator);
		AnimationUtil::DrawSkeleton(mCharater.modelId, boneTransforms);
	}

}

void GameState::DebugUI()
{
	ImGui::Begin("debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

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

	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Use Skeleton", &useSkeleton);

		int maxAnim = mCharacterAnimator.GetAnimationCount() - 1;
		if (ImGui::DragInt("AnimIndex", &mAnimationIndex, 1, -1, maxAnim))
		{
			mCharacterAnimator.PlayAnimation(mAnimationIndex, true);

		}
	}

	if (ImGui::CollapsingHeader("Camera Info", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("CamPos", &mCamPosition.x, 0.1f))
		{
			mCamera.SetPosition(mCamPosition);
			mCamera.SetLookAt(mCamLookAt);
		}
		if (ImGui::DragFloat3("CamDirction", &mCamDirction.x, 0.1f))
		{
			mCamera.SetDirection(mCamDirction);
			mCamera.SetLookAt(mCamLookAt);
		}
		if (ImGui::DragFloat3("CamLookPoint", &mCamLookAt.x, 0.1f))
		{
			mCamera.SetLookAt(mCamLookAt);
		}
	}

	if (ImGui::CollapsingHeader("Character Info", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Character1 Pos", &mTempTransform.position.x, 0.1f);

		if (ImGui::DragFloat3("Character1 Rot", &mRotation.x, 0.1f))
		{
			mTempTransform.rotation = Math::Quaternion::CreateFromYawPitchRoll(mRotation.x, mRotation.y, mRotation.z);
		}
		if (ImGui::Button("MoveToTransform"))
		{
			mCharater.transform = mTempTransform;
		}
	}

	mStandardEffect.DebugUI();
	ImGui::End();

}