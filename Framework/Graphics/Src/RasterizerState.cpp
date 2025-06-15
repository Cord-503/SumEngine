#include "Precompiled.h"
#include "RasterizerState.h"

#include "GraphicsSystem.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

std::unique_ptr<RasterizerState> RasterizerState::sInstance = nullptr;

RasterizerState* RasterizerState::Get()
{
    if (sInstance == nullptr)
    {
        sInstance = std::make_unique<RasterizerState>();
    }
    return sInstance.get();
}

void RasterizerState::ClearState()
{
    sInstance.reset();
}

RasterizerState::~RasterizerState()
{
    ASSERT(mRasterizerState == nullptr, "RasterizerState: rasterizer state not released!");
}

void RasterizerState::Initialize(CullMode cullMode, FillMode fillMode)
{
    D3D11_RASTERIZER_DESC desc = {};
    desc.FrontCounterClockwise = false;
    desc.DepthBias = 0;
    desc.DepthBiasClamp = 0.0f;
    desc.SlopeScaledDepthBias = 0.0f;
    desc.DepthClipEnable = true;
    desc.ScissorEnable = false;
    desc.MultisampleEnable = false;
    desc.AntialiasedLineEnable = false;

    // Set cull mode
    switch (cullMode)
    {
    case CullMode::None:
        desc.CullMode = D3D11_CULL_NONE;
        break;
    case CullMode::Front:
        desc.CullMode = D3D11_CULL_FRONT;
        break;
    case CullMode::Back:
        desc.CullMode = D3D11_CULL_BACK;
        break;
    }

    // Set fill mode
    switch (fillMode)
    {
    case FillMode::Wireframe:
        desc.FillMode = D3D11_FILL_WIREFRAME;
        break;
    case FillMode::Solid:
        desc.FillMode = D3D11_FILL_SOLID;
        break;
    }

    auto device = GraphicsSystem::Get()->GetDevice();
    HRESULT hr = device->CreateRasterizerState(&desc, &mRasterizerState);
    ASSERT(SUCCEEDED(hr), "RasterizerState: failed to create rasterizer state.");
}

void RasterizerState::Terminate()
{
    SafeRelease(mRasterizerState);
}

void RasterizerState::Set()
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->RSSetState(mRasterizerState);
}