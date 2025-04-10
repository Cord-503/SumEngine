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
	mConstantBuffer.Initialize(sizeof(Math::Matrix4));

	mVertexShader.Initialize<VertexPX>(path);
	mPixelShader.Initialize(path);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{
	mConstantBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();
}

void StandardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindPS(0);
	mConstantBuffer.BindVS(0);
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
	const Math::Matrix4 wvp = Transpose(matFinal);
	mConstantBuffer.Update(&wvp);

	TextureCache* tc = TextureCache::Get();
	tc->BindPS(renderObject.diffuseId, 0);


	renderObject.meshBuffer.Render();
	
}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;

}

void StandardEffect::DebugUI()
{
}
