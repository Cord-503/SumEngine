#include "Precompiled.h"
#include "ModelIO.h"
#include "Model.h"


using namespace SumEngine;
using namespace SumEngine::Graphics;

void ModelIO::SaveModel(std::filesystem::path filePath, const Model& model)
{
	if (model.meshData.empty())
	{
		return;
	}
	filePath.replace_extension("model");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
	{
		return;
	}

	const uint32_t meshCount = static_cast<uint32_t>(model.meshData.size());
	fprintf_s(file, "MeshCount: %d\n", meshCount);

	for (uint32_t m = 0; m < meshCount; ++m)
	{
		const Model::MeshData& meshData = model.meshData[m];
		fprintf_s(file, "MaterialIndex: %d\n", meshData.materialIndex);

		const Mesh& mesh = meshData.mesh;
		const uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
		fprintf_s(file, "VertexCount: %d\n", vertexCount);
		for (const Vertex& v : mesh.vertices)
		{
			fprintf_s(file, "%f %f %f %f %f %f %f %f %f %f %f\n",
				v.position.x, v.position.y, v.position.z,
				v.normal.x, v.normal.y, v.normal.z,
				v.tangent.x, v.tangent.y, v.tangent.z,
				v.uvCoord.x, v.uvCoord.y);
		}

		const uint32_t indexCount = static_cast<uint32_t>(mesh.indices.size());
		fprintf_s(file, "IndexCount: %d\n", indexCount);

		for (uint32_t i = 2; i < indexCount; i += 3)
		{
			fprintf_s(file, "%d %d %d\n", mesh.indices[i - 2], mesh.indices[i - 1], mesh.indices[i]);
		}
	}
	fclose(file);

}

void ModelIO::LoadModel(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("model");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
	{
		return;
	}

	uint32_t meshCount = 0;
	fscanf_s(file, "MeshCount: %d\n", &meshCount);
	model.meshData.resize(meshCount);

	for (uint32_t m = 0; m < meshCount; ++m)
	{
		Model::MeshData& meshData = model.meshData[m];
		fscanf_s(file, "MaterialIndex: %d\n", &meshData.materialIndex);

		Mesh& mesh = meshData.mesh;
		uint32_t vertexCount = 0;
		fscanf_s(file, "VertexCount: %d\n", &vertexCount);

		mesh.vertices.resize(vertexCount);
		for (Vertex& v : mesh.vertices)
		{
			fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f\n",
				&v.position.x, &v.position.y, &v.position.z,
				&v.normal.x, &v.normal.y, &v.normal.z,
				&v.tangent.x, &v.tangent.y, &v.tangent.z,
				&v.uvCoord.x, &v.uvCoord.y);
		}

		uint32_t indexCount =0;
		fscanf_s(file, "IndexCount: %d\n", &indexCount);
		mesh.indices.resize(indexCount);
		for (uint32_t i = 2; i < indexCount; i += 3)
		{
			fscanf_s(file, "%d %d %d\n", &mesh.indices[i - 2], &mesh.indices[i - 1], &mesh.indices[i]);
		}
	}
	fclose(file);
}

void ModelIO::SaveMaterial(std::filesystem::path filePath, const Model& model)
{
	if (model.materialData.empty())
	{
		return;
	}
	filePath.replace_extension("material");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
	{
		return;
	}

	uint32_t materialCount = static_cast<uint32_t>(model.materialData.size());
	fprintf_s(file, "MaterialCount: %d\n", materialCount);
	for (const Model::MaterialData& md : model.materialData)
	{
		const Material& m = md.material;
		fprintf_s(file, "%f %f %f %f\n", m.emissive.r, m.emissive.g, m.emissive.b, m.emissive.a);
		fprintf_s(file, "%f %f %f %f\n", m.ambient.r, m.ambient.g, m.ambient.b, m.ambient.a);
		fprintf_s(file, "%f %f %f %f\n", m.diffuse.r, m.diffuse.g, m.diffuse.b, m.diffuse.a);
		fprintf_s(file, "%f %f %f %f\n", m.specular.r, m.specular.g, m.specular.b, m.specular.a);
		fprintf_s(file, "Shininess: %f\n", m.shininess);

		fprintf_s(file, "%s\n", md.diffuseMapName.empty() ? "<none>" : md.diffuseMapName.c_str());
		fprintf_s(file, "%s\n", md.normalMapName.empty() ? "<none>" : md.normalMapName.c_str());
		fprintf_s(file, "%s\n", md.specMapName.empty() ? "<none>" : md.specMapName.c_str());
		fprintf_s(file, "%s\n", md.bumpMapName.empty() ? "<none>" : md.bumpMapName.c_str());
	}
	fclose(file);
}

void ModelIO::LoadMaterial(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("material");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
	{
		return;
	}

	auto TryReadTextureName = [&](auto& fileName)
	{
		char buffer[MAX_PATH];
		fscanf_s(file, "%s\n", &buffer, (uint32_t)sizeof(buffer));
		if (strcmp(buffer, "<none>") != 0)
		{
			fileName = filePath.replace_filename(buffer).string();
		}
	};

	uint32_t materialCount = 0;
	fscanf_s(file, "MaterialCount: %d\n", &materialCount);
	model.materialData.resize(materialCount);
	for (Model::MaterialData& md : model.materialData)
	{
		Material& m = md.material;
		fscanf_s(file, "%f %f %f %f\n", &m.emissive.r, &m.emissive.g, &m.emissive.b, &m.emissive.a);
		fscanf_s(file, "%f %f %f %f\n", &m.ambient.r, &m.ambient.g, &m.ambient.b, &m.ambient.a);
		fscanf_s(file, "%f %f %f %f\n", &m.diffuse.r, &m.diffuse.g, &m.diffuse.b, &m.diffuse.a);
		fscanf_s(file, "%f %f %f %f\n", &m.specular.r, &m.specular.g, &m.specular.b, &m.specular.a);
		fscanf_s(file, "Shininess: %f\n", &m.shininess);

		TryReadTextureName(md.diffuseMapName);
		TryReadTextureName(md.normalMapName);
		TryReadTextureName(md.specMapName);
		TryReadTextureName(md.bumpMapName);
		
	}
	fclose(file);
}
