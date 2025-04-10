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

	TextureCache* tc = TextureCache::Get();
	MeshPX mesh = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
	mRenderObject.meshBuffer.Initialize<MeshPX>(mesh);
	mRenderObject.diffuseId = tc->LoadTexture("skysphere/sky.jpg");

	mRenderObject2.transform.position = { 2.0f, 0.0f, 0.0f };
	mRenderObject2.meshBuffer.Initialize<MeshPX>(mesh);
	mRenderObject2.diffuseId = tc->LoadTexture("skysphere/sky.jpg");

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mRenderObject.Terminate();
	mRenderObject2.Terminate();
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

	mStandardEffect.Begin();
		mStandardEffect.Render(mRenderObject);
		mStandardEffect.Render(mRenderObject2);
	mStandardEffect.End();
	SimpleDraw:: AddGroundPlane(10.0f, Colors::LightGray);
	SimpleDraw::Render(mCamera);

}
