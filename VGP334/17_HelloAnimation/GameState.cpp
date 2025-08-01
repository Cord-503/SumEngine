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

	Mesh mesh = MeshBuilder::CreateSphere(60, 60, 1.0f);
	mRenderObject.meshBuffer.Initialize<Mesh>(mesh);
	mRenderObject.diffuseId = tc->LoadTexture("planets/earth/earth.jpg");
	mRenderObject.normalId = tc->LoadTexture("planets/earth/earth_normal.jpg");
	mRenderObject.specularId = tc->LoadTexture("planets/earth/earth_spec.jpg");
	mRenderObject.bumpId = tc->LoadTexture("planets/earth/earth_bump.jpg");


	//mRenderObject2.transform.position = { 2.0f, 0.0f, 0.0f };
	//mRenderObject2.meshBuffer.Initialize<Mesh>(mesh);
	//mRenderObject2.diffuseId = tc->LoadTexture("skysphere/sky.jpg");

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	//mAnimation = AnimationBuilder()
	//	.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 0.0f, EaseType::EaseIn)
	//	.AddPositionKey({ 0.0f, 2.0f, 0.0f }, 1.0f, EaseType::EaseIn)
	//	.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 2.0f, EaseType::EaseIn)
	//	.AddRotationKey(Quaternion::Identity, 0.0f)
	//	.AddRotationKey(Quaternion::CreateFromAxisAngle(Vector3::YAxis, 90.0f* Math::Constants::DegToRad), 0.5f)
	//	.AddRotationKey(Quaternion::CreateFromAxisAngle(Vector3::YAxis, 180.0f* Math::Constants::DegToRad), 1.0f)
	//	.AddRotationKey(Quaternion::CreateFromAxisAngle(Vector3::YAxis, 270.0f* Math::Constants::DegToRad), 1.5f)
	//	.AddRotationKey(Quaternion::CreateFromAxisAngle(Vector3::YAxis, 360.0f* Math::Constants::DegToRad), 2.0f)
	//	.AddScaleKey({ 1.0f, 1.0f, 1.0f }, 0.0f)
	//	.AddScaleKey(Math::Vector3::One * 0.5f, 1.0f)
	//	.AddScaleKey(Math::Vector3::One, 2.0f)
	//	.Build();

	mAnimation = AnimationBuilder()
		.AddPositionKey({ 2.0f, 2.0f, 0.0f }, 0.0f, EaseType::EaseInOut)
		.AddPositionKey({ 1.0f, 3.0f, 0.0f }, 1.875f, EaseType::EaseInOut)
		.AddPositionKey({ -2.0f, 2.0f, 0.0f }, 3.75f, EaseType::EaseInOut)
		.AddPositionKey({ -3.0f, 0.0f, 0.0f }, 5.625f, EaseType::EaseInOut)
		.AddPositionKey({ -2.0f, -2.0f, 0.0f }, 7.5f, EaseType::EaseInOut)
		.AddPositionKey({ -1.0f, -3.0f, 0.0f }, 9.375f, EaseType::EaseInOut)
		.AddPositionKey({ 2.0f, -2.0f, 0.0f }, 11.25f, EaseType::EaseInOut)
		.AddPositionKey({ 3.0f, 0.0f, 0.0f }, 13.125f, EaseType::EaseInOut)
		.AddPositionKey({ 2.0f, 2.0f, 0.0f }, 15.0f, EaseType::EaseInOut)

		.AddRotationKey(Quaternion::Identity, 0.0f)
		.AddRotationKey(Quaternion::CreateFromAxisAngle(Vector3::YAxis, 120.0f * Math::Constants::DegToRad), 2.5f)
		.AddRotationKey(Quaternion::CreateFromAxisAngle(Vector3::YAxis, 240.0f * Math::Constants::DegToRad), 5.0f)
		.AddRotationKey(Quaternion::CreateFromAxisAngle(Vector3::YAxis, 360.0f * Math::Constants::DegToRad), 7.5f)
		.AddRotationKey(Quaternion::CreateFromAxisAngle(Vector3::YAxis, 480.0f * Math::Constants::DegToRad), 10.0f)
		.AddRotationKey(Quaternion::CreateFromAxisAngle(Vector3::YAxis, 600.0f * Math::Constants::DegToRad), 12.5f)
		.AddRotationKey(Quaternion::CreateFromAxisAngle(Vector3::YAxis, 720.0f * Math::Constants::DegToRad), 15.0f)

		.AddScaleKey(Math::Vector3::One, 0.0f)
		.AddScaleKey(Math::Vector3::One * 0.8f, 1.25f)
		.AddScaleKey(Math::Vector3::One, 2.5f)
		.AddScaleKey(Math::Vector3::One * 0.8f, 3.75f)
		.AddScaleKey(Math::Vector3::One, 5.0f)
		.AddScaleKey(Math::Vector3::One * 0.8f, 6.25f)
		.AddScaleKey(Math::Vector3::One, 7.5f)
		.AddScaleKey(Math::Vector3::One * 0.8f, 8.75f)
		.AddScaleKey(Math::Vector3::One, 10.0f)
		.AddScaleKey(Math::Vector3::One * 0.8f, 11.25f)
		.AddScaleKey(Math::Vector3::One, 12.5f)
		.AddScaleKey(Math::Vector3::One * 0.8f, 13.75f)
		.AddScaleKey(Math::Vector3::One, 15.0f)
		.Build();

}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mRenderObject.Terminate();
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

	mAnimationTime += deltaTime;
	while (mAnimation.GetDuration() > 0.0f && mAnimationTime > mAnimation.GetDuration())
	{
		mAnimationTime -= mAnimation.GetDuration();
	}
	mRenderObject.transform = mAnimation.GetTransform(mAnimationTime);

}

void GameState::Render()
{

	mStandardEffect.Begin();
		mStandardEffect.Render(mRenderObject);
		//mStandardEffect.Render(mRenderObject2);
	mStandardEffect.End();
	SimpleDraw:: AddGroundPlane(10.0f, Colors::LightGray);
	SimpleDraw::Render(mCamera);

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
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &mRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mRenderObject.material.specular.r);
		ImGui::ColorEdit4("Emissive##Material", &mRenderObject.material.emissive.r);
		ImGui::DragFloat("SpecPower##Material", &mRenderObject.material.shininess, 0.01f, 0.0f, 100.0f);
	}
	mStandardEffect.DebugUI();
	ImGui::End();

}