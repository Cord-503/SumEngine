#pragma once
#include "MeshBuffer.h"
#include "Transform.h"
#include "TextureCashe.h"
#include "Material.h"

namespace SumEngine::Graphics
{
	class RenderObject
	{
		public:
			void Terminate();

			MeshBuffer meshBuffer;
			Transform transform;

			Material material;
			TextureId diffuseId;
	};
}