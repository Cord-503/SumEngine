#pragma once

namespace SumEngine::Graphics
{
	class BlendState final
	{
	public:
		static void ClearState();
		static BlendState* Get();

		enum class Mode
		{
			Opaque,
			AlphaBlend,
			AlphaPremultiplied,
			Additive
		};

		BlendState() = default;
		~BlendState();
		BlendState(const BlendState&) = delete;
		BlendState& operator=(const BlendState&) = delete;

		void Initialize(Mode mode);
		void Terminate();
		void Set();

	private:
		ID3D11BlendState* mBlendState = nullptr;
		static std::unique_ptr<BlendState> sInstance;
	};
}