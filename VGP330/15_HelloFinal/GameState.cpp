#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	mCharater.Initialize("../../Assets/Models/Amy/Amy.model");

	Mesh ground = MeshBuilder::CreatePlane(10.0f, 10.0f, 1);
	mGround.meshBuffer.Initialize(ground);
	mGround.diffuseId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

	// 初始化标准效果
	std::filesystem::path standardShaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(standardShaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());

	// 初始化风格化效果
	std::filesystem::path stylizedShaderFile = L"../../Assets/Shaders/Stylized.fx";
	mStylizedEffect.Initialize(stylizedShaderFile);
	mStylizedEffect.SetCamera(mCamera);
	mStylizedEffect.SetDirectionalLight(mDirectionalLight);
	mStylizedEffect.SetShadowMap(mShadowEffect.GetDepthMap());
	mStylizedEffect.SetLightCamera(mShadowEffect.GetLightCamera());

	// 初始化阴影效果
	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	GraphicsSystem* gs = GraphicsSystem::Get();
	const uint32_t screenWidth = gs->GetBackBufferWidth();
	const uint32_t screenHeight = gs->GetBackBufferHeight();

	mOpaqueBlendState.Initialize(BlendState::Mode::Opaque);
	mAlphaBlendState.Initialize(BlendState::Mode::AlphaBlend);
}

void GameState::Terminate()
{
	mCharater.Terminate();
	mGround.Terminate();
	mStandardEffect.Terminate();
	mShadowEffect.Terminate();
	mStylizedEffect.Terminate();

	mOpaqueBlendState.Terminate();
	mAlphaBlendState.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
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
	// 渲染阴影贴图
	mShadowEffect.Begin();
	mShadowEffect.Render(mCharater);
	mShadowEffect.End();

	// 根据渲染模式选择渲染方式
	switch (mRenderMode)
	{
	case RenderMode::Standard:
		mStandardEffect.Begin();
		mStandardEffect.Render(mCharater);
		mStandardEffect.Render(mGround);
		mStandardEffect.End();
		break;

	case RenderMode::Stylized:
		mStylizedEffect.Begin();
		mStylizedEffect.Render(mCharater);
		mStylizedEffect.Render(mGround);
		mStylizedEffect.End();
		break;

	case RenderMode::Both:
		// 先渲染标准效果作为基础
		mOpaqueBlendState.Set();
		mStandardEffect.Begin();
		mStandardEffect.Render(mCharater);
		mStandardEffect.Render(mGround);
		mStandardEffect.End();

		// 然后使用透明混合渲染风格化效果
		mAlphaBlendState.Set();
		mStylizedEffect.Begin();
		mStylizedEffect.Render(mCharater);
		mStylizedEffect.Render(mGround);
		mStylizedEffect.End();

		// 恢复默认混合状态
		mOpaqueBlendState.Set();
		break;
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

	// 渲染模式选择
	if (ImGui::CollapsingHeader("Render Mode", ImGuiTreeNodeFlags_DefaultOpen))
	{
		const char* modes[] = { "Standard", "Stylized", "Both" };
		int currentMode = static_cast<int>(mRenderMode);
		if (ImGui::Combo("Mode", &currentMode, modes, IM_ARRAYSIZE(modes)))
		{
			mRenderMode = static_cast<RenderMode>(currentMode);
		}
	}

	mStandardEffect.DebugUI();
	mStylizedEffect.DebugUI();  // 添加这行
	mShadowEffect.DebugUI();

	ImGui::End();
}