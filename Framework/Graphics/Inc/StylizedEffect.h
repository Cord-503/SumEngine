#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "RasterizerState.h"
namespace SumEngine::Graphics
{
    class Camera;
    class RenderObject;
    class RenderGroup;
    class Texture;

    class StylizedEffect final
    {
    public:
        void Initialize(const std::filesystem::path& path);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);
        void Render(const RenderGroup& renderGroup);

        void SetCamera(const Camera& camera);
        void SetDirectionalLight(const DirectionalLight& light);
        void SetLightCamera(const Camera& camera);
        void SetShadowMap(const Texture& shadowMap);

        void DebugUI();

    private:
        struct TransformData
        {
            Math::Matrix4 wvp;
            Math::Matrix4 world;
            Math::Vector3 viewPosition;
            float padding = 0.0f;
        };

        struct SettingsData
        {
            int useDiffuseMap = 1;
            int useNormalMap = 1;
            int useShadowMap = 1;
            int renderOutline = 1;
            float outlineWidth = 0.1f;
            float colorBands = 4.0f;
            float specularThreshold = 0.5f;
            float outlineThickness = 0.01f;
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        TransformBuffer mTransformBuffer;

        using LightBuffer = TypedConstantBuffer<DirectionalLight>;
        LightBuffer mLightBuffer;

        using MaterialBuffer = TypedConstantBuffer<Material>;
        MaterialBuffer mMaterialBuffer;

        using SettingsBuffer = TypedConstantBuffer<SettingsData>;
        SettingsBuffer mSettingsBuffer;

        VertexShader mVertexShader;
        VertexShader mOutlineVertexShader;
        PixelShader mPixelShader;
        PixelShader mOutlinePixelShader;
        Sampler mSampler;

        SettingsData mSettingsData;
        const Camera* mCamera = nullptr;
        const Camera* mLightCamera = nullptr;
        const DirectionalLight* mDirectionalLight = nullptr;
        const Texture* mShadowMap = nullptr;
        RasterizerState mRasterizerStateCullFront;
        RasterizerState mRasterizerStateCullBack;
    };
}