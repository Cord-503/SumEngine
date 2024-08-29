#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;

const char* gDrawTypeNames[] =
{
	"None",
	"Line",
	"Sphere",
	"AABB",
	"AABBFilled"
};

void GameState::Initialize()
{	
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
}

void GameState::Terminate()
{
	
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

Vector3 minExtents = -Vector3::One;
Vector3 maxExtents = Vector3::One;
Color lineColor = Colors::Green;
void GameState::Render()
{
	if (mDebugDrawType == DebugDrawType::Sphere)
	{
		SimpleDraw::AddSphere(30, 30, 2.0f, minExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::Line)
	{
		SimpleDraw::AddLine(minExtents, maxExtents, lineColor);
	}
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	int currentDrawType = static_cast<int>(mDebugDrawType);
	if (ImGui::Combo("DrawType", &currentDrawType, gDrawTypeNames, static_cast<int>(std::size(gDrawTypeNames))))
	{
		mDebugDrawType = (DebugDrawType)currentDrawType;
	}

	if (mDebugDrawType == DebugDrawType::Sphere)
	{
		// ImGui::DragFloat("SphereRadius", &radius, 0.1f, 0.01f, 1000000.0f);
		ImGui::DragFloat3("Pos", &minExtents.x);
	}
	
	else if (mDebugDrawType == DebugDrawType::Line)
	{
		ImGui::DragFloat3("LineStart", &minExtents.x);
		ImGui::DragFloat3("LineEnd", &maxExtents.x);
	}


	ImGui::DragFloat3("minExtents", &minExtents.x);
	ImGui::DragFloat3("maxExtents", &maxExtents.x);
	ImGui::ColorEdit4("Color", &lineColor.r);
	ImGui::End();
}

