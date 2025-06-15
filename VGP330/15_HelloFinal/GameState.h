#pragma once

#include <SumEngine/Inc/SumEngine.h>

class GameState : public SumEngine::AppState
{
private:
    SumEngine::Graphics::BlendState mOpaqueBlendState;
    SumEngine::Graphics::BlendState mAlphaBlendState; 
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

protected:
    void UpdateCamera(float deltaTime);

    SumEngine::Graphics::Camera mCamera;
    SumEngine::Graphics::DirectionalLight mDirectionalLight;

    SumEngine::Graphics::RenderGroup mCharater;
    SumEngine::Graphics::RenderObject mGround;

    SumEngine::Graphics::StandardEffect mStandardEffect;
    SumEngine::Graphics::ShadowEffect mShadowEffect;
    SumEngine::Graphics::StylizedEffect mStylizedEffect;
    enum class RenderMode
    {
        Standard,
        Stylized,
        Both
    };
    RenderMode mRenderMode = RenderMode::Both;

};