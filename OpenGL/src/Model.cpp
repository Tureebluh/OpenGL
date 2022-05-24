#include <Model.h>

Model::Model() {

}

void Model::LoadModel(const std::string& fileName) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene) {
		printf("Filename: %s\nModel failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
}

void Model::LoadNode(aiNode* node, const aiScene* scene) {
	for (int i = 0; i < node->mNumMeshes; i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < mesh->mNumVertices; i++) {
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0]) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), {0.0f, 0.0f});
		}
		vertices.insert(vertices.end(), { -(mesh->mNormals[i].x),-(mesh->mNormals[i].y), -(mesh->mNormals[i].z) });
		
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTexture.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterial(const aiScene* scene, aiTextureType textureType, aiMaterial* material, int i) {
	if (material->GetTextureCount(textureType)) {
		aiString path;
		if (material->GetTexture(textureType, 0, &path) == AI_SUCCESS) {
			int idx = std::string(path.data).rfind("\\");
			std::string fileName = std::string(path.data).substr(idx + 1);

			std::string texturePath = std::string("Textures/") + fileName;
			textureList[i] = new Texture(texturePath.c_str());

			if (!textureList[i]->LoadTexture(true)) {
				printf("Failed to load texture. %s\n", texturePath);
				delete textureList[i];
				textureList[i] = nullptr;
			}
		}
	}
}

void Model::LoadMaterials(const aiScene* scene) {
	textureList.resize(scene->mNumMaterials);

	for (int i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_HEIGHT)) { LoadMaterial(scene, aiTextureType_HEIGHT, material, i); }
		if (material->GetTextureCount(aiTextureType_DISPLACEMENT)) { LoadMaterial(scene, aiTextureType_DISPLACEMENT, material, i); }
		if (material->GetTextureCount(aiTextureType_BASE_COLOR)) { LoadMaterial(scene, aiTextureType_BASE_COLOR, material, i); }
		if (material->GetTextureCount(aiTextureType_DIFFUSE)) { LoadMaterial(scene, aiTextureType_DIFFUSE, material, i); }
		if (material->GetTextureCount(aiTextureType_AMBIENT)) { LoadMaterial(scene, aiTextureType_AMBIENT, material, i); }
		if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION)) { LoadMaterial(scene, aiTextureType_AMBIENT_OCCLUSION, material, i); }
		if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS)) { LoadMaterial(scene, aiTextureType_DIFFUSE_ROUGHNESS, material, i); }
		if (material->GetTextureCount(aiTextureType_CLEARCOAT)) { LoadMaterial(scene, aiTextureType_CLEARCOAT, material, i); }
		if (material->GetTextureCount(aiTextureType_LIGHTMAP)) { LoadMaterial(scene, aiTextureType_LIGHTMAP, material, i); }
		if (material->GetTextureCount(aiTextureType_EMISSIVE)) { LoadMaterial(scene, aiTextureType_EMISSIVE, material, i); }
		if (material->GetTextureCount(aiTextureType_REFLECTION)) { LoadMaterial(scene, aiTextureType_REFLECTION, material, i); }
		if (material->GetTextureCount(aiTextureType_SPECULAR)) { LoadMaterial(scene, aiTextureType_SPECULAR, material, i); }
		if (material->GetTextureCount(aiTextureType_OPACITY)) { LoadMaterial(scene, aiTextureType_OPACITY, material, i); }
		if (material->GetTextureCount(aiTextureType_NONE)) { LoadMaterial(scene, aiTextureType_NONE, material, i); }
		if (material->GetTextureCount(aiTextureType_UNKNOWN)) { LoadMaterial(scene, aiTextureType_UNKNOWN, material, i); }

		if (!textureList[i]) {
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTexture(true);
		}
	}
}

void Model::RenderModel() {
	for (int i = 0; i < meshList.size(); i++) {
		unsigned int materialIndex = meshToTexture[i];

		if (materialIndex < textureList.size() && textureList[materialIndex]) {
			textureList[materialIndex]->UseTexture();
		}
		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel() {
	for (int i = 0; i < meshList.size(); i++) {
		if (meshList[i]) {
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}
	for (int i = 0; i < textureList.size(); i++) {
		if (textureList[i]) {
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

Model::~Model() {
	ClearModel();
}