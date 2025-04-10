#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace SumEngine::Graphics
{
	class Camera;
	class RenderObject;

	class StandardEffect final
	{
		public:
			void Initialize(const std::filesystem::path& path);
			void Terminate();

			void Begin();
			void End();

			void Render(const RenderObject& renderObject);

			void SetCamera(const Camera& camera);

			void DebugUI();

		private:
			ConstantBuffer mConstantBuffer;
			VertexShader mVertexShader;
			PixelShader mPixelShader;
			Sampler mSampler;

			const Camera* mCamera = nullptr;

	};

}