#pragma once
#include "Precompiled.h"
#include "StandardEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "Texture.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& path)
{
	mTransformBuffer.Initialize();
	
	mLightBuffer.Initialize();

	mMaterialBuffer.Initialize();

	mVertexShader.Initialize<Vertex>(path);
	mPixelShader.Initialize(path);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{

	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
}

void StandardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mLightBuffer.BindPS(1);
	mLightBuffer.BindVS(1);
	mMaterialBuffer.BindPS(2);

	mSampler.BindPS(0);
}

void StandardEffect::End()
{
	// null
}

void StandardEffect::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matMatrix = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	const Math::Matrix4 matFinal = matMatrix * matView * matProj;

	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	data.world = Math::Transpose(matMatrix);
	data.viewPosition = mCamera->GetPosition();
	mTransformBuffer.Update(data);
	mLightBuffer.Update(*mDirectionalLight);

	mMaterialBuffer.Update(renderObject.material);

	TextureCache* tc = TextureCache::Get();
	tc->BindPS(renderObject.diffuseId, 0);

	renderObject.meshBuffer.Render();
	
}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;

}

void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void StandardEffect::DebugUI()
{
}
