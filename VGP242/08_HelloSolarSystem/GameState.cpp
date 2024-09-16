#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;


void GameState::Initialize()
{
	MeshPX mesh = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
	MeshPX mesh2 = MeshBuilder::CreateSpherePX(60, 60, 2.0f);
	
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	objects[0].mMeshBuffer.Initialize<MeshPX>(mesh);
	objects[1].mMeshBuffer.Initialize<MeshPX>(mesh2);
	//mMeshBuffer.Initialize<MeshPX>(mesh);
	objects[0].mConstantBuffer.Initialize(sizeof(Matrix4));
	objects[1].mConstantBuffer.Initialize(sizeof(Matrix4));
	//mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
	objects[0].mVertexShader.Initialize<VertexPX>(shaderFile);
	objects[1].mVertexShader.Initialize<VertexPX>(shaderFile);
	//mVertexShader.Initialize<VertexPX>(shaderFile);
	objects[0].mPixelShader.Initialize(shaderFile);
	objects[1].mPixelShader.Initialize(shaderFile);
	//mPixelShader.Initialize(shaderFile);

	objects[0].mDiffuseTexture.Initialize("../../Assets/Images/planets/earth/earth.jpg");
	objects[1].mDiffuseTexture.Initialize("../../Assets/Images/planets/jupiter.jpg");
	//mDiffuseTexture.Initialize("../../Assets/Images/planets/earth/earth.jpg");
	objects[0].mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	objects[1].mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	//mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);
}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	objects[0].mSampler.Terminate();
	objects[0].mDiffuseTexture.Terminate();
	objects[0].mPixelShader.Terminate();
	objects[0].mVertexShader.Terminate();
	objects[0].mConstantBuffer.Terminate();
	objects[0].mMeshBuffer.Terminate();

	objects[1].mSampler.Terminate();
	objects[1].mDiffuseTexture.Terminate();
	objects[1].mPixelShader.Terminate();
	objects[1].mVertexShader.Terminate();
	objects[1].mConstantBuffer.Terminate();
	objects[1].mMeshBuffer.Terminate();
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

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	objects[0].mVertexShader.Bind();
	objects[0].mPixelShader.Bind();

	objects[1].mVertexShader.Bind();
	objects[1].mPixelShader.Bind();

	objects[0].mDiffuseTexture.BindPS(0);
	objects[0].mSampler.BindPS(0);

	objects[1].mDiffuseTexture.BindPS(0);
	objects[1].mSampler.BindPS(0);

	// constant buffer
	Matrix4 matWorld = Matrix4::Identity;
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	Matrix4 matFinal = matWorld * matView * matProj;
	Matrix4 wvp = Transpose(matFinal);
	objects[0].mConstantBuffer.Update(&wvp);
	objects[0].mConstantBuffer.BindVS(0);

	// mesh buffer
	objects[0].mMeshBuffer.Render();

	Matrix4 matWorld2 = Matrix4::Identity;
	Matrix4 matView2 = mCamera.GetViewMatrix();
	Matrix4 matProj2 = mCamera.GetProjectionMatrix();
	Matrix4 matFinal2 = matWorld2 * matView2 * matProj2;
	Matrix4 wvp2 = Transpose(matFinal2);
	objects[1].mConstantBuffer.Update(&wvp2);
	objects[1].mConstantBuffer.BindVS(0);

	objects[1].mMeshBuffer.Render();

	Matrix4 matWorld1 = Matrix4::Identity;
	Matrix4 matView1 = mRenderTargetCamera.GetViewMatrix();
	Matrix4 matProj1 = mRenderTargetCamera.GetProjectionMatrix();
	Matrix4 matFinal1 = matWorld1 * matView1 * matProj1;
	Matrix4 wvp1 = Transpose(matFinal1);
	objects[0].mConstantBuffer.Update(&wvp1);
	objects[0].mConstantBuffer.BindVS(0);

	mRenderTarget.BeginRender();
	objects[0].mMeshBuffer.Render();
	mRenderTarget.EndRender();
}

bool buttonValue = false;
int intValue = 0;
void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 256, 256 },
		{ 0, 0 },	//uv0
		{ 1, 1 },	//uv1
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 });
	ImGui::End();
}
