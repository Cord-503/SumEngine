#include "Precompiled.h"
#include "StylizedEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "Texture.h"
#include "RasterizerState.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

void StylizedEffect::Initialize(const std::filesystem::path& path)
{
    mTransformBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
    mSettingsBuffer.Initialize();

    // Initialize shaders for main pass
    mVertexShader.Initialize<Vertex>(path);
    mPixelShader.Initialize(path);

    // Initialize shaders for outline pass
    mOutlineVertexShader.Initialize<Vertex>(path);
    mOutlinePixelShader.Initialize(path);

    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    // Initialize rasterizer states for outline rendering
    mRasterizerStateCullFront.Initialize(RasterizerState::CullMode::Front, RasterizerState::FillMode::Solid);
    mRasterizerStateCullBack.Initialize(RasterizerState::CullMode::Back, RasterizerState::FillMode::Solid);
}

void StylizedEffect::Terminate()
{
    mVertexShader.Terminate();
    mPixelShader.Terminate();
    mOutlineVertexShader.Terminate();
    mOutlinePixelShader.Terminate();
    mSampler.Terminate();
    mTransformBuffer.Terminate();
    mLightBuffer.Terminate();
    mMaterialBuffer.Terminate();
    mSettingsBuffer.Terminate();
    mRasterizerStateCullFront.Terminate();
    mRasterizerStateCullBack.Terminate();
}

void StylizedEffect::Begin()
{
    // Bind constant buffers and sampler
    mTransformBuffer.BindVS(0);
    mLightBuffer.BindPS(1);
    mLightBuffer.BindVS(1);
    mMaterialBuffer.BindPS(2);
    mSettingsBuffer.BindPS(3);
    mSettingsBuffer.BindVS(3);
    mSampler.BindPS(0);
}

void StylizedEffect::End()
{
    if (mSettingsData.useShadowMap > 0 && mShadowMap != nullptr)
    {
        Texture::UnbindPS(4);
    }
}

void StylizedEffect::Render(const RenderObject& renderObject)
{
    // Prepare transform data
    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData data;
    data.wvp = Math::Transpose(matFinal);
    data.world = Math::Transpose(matWorld);
    data.viewPosition = mCamera->GetPosition();

    // Shadow map setup
    if (mSettingsData.useShadowMap > 0 && mShadowMap != nullptr)
    {
        mShadowMap->BindPS(4);
    }

    mTransformBuffer.Update(data);
    mLightBuffer.Update(*mDirectionalLight);
    mMaterialBuffer.Update(renderObject.material);

    // Update settings
    SettingsData settingsData = mSettingsData;
    settingsData.useDiffuseMap = (mSettingsData.useDiffuseMap > 0 && renderObject.diffuseId > 0) ? 1 : 0;
    settingsData.useNormalMap = (mSettingsData.useNormalMap > 0 && renderObject.normalId > 0) ? 1 : 0;
    settingsData.useShadowMap = mSettingsData.useShadowMap > 0 && mShadowMap != nullptr;
    mSettingsBuffer.Update(settingsData);

    // Bind textures
    TextureCache* tc = TextureCache::Get();
    tc->BindPS(renderObject.diffuseId, 0);
    tc->BindPS(renderObject.normalId, 1);

    // Render outline pass first (if enabled)
    if (mSettingsData.renderOutline > 0)
    {
        // Set front face culling for outline rendering
        mRasterizerStateCullFront.Set();

        mOutlineVertexShader.Bind();
        mOutlinePixelShader.Bind();

        renderObject.meshBuffer.Render();

        // Reset to back face culling
        mRasterizerStateCullBack.Set();
    }

    // Render main stylized pass
    mVertexShader.Bind();
    mPixelShader.Bind();

    renderObject.meshBuffer.Render();
}

void StylizedEffect::Render(const RenderGroup& renderGroup)
{
    // Similar to StandardEffect but for each object in the group
    const Math::Matrix4 matWorld = renderGroup.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData data;
    data.wvp = Math::Transpose(matFinal);
    data.world = Math::Transpose(matWorld);
    data.viewPosition = mCamera->GetPosition();

    if (mSettingsData.useShadowMap > 0 && mShadowMap != nullptr)
    {
        mShadowMap->BindPS(4);
    }

    mTransformBuffer.Update(data);
    mLightBuffer.Update(*mDirectionalLight);

    TextureCache* tc = TextureCache::Get();

    for (const RenderObject& renderObject : renderGroup.renderObjects)
    {
        SettingsData settingsData = mSettingsData;
        settingsData.useDiffuseMap = (mSettingsData.useDiffuseMap > 0 && renderObject.diffuseId > 0) ? 1 : 0;
        settingsData.useNormalMap = (mSettingsData.useNormalMap > 0 && renderObject.normalId > 0) ? 1 : 0;
        settingsData.useShadowMap = mSettingsData.useShadowMap > 0 && mShadowMap != nullptr;
        mSettingsBuffer.Update(settingsData);
        mMaterialBuffer.Update(renderObject.material);

        tc->BindPS(renderObject.diffuseId, 0);
        tc->BindPS(renderObject.normalId, 1);

        // Outline pass
        if (mSettingsData.renderOutline > 0)
        {
            mRasterizerStateCullFront.Set();
            mOutlineVertexShader.Bind();
            mOutlinePixelShader.Bind();
            renderObject.meshBuffer.Render();
            mRasterizerStateCullBack.Set();
        }

        // Main pass
        mVertexShader.Bind();
        mPixelShader.Bind();
        renderObject.meshBuffer.Render();
    }
}

void StylizedEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void StylizedEffect::SetLightCamera(const Camera& camera)
{
    mLightCamera = &camera;
}

void StylizedEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
    mDirectionalLight = &directionalLight;
}

void StylizedEffect::SetShadowMap(const Texture& shadowMap)
{
    mShadowMap = &shadowMap;
}

void StylizedEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("StylizedEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
        if (ImGui::Checkbox("UseDiffuseMap", &useDiffuseMap))
        {
            mSettingsData.useDiffuseMap = (useDiffuseMap) ? 1 : 0;
        }

        bool useNormalMap = mSettingsData.useNormalMap > 0;
        if (ImGui::Checkbox("UseNormalMap", &useNormalMap))
        {
            mSettingsData.useNormalMap = (useNormalMap) ? 1 : 0;
        }

        bool useShadowMap = mSettingsData.useShadowMap > 0;
        if (ImGui::Checkbox("UseShadowMap", &useShadowMap))
        {
            mSettingsData.useShadowMap = useShadowMap ? 1 : 0;
        }

        bool renderOutline = mSettingsData.renderOutline > 0;
        if (ImGui::Checkbox("RenderOutline", &renderOutline))
        {
            mSettingsData.renderOutline = renderOutline ? 1 : 0;
        }

        ImGui::DragFloat("OutlineWidth", &mSettingsData.outlineWidth, 0.001f, 0.0f, 1.0f);
        ImGui::DragFloat("OutlineThickness", &mSettingsData.outlineThickness, 0.001f, 0.001f, 0.1f);
        ImGui::DragFloat("ColorBands", &mSettingsData.colorBands, 0.1f, 2.0f, 10.0f);
        ImGui::DragFloat("SpecularThreshold", &mSettingsData.specularThreshold, 0.01f, 0.0f, 1.0f);
    }
}