// Reference: https://learnopengl.com
#pragma once

#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <math.h>
#include <assert.h>
#pragma comment(lib, "glew32.lib")
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "LoadTexture.h"

namespace Layout_h {
	static const int NameBase = 1000000;
	static int NameGen = 100;
	std::map<int, void*> NamedLayout;
}

class SkyBox {
private:
	float X, Y, Z;
	float sX, sY, sZ;
	unsigned int* Tex;
	bool AnimSky;
	float fRot;
public:
	SkyBox(float x, float y, float z, float ScaleX, float ScaleY, float ScaleZ, unsigned int *tex);
	~SkyBox() { if (Tex)free(Tex); };
	void SetAnimSky();
	void DrawSky();
	void SetTex(unsigned int* tex) { if (tex) for (int i = 0; i < 6; i++) Tex[i] = tex[i]; }
	void SetX(const float x) { X = x; }
	void SetY(const float y) { Y = y; }
	void SetZ(const float z) { Z = z; }
	float getX() const { return X; }
	float getY() const { return Y; }
	float getZ() const { return Z; }
	void SetPos(const float x, const float y, const float z) { X = x; Y = y; Z = z; }
};

SkyBox::SkyBox(float x, float y, float z, float ScaleX, float ScaleY, float ScaleZ, unsigned int *tex) {
	X = x;
	Y = y;
	Z = z;
	sX = ScaleX;
	sY = ScaleY;
	sZ = ScaleZ;
	Tex = (unsigned int *)malloc(sizeof(unsigned int) * 6);
	if (tex && Tex)
		for (int i = 0; i < 6; i++)
			Tex[i] = tex[i];
	AnimSky = false;
	fRot = 0;
}

void SkyBox::SetAnimSky() {
	AnimSky = !AnimSky;
}

void SkyBox::DrawSky() {

	glPushMatrix(); {
	
		glTranslatef(X, Y, Z);
		glScalef(sX, sY, sZ);

		glPushMatrix(); {
			glDepthMask(GL_FALSE);
			assert(Tex != NULL);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[4]);
			glBegin(GL_QUADS);
			// +z
			glTexCoord2i(1, 1); glVertex3i(-1, 1, 1);
			glTexCoord2i(1, 0); glVertex3i(-1, -1, 1);
			glTexCoord2i(0, 0); glVertex3i(1, -1, 1);
			glTexCoord2i(0, 1); glVertex3i(1, 1, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[5]);
			glBegin(GL_QUADS);
			// -z
			glTexCoord2i(1, 1); glVertex3i(1, 1, -1);
			glTexCoord2i(1, 0); glVertex3i(1, -1, -1);
			glTexCoord2i(0, 0); glVertex3i(-1, -1, -1);
			glTexCoord2i(0, 1); glVertex3i(-1, 1, -1);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[0]);
			glBegin(GL_QUADS);
			// +x
			glTexCoord2i(1, 1); glVertex3i(1, 1, 1);
			glTexCoord2i(1, 0); glVertex3i(1, -1, 1);
			glTexCoord2i(0, 0); glVertex3i(1, -1, -1);
			glTexCoord2i(0, 1); glVertex3i(1, 1, -1);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[1]);
			glBegin(GL_QUADS);
			// -x
			glTexCoord2i(1, 1); glVertex3i(-1, 1, -1);
			glTexCoord2i(1, 0); glVertex3i(-1, -1, -1);
			glTexCoord2i(0, 0); glVertex3i(-1, -1, 1);
			glTexCoord2i(0, 1); glVertex3i(-1, 1, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[2]);
			glBegin(GL_QUADS);
			// +y
			glTexCoord2i(1, 1); glVertex3i(1, 1, 1);
			glTexCoord2i(1, 0); glVertex3i(1, 1, -1);
			glTexCoord2i(0, 0); glVertex3i(-1, 1, -1);
			glTexCoord2i(0, 1); glVertex3i(-1, 1, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[3]);
			glBegin(GL_QUADS);
			// -y
			glTexCoord2i(1, 1); glVertex3i(1, -1, -1);
			glTexCoord2i(1, 0); glVertex3i(1, -1, 1);
			glTexCoord2i(0, 0); glVertex3i(-1, -1, 1);
			glTexCoord2i(0, 1); glVertex3i(-1, -1, -1);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glDepthMask(GL_TRUE);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

class Terrain {
private:
	float X, Y, Z;
	float sX, sY, sZ;
	unsigned int* Tex;
public:
	Terrain(float x, float y, float z, float ScaleX, float ScaleY, float ScaleZ, unsigned int* tex);
	~Terrain() { if (Tex)free(Tex); }
	void DrawTerrain(int rept = 5);
	void SetTex(unsigned int* tex) { if (tex) for (int i = 0; i < 6; i++) Tex[i] = tex[i]; }
	void SetX(const float x) { X = x; }
	void SetY(const float y) { Y = y; }
	void SetZ(const float z) { Z = z; }
	float getX() const { return X; }
	float getY() const { return Y; }
	float getZ() const { return Z; }
	void SetPos(const float x, const float y, const float z) { X = x; Y = y; Z = z; }
};

Terrain::Terrain(float x, float y, float z, float ScaleX, float ScaleY, float ScaleZ, unsigned int* tex) {
	X = x;
	Y = y;
	Z = z;
	sX = ScaleX;
	sY = ScaleY;
	sZ = ScaleZ;
	Tex = (unsigned int*)malloc(sizeof(unsigned int) * 1);
	if (tex && Tex)
		for (int i = 0; i < 1; i++)
			Tex[i] = tex[i];
}

void Terrain::DrawTerrain(int rept) {
	glPushMatrix(); {

		glTranslatef(X, Y, Z);
		glScalef(sX, sY, sZ);

		glPushMatrix(); {
			assert(Tex != NULL);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[0]);
			glBegin(GL_QUADS);
			// +z
			glTexCoord2i(1, 1); glVertex3i(-1, 1, 1);
			glTexCoord2i(1, 0); glVertex3i(-1, -1, 1);
			glTexCoord2i(0, 0); glVertex3i(1, -1, 1);
			glTexCoord2i(0, 1); glVertex3i(1, 1, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[0]);
			glBegin(GL_QUADS);
			// -z
			glTexCoord2i(1, 1); glVertex3i(1, 1, -1);
			glTexCoord2i(1, 0); glVertex3i(1, -1, -1);
			glTexCoord2i(0, 0); glVertex3i(-1, -1, -1);
			glTexCoord2i(0, 1); glVertex3i(-1, 1, -1);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[0]);
			glBegin(GL_QUADS);
			// +x
			glTexCoord2i(1, 1); glVertex3i(1, 1, 1);
			glTexCoord2i(1, 0); glVertex3i(1, -1, 1);
			glTexCoord2i(0, 0); glVertex3i(1, -1, -1);
			glTexCoord2i(0, 1); glVertex3i(1, 1, -1);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[0]);
			glBegin(GL_QUADS);
			// -x
			glTexCoord2i(1, 1); glVertex3i(-1, 1, -1);
			glTexCoord2i(1, 0); glVertex3i(-1, -1, -1);
			glTexCoord2i(0, 0); glVertex3i(-1, -1, 1);
			glTexCoord2i(0, 1); glVertex3i(-1, 1, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			for (int i = 0; i < rept; i++) {
				for (int j = 0; j < rept; j++) {
					//if ((i > 0 || i < 4))continue;
					glPushMatrix();
					glTranslatef(-1.0 + 1.0 / rept + i * 2.0 / rept, 0, -1.0 + 1.0 / rept + j * 2.0 / rept);
					glScalef(0.2, 1, 0.2);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, Tex[0]);
					glBegin(GL_QUADS);
					// +y
					glTexCoord2i(1, 1); glVertex3i(1, 1, 1);
					glTexCoord2i(1, 0); glVertex3i(1, 1, -1);
					glTexCoord2i(0, 0); glVertex3i(-1, 1, -1);
					glTexCoord2i(0, 1); glVertex3i(-1, 1, 1);
					glEnd();
					glDisable(GL_TEXTURE_2D);
					glPopMatrix();
				}
			}

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Tex[0]);
			glBegin(GL_QUADS);
			// -y
			glTexCoord2i(1, 1); glVertex3i(1, -1, -1);
			glTexCoord2i(1, 0); glVertex3i(1, -1, 1);
			glTexCoord2i(0, 0); glVertex3i(-1, -1, 1);
			glTexCoord2i(0, 1); glVertex3i(-1, -1, -1);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

class Layout {
protected:
	bool ShdrInit;
	bool Init;

public:
	GLuint VAO, VBO, EBO;
	GLuint ShdrHdl;

	Layout();
	virtual ~Layout();
	virtual GLuint* getVtx() { return Vertices; }
	virtual GLuint* getInd() { return Indices; }
	virtual void setVtx(GLuint* v) = 0;
	virtual void setInd(GLuint* d) = 0;
	virtual void setShdr(GLuint hdl) = 0;
	virtual void setInit() = 0;
	virtual void Draw() = 0;

	// Reference: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/mesh.h
	#define MAX_BONE_INFLUENCE 4
	struct _SmplVertex {
		// position
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
	};
	typedef struct _SmplVertex SmplVertex;
	struct _VertexExtnd {
		// tangent
		glm::vec3 Tangent;
		// bitangent
		glm::vec3 Bitangent;
		//bone indexes which will influence this vertex
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];
	};
	typedef struct _VertexExtnd VertexExtnd;
	struct _HdlTexture {
		unsigned int Hdl;
		std::string Type;
		std::string Path;
	};
	typedef struct _HdlTexture HdlTexture;

private:
	GLuint* Vertices, * Indices;

};

Layout::Layout()
{
	VAO = VBO = EBO = 0;
	Vertices = Indices = NULL;
	ShdrInit = false;
}

Layout::~Layout()
{
	if (Vertices) { free(Vertices); }
	if (Indices) { free(Indices); }
}

class Mesh : public Layout {
public:
	Mesh(std::vector<Layout::SmplVertex> vtx, std::vector<unsigned int> indx, std::vector<Layout::HdlTexture> textures);
	~Mesh();
	void setShdr(GLuint hdl) { ShdrHdl = hdl; ShdrInit = true; }
	void Draw();
	void Draw(Shader& shdr);

	std::vector<SmplVertex> Vtx;
	//std::vector<VertexExtnd> VtxExtnd; //not used
	std::vector<unsigned int> Indx;
	std::vector<HdlTexture> Textures;

	// not used
	virtual void setVtx(GLuint* v) {};
	virtual void setInd(GLuint* d) {};
	virtual void setInit() {};

private:

};

// Reference: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/mesh.h
Mesh::Mesh(std::vector<SmplVertex> vtx, std::vector<unsigned int> indx, std::vector<HdlTexture> textures) {
	Vtx = vtx;
	Indx = indx;
	Textures = textures;

	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, Vtx.size() * sizeof(_SmplVertex), &Vtx[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indx.size() * sizeof(unsigned int), &Indx[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_SmplVertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(_SmplVertex), (void*)offsetof(_SmplVertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(_SmplVertex), (void*)offsetof(_SmplVertex, TexCoords));

	glBindVertexArray(0);
	Init = true;
}

Mesh::~Mesh() {}

// Reference: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/mesh.h
void Mesh::Draw() {
	assert(ShdrInit);
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = Textures[i].Type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to string
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to string
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to string

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(ShdrHdl, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, Textures[i].Hdl);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Indx.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Draw(Shader& shdr) {
	GLuint t = ShdrHdl;
	ShdrHdl = shdr.getHdl();
	Draw();
	ShdrHdl = t;
}


// Reference: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h
class Model : public Layout {
private:
	std::vector<HdlTexture> TextureLoaded;
	std::vector<Mesh> Meshes;
	std::string Dir;

public:
	Model();
	~Model();
	void setInit() { Init = true; }
	void setShdr(GLuint hdl);
	void Draw();
	void Draw(Shader& shdr);
	std::vector<Mesh>& getMeshes() { return Meshes; }
	std::vector<HdlTexture>& getTxtr() { return TextureLoaded; }
	std::string& getDir() { return Dir; }
};

Model::Model() {
	ShdrInit = false;
	Init = false;
}

Model::~Model(){}

void Model::setShdr(GLuint hdl) {
	for (size_t i = 0; i < Meshes.size(); i++) {
		Meshes[i].setShdr(hdl);
	}
	ShdrInit = true;
}

void Model::Draw() {
	assert(Init);
	assert(ShdrInit);
	for (size_t i = 0; i < Meshes.size(); i++) {
		Meshes[i].Draw();
	}
}

void Model::Draw(Shader& shdr) {
	assert(Init);
	for (size_t i = 0; i < Meshes.size(); i++) {
		Meshes[i].Draw(shdr);
	}
}

class ObjHandler {
private:
	Assimp::Importer Imptr;
	void ProcessNode(aiNode* Node, const aiScene* Sc, Model* Mdl);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* Mdl);

public:
	ObjHandler();
	~ObjHandler();
	void ObjLoad(const char* filePath, Model* Mdl);
	bool ObjSave();
	std::vector<Layout::HdlTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, Model* Mdl);
	unsigned int TextureFromFile(const char* path, const std::string& dir);
};

ObjHandler::ObjHandler() {}

ObjHandler::~ObjHandler() {}

// Reference: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h
void ObjHandler::ObjLoad(const char* filePath, Model* Mdl) {
	const aiScene* sc = Imptr.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	assert(sc && !(sc->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && sc->mRootNode);
	std::string strPath(filePath);
	Mdl->getDir() = strPath.substr(0, strPath.find_last_of('/'));

	ProcessNode(sc->mRootNode, sc, Mdl);
	Mdl->setInit();
}

bool ObjHandler::ObjSave() {
	// not implemented
	return false;
}

// Reference: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h
void ObjHandler::ProcessNode(aiNode* Node, const aiScene* Sc, Model* Mdl) {
	for (unsigned int i = 0; i < Node->mNumMeshes; i++) {
		aiMesh* mesh = Sc->mMeshes[Node->mMeshes[i]];
		Mdl->getMeshes().push_back(ProcessMesh(mesh, Sc, Mdl));
	}
	for (unsigned int i = 0; i < Node->mNumChildren; i++) {
		ProcessNode(Node->mChildren[i], Sc, Mdl);
	}
}

// Reference: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h
Mesh ObjHandler::ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* Mdl) {
	// data to fill
	std::vector<Layout::SmplVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Layout::HdlTexture> textures;

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Layout::SmplVertex vertex;
		glm::vec3 vect; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vect.x = mesh->mVertices[i].x;
		vect.y = mesh->mVertices[i].y;
		vect.z = mesh->mVertices[i].z;
		vertex.Position = vect;
		// normals
		if (mesh->HasNormals())
		{
			vect.x = mesh->mNormals[i].x;
			vect.y = mesh->mNormals[i].y;
			vect.z = mesh->mNormals[i].z;
			vertex.Normal = vect;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
			//not used
			//// tangent
			//vect.x = mesh->mTangents[i].x;
			//vect.y = mesh->mTangents[i].y;
			//vect.z = mesh->mTangents[i].z;
			//vertex.Tangent = vect;
			//// bitangent
			//vect.x = mesh->mBitangents[i].x;
			//vect.y = mesh->mBitangents[i].y;
			//vect.z = mesh->mBitangents[i].z;
			//vertex.Bitangent = vect;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Layout::HdlTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", Mdl);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Layout::HdlTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", Mdl);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Layout::HdlTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", Mdl);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Layout::HdlTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", Mdl);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

// Reference: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h
std::vector<Layout::HdlTexture> ObjHandler::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, Model* Mdl) {
	std::vector<Layout::HdlTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < Mdl->getTxtr().size(); j++)
		{
			if (std::strcmp(Mdl->getTxtr()[j].Path.data(), str.C_Str()) == 0)
			{
				textures.push_back(Mdl->getTxtr()[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Layout::HdlTexture texture;
			texture.Hdl = TextureFromFile(str.C_Str(), Mdl->getDir());
			texture.Type = typeName;
			texture.Path = str.C_Str();
			textures.push_back(texture);
			Mdl->getTxtr().push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
		}
	}
	return textures;
};

unsigned int ObjHandler::TextureFromFile(const char* path, const std::string& dir) {
	unsigned int TexHdl;
	glGenTextures(1, &TexHdl);
	TexLoad(std::string(dir + '/' + std::string(path)).c_str(), &TexHdl, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	return TexHdl;
}

class OperableLayout {
public:
	OperableLayout();
	virtual ~OperableLayout() {}

	void AddTrans(float x, float y, float z) { Trans[0] += x, Trans[1] += y, Trans[2] += z; }
	GLfloat* getTrans() { return &Trans[0]; }
	void MultScale(float s) { Scale *= s; }
	void AddRot(float x, float y, float z) { Rot[0] += x, Rot[1] += y, Rot[2] += z; }
	void SetDisplayHide() { Display = !Display; }

	virtual void SetObj(Layout* obj);
	virtual void SetName();
	virtual void LeftMouseHit();
	virtual void RightMouseHit();
	virtual void Draw();

	Layout* Obj;

protected:
	GLfloat Trans[3];
	GLfloat Scale;
	GLfloat Rot[3];
	bool Display;
	bool NameInit;
	GLint Name;
};

OperableLayout::OperableLayout() {
	Trans[0] = Trans[1] = Trans[2] = 0;
	Scale = 1;
	Rot[0] = Rot[1] = Rot[2] = 0;
	Obj = NULL;
	Display = true;
	NameInit = false;
	GLint Name = 0;
}

void OperableLayout::SetObj(Layout* obj) {
	Obj = obj;
	SetName();
}

void OperableLayout::SetName() {
	if (!NameInit) {
		Name = Layout_h::NameGen++;
		NameInit = true;
		Layout_h::NamedLayout[Name] = (void*)this;
		//printf("NamedLayout[Name] = %016lx\n", Layout_h::NamedLayout[Name]);
	}
}

void OperableLayout::Draw() {
	assert(Obj != NULL);
	if (!Display) return;

	glPushName(Layout_h::NameBase);
	glPushMatrix(); {
		glPushName(Name);
		glTranslatef(Trans[0], Trans[1], Trans[2]);
		glScalef(Scale, Scale, Scale);
		glRotatef(Rot[0], 1.0, 0, 0);
		glRotatef(Rot[1], 0, 1.0, 0);
		glRotatef(Rot[2], 0, 0, 1.0);
		Obj->Draw();
		glPopName();
	}
	glPopMatrix();
	glPopName();
}

/*interfaces for GameObject*/
void OperableLayout::LeftMouseHit() {

}
void OperableLayout::RightMouseHit() {
	
}

class SolidTeapot : public OperableLayout
{
public:
	SolidTeapot();
	~SolidTeapot();
	void Draw();

private:

};

SolidTeapot::SolidTeapot()
{
}

SolidTeapot::~SolidTeapot()
{
}

void SolidTeapot::Draw() {
	if (!Display) return;

	glPushName(Layout_h::NameBase);
	glPushMatrix(); {
		if(NameInit)
			glPushName(Name);
		glTranslatef(Trans[0], Trans[1], Trans[2]);
		glScalef(Scale, Scale, Scale);
		glRotatef(Rot[0], 1.0, 0, 0);
		glRotatef(Rot[1], 0, 1.0, 0);
		glRotatef(Rot[2], 0, 0, 1.0);
		glutSolidTeapot(0.5f);
		if (NameInit)
			glPopName();
	}
	glPopMatrix();
	glPopName();
}

#endif