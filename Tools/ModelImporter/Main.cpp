#include "SumEngine/Inc/SumEngine.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <cstdio>

using namespace SumEngine;
using namespace SumEngine::Graphics;
using namespace SumEngine::Math;

struct Arguments
{
	std::filesystem::path inputFileName;
	std::filesystem::path outputFileName;
	float scale = 1.0f;
};

Vector3 ToVector3(const aiVector3D& v)
{
	return {
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z)
	};
}

Quaternion ToQuaternion(const aiQuaternion& q)
{
	return {
		static_cast<float>(q.x),
		static_cast<float>(q.y),
		static_cast<float>(q.z),
		static_cast<float>(q.w)
	};
}

Vector2 ToTexCoord(const aiVector3D& v)
{
	return {
		static_cast<float>(v.x),
		static_cast<float>(v.y)
	};
}

Color ToColor(const aiColor3D& c)
{
	return {
		static_cast<float>(c.r),
		static_cast<float>(c.g),
		static_cast<float>(c.b),
		1.0f
	};
}

std::string FindTexture(const aiScene* scene, const aiMaterial* aiMaterial, aiTextureType textureType, const Arguments& args, const std::string& suffix, uint32_t materialIndex)
{
	// next week
	return "";
}

std::optional<Arguments> ParserArgs(int argc, char* argv[])
{
	if (argc < 3)
	{
		return std::nullopt;
	}

	Arguments args;
	args.inputFileName = argv[argc - 2];
	args.outputFileName = argv[argc - 1];



	for (int i = 1; i + 2 < argc; ++i)
	{
		if (strcmp(argv[i], "-scale") == 0)
		{
			args.scale = atof(argv[i + 1]);
			++i;
		}
	}

	return args;

}




int main(int argc, char* argv[])
{
	const auto argOpt = ParserArgs(argc, argv);
	if (!argOpt.has_value())
	{
		printf("Not enough arguments, import fbx failed\n");
		return -1;
	}


	printf("Begin Import\n");
	const Arguments& args = argOpt.value();

	// use assimp
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;

	const aiScene* scene = importer.ReadFile(args.inputFileName.u8string().c_str(), flags);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}

	printf("Importing %s...\n", args.inputFileName.u8string().c_str());

	Model model;
	if (scene->HasMeshes())
	{
		printf("Reading Mesh Data...\n");
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const auto& aiMesh = scene->mMeshes[meshIndex];

			if (aiMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
			{
				continue;
			}

			const uint32_t numVertices = aiMesh->mNumVertices;
			const uint32_t numFaces = aiMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			Model::MeshData& meshData = model.meshData.emplace_back();

			printf("Reading Material Index...\n");
			meshData.materialIndex = aiMesh->mMaterialIndex;

			printf("Reading Vertices...\n");
			Mesh& mesh = meshData.mesh;
			mesh.vertices.resize(numVertices);

			const aiVector3D* positions = aiMesh->mVertices;
			const aiVector3D* normals = aiMesh->mNormals;
			const aiVector3D* tangents = aiMesh->HasTangentsAndBitangents() ? aiMesh->mTangents : nullptr;
			const aiVector3D* texCoords = aiMesh->HasTextureCoords(0) ? aiMesh->mTextureCoords[0] : nullptr;

			for (uint32_t v = 0; v < numVertices; ++v)
			{
				Vertex& vertex = mesh.vertices[v];
				vertex.position = ToVector3(positions[v]) * args.scale;
				vertex.normal = ToVector3(normals[v]);
				vertex.tangent = tangents ? ToVector3(tangents[v]) : Vector3::Zero;
				vertex.uvCoord = texCoords ? ToTexCoord(texCoords[v]) : Vector2::Zero;

			}

			printf("Reading indices...\n");
			mesh.indices.reserve(numIndices);
			const auto& aiFaces = aiMesh->mFaces;
			for (uint32_t f = 0; f < numFaces; ++f)
			{
				const auto& aiFace = aiFaces[f];
				for (uint32_t i = 0; i < 3; ++i)
				{
					mesh.indices.push_back(aiFace.mIndices[i]);
				}
			}



		}

	}

	if (scene->HasMaterials()) 
	{
		printf("Reading meterial data..\n");

		const uint32_t numMaterials = scene->mNumMaterials;
		model.materialData.resize(numMaterials);

		for (uint32_t materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
		{
			const auto aiMaterial = scene->mMaterials[materialIndex];
			aiColor3D ambient, diffuse, emissive, specular;
			ai_real specularPower = 1.0f;
			aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
			aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
			aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
			aiMaterial->Get(AI_MATKEY_SHININESS, specularPower);

			Model::MaterialData& materialData = model.materialData[materialIndex];
			materialData.material.ambient = ToColor(ambient);
			materialData.material.diffuse = ToColor(diffuse);
			materialData.material.emissive = ToColor(emissive);
			materialData.material.specular = ToColor(specular);
			materialData.material.shininess = static_cast<float>(specularPower);

			materialData.diffuseMapName = FindTexture(scene, aiMaterial, aiTextureType_DIFFUSE, args, "_diff", materialIndex);
			materialData.normalMapName = FindTexture(scene, aiMaterial, aiTextureType_NORMALS, args, "_norm", materialIndex);
			materialData.specMapName = FindTexture(scene, aiMaterial, aiTextureType_SPECULAR, args, "_spec", materialIndex);
			materialData.bumpMapName = FindTexture(scene, aiMaterial, aiTextureType_DISPLACEMENT, args, "_bump", materialIndex);
		}

	}

	// next week



	return 0;
}