#pragma once

namespace SumEngine::Graphics
{
    class RasterizerState final
    {
    public:
        enum class CullMode
        {
            None,
            Front,
            Back
        };

        enum class FillMode
        {
            Wireframe,
            Solid
        };

        static RasterizerState* Get();
        static void ClearState();

        RasterizerState() = default;
        ~RasterizerState();
        RasterizerState(const RasterizerState&) = delete;
        RasterizerState& operator=(const RasterizerState&) = delete;

        void Initialize(CullMode cullMode, FillMode fillMode);
        void Terminate();
        void Set();

    private:
        ID3D11RasterizerState* mRasterizerState = nullptr;
        static std::unique_ptr<RasterizerState> sInstance;
    };
}