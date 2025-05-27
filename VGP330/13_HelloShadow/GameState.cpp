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

	TextureCache* tc = TextureCache::Get();
	
	//mCharater.Initialize("../../Assets/Models/Ortiz/Ortiz.model");
	mCharater.Initialize("../../Assets/Models/Amy/Amy.model");

	Mesh ground = MeshBuilder::CreatePlane(10.0f, 10.0f, 1);
	mGround.meshBuffer.Initialize(ground);
	mGround.diffuseId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");


	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());


	// Initialize the shadow effect
	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	Mesh sphere = MeshBuilder::CreateSphere(500, 20, 1.5f);
	mSphere.meshBuffer.Initialize(sphere);
	mSphere.transform.position.x = 21.0f;
	mSphere.transform.position.y = 3.5f;

	GraphicsSystem* gs = GraphicsSystem::Get();
	const uint32_t screenWidth = gs->GetBackBufferWidth();
	const uint32_t screenHeight = gs->GetBackBufferHeight();
}

void GameState::Terminate()
{
	mCharater.Terminate();
	mGround.Terminate();
	mStandardEffect.Terminate();
	mShadowEffect.Terminate();
	mSphere.Terminate();
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
	mShadowEffect.Begin();
		mShadowEffect.Render(mCharater);

		mShadowEffect.Render(mSphere);
	mShadowEffect.End();

	mStandardEffect.Begin();
		mStandardEffect.Render(mCharater);

		mShadowEffect.Render(mSphere);

		mStandardEffect.Render(mGround);
	mStandardEffect.End();
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
	mStandardEffect.DebugUI();

	mShadowEffect.DebugUI();

	ImGui::End();

}