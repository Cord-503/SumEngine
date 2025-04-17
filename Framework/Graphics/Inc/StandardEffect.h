#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "DirectionalLight.h"
#include "Material.h"

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
			void SetDirectionalLight(const DirectionalLight& light);	

			void DebugUI();

		private:
			struct TransformData
			{
				Math::Matrix4 wvp;//world view projection matrix
				Math::Matrix4 world;
				Math::Vector3 viewPosition;

				float padding = 0.0f; // padding to make it 16 bytes

			};

			using TransformBuffer = TypedConstantBuffer<TransformData>;
			TransformBuffer mTransformBuffer;

			using LightBuffer = TypedConstantBuffer<DirectionalLight>;
			LightBuffer mLightBuffer;

			using MaterialBuffer = TypedConstantBuffer<Material>;
			MaterialBuffer mMaterialBuffer;

			VertexShader mVertexShader;
			PixelShader mPixelShader;
			Sampler mSampler;

			const Camera* mCamera = nullptr;
			const DirectionalLight* mDirectionalLight = nullptr;

	};

}