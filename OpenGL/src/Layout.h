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
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glut.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "LoadTexture.h"

#define max(a, b) ((a) > (b)? (a): (b))
#define min(a, b) ((a) > (b)? (b): (a))
#define PI 3.1415926535

namespace Layout_h {
	static const int NameBase = 1000000;
	static int NameGen = 100;
	std::map<int, void*> NamedLayout;//from name to OperableLayout instance
	std::map<int, void*> AddedLayout;//from name to PlayAddedLayout instance
	std::map<int, int> NameInGUI;//from name to index in ObjList in GUI
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	constexpr unsigned long long LIGHTSOURCE = 0x1;
	constexpr unsigned long long GENERALPRISMGENERATOR = 0x2;
	std::vector<unsigned int> Tex4NonShdrObj;
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
	void SetTex(unsigned int* tex) { if (tex) for (int i = 0; i < 1; i++) Tex[i] = tex[i]; }
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

	// not used
	virtual void setVtx(GLuint* v) {};
	virtual void setInd(GLuint* d) {};
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
	bool Flip;
	void ProcessNode(aiNode* Node, const aiScene* Sc, Model* Mdl);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* Mdl);

public:
	ObjHandler();
	~ObjHandler();
	void ObjLoad(const char* filePath, Model* Mdl, bool flip = true);
	bool ObjSave();
	std::vector<Layout::HdlTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, Model* Mdl);
	unsigned int TextureFromFile(const char* path, const std::string& dir);
};

ObjHandler::ObjHandler() {}

ObjHandler::~ObjHandler() {}

// Reference: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h
void ObjHandler::ObjLoad(const char* filePath, Model* Mdl, bool flip) {
	Flip = flip;
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
	TexLoad(std::string(dir + '/' + std::string(path)).c_str(), &TexHdl, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, Flip);
	return TexHdl;
}

class OperableLayout {
public:
	OperableLayout();
	virtual ~OperableLayout() {}

	void AddTrans(float x, float y, float z) { Trans[0] += x, Trans[1] += y, Trans[2] += z; }
	GLfloat* getTrans() { return &Trans[0]; }
	void MultScale(float s) { Scale *= s; }
	void SetCompulsoryScale(float s) { Scale *= s; CompulsoryScale = true; }
	bool CheckCompulsoryScale(float* s) { if (CompulsoryScale && s)*s = Scale; return CompulsoryScale; }
	void AddRot(float x, float y, float z) { Rot[0] += x, Rot[1] += y, Rot[2] += z; }
	void SetDisplayHide() { Display = !Display; }
	bool NotHidden() const { return Display; }
	bool IsObjFromFile() const { return ObjFromFile; }
	bool IsLightSource() const { return Shine; }
	void SetLightSource() { Shine = true; }
	bool IsGeneralPrismCreator() const { return IsPrismCreator; }
	void SetSpecialOptions(unsigned long long x);
	bool SetSpareTex(Layout* newobj);
	float IsLit() const { return Lit; }
	GLuint getName() const { return Name; }

	virtual void SetObj(Layout* obj);
	virtual void SetName();
	virtual void LeftMouseHit();
	virtual void RightMouseHit();
	virtual void Draw();
	virtual void glutDraw() {};//for subclasses to add objects from glut lib, also providng interface for PlayerAddedLayout
	virtual void OperationShowedGUI();//interface for GUI.h
	float getPos(char p) const { p = (p | 0x20) - 'x'; return Trans[max(0, min(p, 2))]; }

	Layout* Obj;
	Layout* texSpare;

protected:
	GLfloat Trans[3];
	GLfloat Scale;
	GLfloat ExtraScales[3];
	GLfloat Rot[3];
	GLfloat rotvX, rotvY, rotvZ;
	bool ObjFromFile;
	bool Display;
	bool NameInit;
	bool Shine;
	bool Lit;
	bool CompulsoryScale;
	bool IsPrismCreator;
	GLint Name;
};

OperableLayout::OperableLayout() {
	Trans[0] = Trans[1] = Trans[2] = 0;
	Scale = 1;
	ExtraScales[0] = ExtraScales[1] = ExtraScales[2] = 1;
	Rot[0] = Rot[1] = Rot[2] = 0;
	rotvX = rotvY = rotvZ = 0;
	Obj = NULL;
	ObjFromFile = false;
	Display = true;
	NameInit = false;
	Shine = false;
	Lit = true;
	texSpare = NULL;
	CompulsoryScale = true;
	IsPrismCreator = false;
	GLint Name = 0;
}

void OperableLayout::SetObj(Layout* obj) {
	Obj = obj;
	ObjFromFile = true;
	SetName();
}

bool OperableLayout::SetSpareTex(Layout* newobj) {
	if (Obj) {
		texSpare = newobj;
		return true;
	}
	return false;
}

void OperableLayout::SetName() {
	if (!NameInit) {
		Name = Layout_h::NameGen++;
		NameInit = true;
		Layout_h::NamedLayout[Name] = (void*)this;
		//printf("NamedLayout[%d] = %016lx\n", Name, Layout_h::NamedLayout[Name]);
	}
}

void OperableLayout::SetSpecialOptions(unsigned long long x) {
	if (x & 0x1) Shine = true;
	if (x & 0x2) IsPrismCreator = true;
}

void OperableLayout::Draw() {
	assert(Obj != NULL);
	if (!Display) return;

	glPushName(Layout_h::NameBase);
	glPushMatrix(); {
		glPushName(Name);
		glTranslatef(Trans[0], Trans[1], Trans[2]);
		glScalef(Scale, Scale, Scale);
		glScalef(ExtraScales[0], ExtraScales[1], ExtraScales[2]);
		glRotatef(Rot[0], 1.0, 0, 0);
		glRotatef(Rot[1], 0, 1.0, 0);
		glRotatef(Rot[2], 0, 0, 1.0);
		Obj->Draw();
		glPopName();
	}
	glPopMatrix();
	glPopName();
	Rot[0] += rotvX;
	Rot[1] += rotvY;
	Rot[2] += rotvZ;
}

void OperableLayout::OperationShowedGUI() {
	ImGui::SliderFloat("X Rotate Speed", &rotvX, -1.0f, 1.0f);
	if (ImGui::Button("clearX speed")) { rotvX = 0; }
	ImGui::SliderFloat("Y Rotate Speed", &rotvY, -1.0f, 1.0f);
	if (ImGui::Button("clearY speed")) { rotvY = 0; }
	ImGui::SliderFloat("Z Rotate Speed", &rotvZ, -1.0f, 1.0f);
	if (ImGui::Button("clearZ speed")) { rotvZ = 0; }

	ImGui::SliderFloat("Extra Stretch in X", &ExtraScales[0], -0.2f, 5.0f);
	if (ImGui::Button("resetX stretch")) { ExtraScales[0] = 1; }
	ImGui::SliderFloat("Extra Stretch in Y", &ExtraScales[1], -0.2f, 5.0f);
	if (ImGui::Button("resetY stretch")) { ExtraScales[1] = 1; }
	ImGui::SliderFloat("Extra Stretch in Z", &ExtraScales[2], -0.2f, 5.0f);
	if (ImGui::Button("resetZ stretch")) { ExtraScales[2] = 1; }
}

/*interfaces for GameObject and GUI*/
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
	void glutDraw();

protected:
	virtual void glutDrawObj() {
		glutSolidTeapot(0.5f);
	}

private:

};

SolidTeapot::SolidTeapot() {
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
		glScalef(ExtraScales[0], ExtraScales[1], ExtraScales[2]);
		glRotatef(Rot[0], 1.0, 0, 0);
		glRotatef(Rot[1], 0, 1.0, 0);
		glRotatef(Rot[2], 0, 0, 1.0);
		glutDrawObj();
		if (NameInit)
			glPopName();
	}
	glPopMatrix();
	glPopName();
	Rot[0] += rotvX;
	Rot[1] += rotvY;
	Rot[2] += rotvZ;
}

void SolidTeapot::glutDraw() {
	glutDrawObj();
	Rot[0] += rotvX;
	Rot[1] += rotvY;
	Rot[2] += rotvZ;
}

class SolidCube : public SolidTeapot
{
public:
	SolidCube() {};
	~SolidCube() {};

protected:
	void virtual glutDrawObj() {
		glBegin(GL_QUADS);
		// +z
		glTexCoord2i(1, 1); glVertex3i(-1, 1, 1);
		glTexCoord2i(1, 0); glVertex3i(-1, -1, 1);
		glTexCoord2i(0, 0); glVertex3i(1, -1, 1);
		glTexCoord2i(0, 1); glVertex3i(1, 1, 1);
		glEnd();
		glBegin(GL_QUADS);
		// -z
		glTexCoord2i(1, 1); glVertex3i(1, 1, -1);
		glTexCoord2i(1, 0); glVertex3i(1, -1, -1);
		glTexCoord2i(0, 0); glVertex3i(-1, -1, -1);
		glTexCoord2i(0, 1); glVertex3i(-1, 1, -1);
		glEnd();
		glBegin(GL_QUADS);
		// +x
		glTexCoord2i(1, 1); glVertex3i(1, 1, 1);
		glTexCoord2i(1, 0); glVertex3i(1, -1, 1);
		glTexCoord2i(0, 0); glVertex3i(1, -1, -1);
		glTexCoord2i(0, 1); glVertex3i(1, 1, -1);
		glEnd();
		glBegin(GL_QUADS);
		// -x
		glTexCoord2i(1, 1); glVertex3i(-1, 1, -1);
		glTexCoord2i(1, 0); glVertex3i(-1, -1, -1);
		glTexCoord2i(0, 0); glVertex3i(-1, -1, 1);
		glTexCoord2i(0, 1); glVertex3i(-1, 1, 1);
		glEnd();
		glBegin(GL_QUADS);
		// +y
		glTexCoord2i(1, 1); glVertex3i(1, 1, 1);
		glTexCoord2i(1, 0); glVertex3i(1, 1, -1);
		glTexCoord2i(0, 0); glVertex3i(-1, 1, -1);
		glTexCoord2i(0, 1); glVertex3i(-1, 1, 1);
		glEnd();
		glBegin(GL_QUADS);
		// -y
		glTexCoord2i(1, 1); glVertex3i(1, -1, -1);
		glTexCoord2i(1, 0); glVertex3i(1, -1, 1);
		glTexCoord2i(0, 0); glVertex3i(-1, -1, 1);
		glTexCoord2i(0, 1); glVertex3i(-1, -1, -1);
		glEnd();
	}

private:

};

class SolidSphere : public SolidTeapot
{
public:
	SolidSphere() {};
	~SolidSphere() {};

protected:
	void virtual glutDrawObj() {
		glutSolidSphere(0.75, 100, 100);
	}

private:

};

class SolidTorus : public SolidTeapot
{
public:
	SolidTorus() {};
	~SolidTorus() {};

protected:
	void virtual glutDrawObj() {
		glutSolidTorus(0.25, 0.75, 100, 100);
	}

private:

};

class SolidCone : public SolidTeapot
{
public:
	SolidCone() {};
	~SolidCone() {};

protected:
	void virtual glutDrawObj() {
		float R = 0.5, H = R * std::sqrt(2.0);
		float unitAngle = 360.0 / 100.0;
		glBegin(GL_TRIANGLES); {
			for (int i = 0; i < 100; i++) {
				float rad = unitAngle * i * PI / 180.0;
				float nextrad = unitAngle * (i + 1) * PI / 180.0;
				glTexCoord2f(0.5, 0.0); glVertex3f(0.0, 0.0, 0.0);
				glTexCoord2f(0.0, 1.0); glVertex3f(R * std::cos(rad), 0.0, -R * std::sin(rad));
				glTexCoord2f(1.0, 1.0); glVertex3f(R * std::cos(nextrad), 0.0, -R * std::sin(nextrad));
			}
		}
		glEnd();
		glBegin(GL_TRIANGLES); {
			for (int i = 0; i < 100; i++) {
				float rad = unitAngle * i * PI / 180.0;
				float nextrad = unitAngle * (i + 1) * PI / 180.0;
				glTexCoord2f(0.5, 1.0); glVertex3f(0.0, H, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(R * std::cos(rad), 0.0, -R * std::sin(rad));
				glTexCoord2f(1.0, 0.0); glVertex3f(R * std::cos(nextrad), 0.0, -R * std::sin(nextrad));
			}
		}
		glEnd();
	}

private:

};

class SolidCylinder : public SolidTeapot
{
public:
	SolidCylinder() {};
	~SolidCylinder() {};

protected:
	void virtual glutDrawObj() {
		float R = 0.5, H = R * std::sqrt(2.0), topR = R;
		float unitAngle = 360.0 / 100.0;
		glBegin(GL_TRIANGLES); {
			for (int i = 0; i < 100; i++) {
				float rad = unitAngle * i * PI / 180.0;
				float nextrad = unitAngle * (i + 1) * PI / 180.0;
				glTexCoord2f(0.5, 0.0); glVertex3f(0.0, 0.0, 0.0);
				glTexCoord2f(0.0, 1.0); glVertex3f(R * std::cos(rad), 0.0, -R * std::sin(rad));
				glTexCoord2f(1.0, 1.0); glVertex3f(R * std::cos(nextrad), 0.0, -R * std::sin(nextrad));
			}
		}
		glEnd();
		glBegin(GL_TRIANGLES); {
			for (int i = 0; i <= 100; i++) {
				float rad = unitAngle * i * PI / 180.0;
				float nextrad = unitAngle * (i + 1) * PI / 180.0;
				glTexCoord2f(0.5, 0.0); glVertex3f(0.0, H, 0.0);
				glTexCoord2f(0.0, 1.0); glVertex3f(topR * std::cos(rad), H, -topR * std::sin(rad));
				glTexCoord2f(1.0, 1.0); glVertex3f(topR * std::cos(nextrad), H, -topR * std::sin(nextrad));

			}
		}
		glEnd();
		glBegin(GL_QUADS); {
			for (int i = 0; i < 100; i++) {
				float rad = unitAngle * i * PI / 180.0;
				float nextrad = unitAngle * (i + 1) * PI / 180.0;
				glTexCoord2f(0.0, 0.0); glVertex3f(R * std::cos(rad), 0.0, -R * std::sin(rad));
				glTexCoord2f(1.0, 0.0); glVertex3f(R * std::cos(nextrad), 0.0, -R * std::sin(nextrad));
				glTexCoord2f(1.0, 1.0); glVertex3f(topR * std::cos(nextrad), H, -topR * std::sin(nextrad));
				glTexCoord2f(0.0, 1.0); glVertex3f(topR * std::cos(rad), H, -topR * std::sin(rad));
			}
		}
		glEnd();
	}

private:

};

class SolidCylinderCamber : public SolidTeapot
{
public:
	SolidCylinderCamber() {};
	~SolidCylinderCamber() {};

protected:
	void virtual glutDrawObj() {
		float R = 0.5, H = R * std::sqrt(2.0), topR = R;
		float unitAngle = 360.0 / 100.0;
		glBegin(GL_QUADS); {
			for (int i = 0; i < 100; i++) {
				float rad = unitAngle * i * PI / 180.0;
				float nextrad = unitAngle * (i + 1) * PI / 180.0;
				glTexCoord2f(0.0, 0.0); glVertex3f(R * std::cos(rad), 0.0, -R * std::sin(rad));
				glTexCoord2f(1.0, 0.0); glVertex3f(R * std::cos(nextrad), 0.0, -R * std::sin(nextrad));
				glTexCoord2f(1.0, 1.0); glVertex3f(topR * std::cos(nextrad), H, -topR * std::sin(nextrad));
				glTexCoord2f(0.0, 1.0); glVertex3f(topR * std::cos(rad), H, -topR * std::sin(rad));
			}
		}
		glEnd();
	}

private:

};

class GeneralPrismCreator : public SolidTeapot
{
public:
	GeneralPrismCreator() {};
	~GeneralPrismCreator() {};
	static float fltParams[8];
	static float intParams[8];
	static void SetFloat(int ind, float x) { if (ind >= 0 && ind < 8) fltParams[ind] = x; }
	static void SetInt(int ind, int x) { if (ind >= 0 && ind < 8) intParams[ind] = x; }

protected:
	void virtual glutDrawObj();

private:

};

float GeneralPrismCreator::fltParams[8] = { 0 };
float GeneralPrismCreator::intParams[8] = { 0 };

void GeneralPrismCreator::glutDrawObj() {
	float R = fltParams[0], H = R * fltParams[1], topR = R * fltParams[2];
	int slice = intParams[0];
	float unitAngle = 360.0 / slice;
	glBegin(GL_TRIANGLES); {
		for (int i = 0; i < slice; i++) {
			float rad = unitAngle * i * PI / 180.0;
			float nextrad = unitAngle * (i + 1) * PI / 180.0;
			glTexCoord2f(0.5, 0.0); glVertex3f(0.0, 0.0, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(R * std::cos(rad), 0.0, -R * std::sin(rad));
			glTexCoord2f(1.0, 1.0); glVertex3f(R * std::cos(nextrad), 0.0, -R * std::sin(nextrad));
		}
	}
	glEnd();
	glBegin(GL_TRIANGLES); {
		for (int i = 0; i <= slice; i++) {
			float rad = unitAngle * i * PI / 180.0;
			float nextrad = unitAngle * (i + 1) * PI / 180.0;
			glTexCoord2f(0.5, 0.0); glVertex3f(0.0, H, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(topR * std::cos(rad), H, -topR * std::sin(rad));
			glTexCoord2f(1.0, 1.0); glVertex3f(topR * std::cos(nextrad), H, -topR * std::sin(nextrad));

		}
	}
	glEnd();
	glBegin(GL_QUADS); {
		for (int i = 0; i < slice; i++) {
			float rad = unitAngle * i * PI / 180.0;
			float nextrad = unitAngle * (i + 1) * PI / 180.0;
			glTexCoord2f(0.0, 0.0); glVertex3f(R * std::cos(rad), 0.0, -R * std::sin(rad));
			glTexCoord2f(1.0, 0.0); glVertex3f(R * std::cos(nextrad), 0.0, -R * std::sin(nextrad));
			glTexCoord2f(1.0, 1.0); glVertex3f(topR * std::cos(nextrad), H, -topR * std::sin(nextrad));
			glTexCoord2f(0.0, 1.0); glVertex3f(topR * std::cos(rad), H, -topR * std::sin(rad));
		}
	}
	glEnd();
}

class PlayerAddedLayout : public OperableLayout
// Class PlayerAddedLayout is a container for OperableLayout, which is created to provide interfaces for GUI.h
// When in BACKPACK_GUI mode, if the player click the screen, a PlayerAdderLayout instance will be created
// and the function glutDraw() of corresponding OperableLayout instance will be bind into it.
// Especially, if the corresponding instance is loaded obj, the pointer of it must be set and void (*DrawFunc)(void) is not needed.
// After that, this PlayerAddedLayout instance will be pushed into the std::map AddedLayout, and will be displayed later.
//
{
public:
	PlayerAddedLayout();
	~PlayerAddedLayout();
	virtual void Draw();
	virtual void PlayerAddInit(float x, float y, float z, bool assertShdr = true, bool appointedDrawFunc = false, void (*func)(void) = NULL);
	virtual void PlayerAddInit(float x, float y, float z, OperableLayout* layout);
	virtual void PushIntoList(int it) { Layout_h::AddedLayout[Name] = (void*)this; Layout_h::NameInGUI[Name] = it; }
	virtual void OperationShowedGUI();

	static void DrawPlayerAddedLayouts(GLint Shdr, bool Inconvenient2UpdateShdr = false);

protected:
	GLfloat Spec[4], Emi[4], Diff[4];
	int IndTex4GlutObj;
	bool AssertShdr;
	bool AppointedDrawFunc;
	void (*DrawFunc)(void);
	virtual void SetParams() {};
	virtual void ExtraParamsShowedGUI() {};
	bool AppointedGlutDrawFunc;
	OperableLayout* ptrLayout;

private:

};

PlayerAddedLayout::PlayerAddedLayout()
{
	AssertShdr = true;
	AppointedDrawFunc = false;
	AppointedGlutDrawFunc = false;
	DrawFunc = NULL;
	for (int i = 0; i < 4; i++) {
		Spec[i] = Emi[i] = 0.0f;
		Diff[i] = 0.4f;
	}
	IndTex4GlutObj = -1;
}

PlayerAddedLayout::~PlayerAddedLayout()
{
}

void PlayerAddedLayout::Draw() {
	if (AssertShdr) assert(Obj != NULL);
	if (!Display) return;

	glPushName(Layout_h::NameBase);
	glPushMatrix(); {
		if (NameInit) glPushName(Name);
		glTranslatef(Trans[0], Trans[1], Trans[2]);
		glScalef(Scale, Scale, Scale);
		glScalef(ExtraScales[0], ExtraScales[1], ExtraScales[2]);
		glRotatef(Rot[0], 1.0, 0, 0);
		glRotatef(Rot[1], 0, 1.0, 0);
		glRotatef(Rot[2], 0, 0, 1.0);
		if (AppointedDrawFunc) {
			SetParams();
			DrawFunc();
		}
		else if (AppointedGlutDrawFunc) {
			glPushMatrix();
			bool texOn = IndTex4GlutObj >= 0 && IndTex4GlutObj < Layout_h::Tex4NonShdrObj.size();
			if (texOn) {
				//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Layout_h::Tex4NonShdrObj[IndTex4GlutObj]);
			}
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Spec);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emi);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Diff);
			SetParams();
			ptrLayout->glutDraw();
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Layout_h::no_mat);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Layout_h::no_mat);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Layout_h::no_mat);
			if (texOn) {
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();
		}
		else Obj->Draw();
		if (NameInit) glPopName();
	}
	glPopMatrix();
	glPopName();
	Rot[0] += rotvX;
	Rot[1] += rotvY;
	Rot[2] += rotvZ;
}

void PlayerAddedLayout::PlayerAddInit(float x, float y, float z, bool assertShdr, bool appointedDrawFunc, void (*func)(void)) {
	SetName();
	Trans[0] = x, Trans[1] = y, Trans[2] = z;
	Scale = 1;
	Rot[0] = 0, Rot[1] = 1, Rot[2] = 0;
	AssertShdr = assertShdr;
	AppointedDrawFunc = appointedDrawFunc;
	DrawFunc = func;
}

void PlayerAddedLayout::PlayerAddInit(float x, float y, float z, OperableLayout* layout) {
	if (layout->IsObjFromFile()) {
		Obj = layout->Obj;
		texSpare = layout->texSpare;
		PlayerAddInit(x, y, z, true, false);
	}
	else {
		PlayerAddInit(x, y, z, false, false);
		AppointedGlutDrawFunc = true;
		ptrLayout = layout;
	}
	layout->CheckCompulsoryScale(&Scale);
}

void PlayerAddedLayout::OperationShowedGUI() {
	if (ptrLayout && AppointedGlutDrawFunc) {
		ExtraParamsShowedGUI();
		if (ImGui::Button("Change Texture")) { IndTex4GlutObj++; if (IndTex4GlutObj >= Layout_h::Tex4NonShdrObj.size()) { IndTex4GlutObj = -1; } }
		ImGui::SliderFloat("X Rotate Speed", &rotvX, -1.0f, 1.0f);
		if (ImGui::Button("clearX speed")) { rotvX = 0; }
		ImGui::SliderFloat("Y Rotate Speed", &rotvY, -1.0f, 1.0f);
		if (ImGui::Button("clearY speed")) { rotvY = 0; }
		ImGui::SliderFloat("Z Rotate Speed", &rotvZ, -1.0f, 1.0f);
		if (ImGui::Button("clearZ speed")) { rotvZ = 0; }
		ImGui::SliderFloat("Extra Stretch in X", &ExtraScales[0], -0.2f, 5.0f);
		if (ImGui::Button("resetX stretch")) { ExtraScales[0] = 1; }
		ImGui::SliderFloat("Extra Stretch in Y", &ExtraScales[1], -0.2f, 5.0f);
		if (ImGui::Button("resetY stretch")) { ExtraScales[1] = 1; }
		ImGui::SliderFloat("Extra Stretch in Z", &ExtraScales[2], -0.2f, 5.0f);
		if (ImGui::Button("resetZ stretch")) { ExtraScales[2] = 1; }
		ImGui::SliderFloat("R specular", &Spec[0], 0.0f, 1.0f);
		ImGui::SliderFloat("G specular", &Spec[1], 0.0f, 1.0f);
		ImGui::SliderFloat("B specular", &Spec[2], 0.0f, 1.0f);
		ImGui::SliderFloat("R emission", &Emi[0], 0.0f, 1.0f);
		ImGui::SliderFloat("G emission", &Emi[1], 0.0f, 1.0f);
		ImGui::SliderFloat("B emission", &Emi[2], 0.0f, 1.0f);
		ImGui::SliderFloat("R ambient & diffusion", &Diff[0], 0.0f, 1.0f);
		ImGui::SliderFloat("G ambient & diffusion", &Diff[1], 0.0f, 1.0f);
		ImGui::SliderFloat("B ambient & diffusion", &Diff[2], 0.0f, 1.0f);
	}
	else {
		ExtraParamsShowedGUI();
		ImGui::SliderFloat("X Rotate Speed", &rotvX, -1.0f, 1.0f);
		if (ImGui::Button("clearX speed")) { rotvX = 0; }
		ImGui::SliderFloat("Y Rotate Speed", &rotvY, -1.0f, 1.0f);
		if (ImGui::Button("clearY speed")) { rotvY = 0; }
		ImGui::SliderFloat("Z Rotate Speed", &rotvZ, -1.0f, 1.0f);
		if (ImGui::Button("clearZ speed")) { rotvZ = 0; }
		ImGui::SliderFloat("Extra Stretch in X", &ExtraScales[0], -0.2f, 5.0f);
		if (ImGui::Button("resetX stretch")) { ExtraScales[0] = 1; }
		ImGui::SliderFloat("Extra Stretch in Y", &ExtraScales[1], -0.2f, 5.0f);
		if (ImGui::Button("resetY stretch")) { ExtraScales[1] = 1; }
		ImGui::SliderFloat("Extra Stretch in Z", &ExtraScales[2], -0.2f, 5.0f);
		if (ImGui::Button("resetZ stretch")) { ExtraScales[2] = 1; }
	}
}

void PlayerAddedLayout::DrawPlayerAddedLayouts(GLint Shdr, bool Inconvenient2UpdateShdr) {
	PlayerAddedLayout* p;
	if (!Inconvenient2UpdateShdr) {
		for (std::map<int, void*>::iterator it = Layout_h::AddedLayout.begin(); it != Layout_h::AddedLayout.end(); it++) {
			p = (PlayerAddedLayout*)it->second;
			if (p->AssertShdr) {
				glUseProgram(Shdr);
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(p->Trans[0], p->Trans[1], p->Trans[2]));
				model = glm::scale(model, glm::vec3(p->Scale * p->ExtraScales[0], p->Scale * p->ExtraScales[1], p->Scale * p->ExtraScales[2]));
				model = glm::rotate(model, glm::radians(p->Rot[0]), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(p->Rot[1]), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(p->Rot[2]), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(Shdr, "model"), 1, GL_FALSE, &model[0][0]);
			}
			else glUseProgram(0);
			p->Draw();
			glUseProgram(0);
		}
	}
	else {
		for (std::map<int, void*>::iterator it = Layout_h::AddedLayout.begin(); it != Layout_h::AddedLayout.end(); it++) {
			p = (PlayerAddedLayout*)it->second;
			if (p->AssertShdr) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(p->Trans[0], p->Trans[1], p->Trans[2]));
				model = glm::scale(model, glm::vec3(p->Scale * p->ExtraScales[0], p->Scale * p->ExtraScales[1], p->Scale * p->ExtraScales[2]));
				model = glm::rotate(model, glm::radians(p->Rot[0]), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(p->Rot[1]), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(p->Rot[2]), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(Shdr, "model"), 1, GL_FALSE, &model[0][0]);
			}
			else continue;
			p->Draw();
		}
		glUseProgram(0);
		for (std::map<int, void*>::iterator it = Layout_h::AddedLayout.begin(); it != Layout_h::AddedLayout.end(); it++) {
			p = (PlayerAddedLayout*)it->second;
			if (p->AssertShdr) continue;
			p->Draw();
		}
	}
}

class PlayerAddedLight : public PlayerAddedLayout
// A container for light source, storing light value only.
// This class is just an interface between GUI and DualLight. Light creating is in other modules.
//
{
public:
	PlayerAddedLight();
	~PlayerAddedLight();
	void OperationShowedGUI();
	glm::vec3 getTrans() { return glm::vec3(Trans[0], Trans[1], Trans[2]); }
	glm::vec3 getltAmbi() { return glm::vec3(ltAmbi[0], ltAmbi[1], ltAmbi[2]); }
	glm::vec3 getltDiff() { return glm::vec3(ltDiff[0], ltDiff[1], ltDiff[2]); }
	glm::vec3 getltSpec() { return glm::vec3(ltSpec[0], ltSpec[1], ltSpec[2]); }
	float getCons() const { return cons; }
	float getLinr() const { return linr; }
	float getQuad() const { return quadr; }

private:
	float ltAmbi[3], ltDiff[3], ltSpec[3];
	float cons, linr, quadr;
};

PlayerAddedLight::PlayerAddedLight() {
	ltAmbi[0] = 0.05f, ltAmbi[1] = 0.00f, ltAmbi[2] = 0.00f;
	ltDiff[0] = 0.80f, ltDiff[1] = 0.00f, ltDiff[2] = 0.00f;
	ltSpec[0] = 1.00f, ltSpec[1] = 1.00f, ltSpec[2] = 0.00f;
	cons = 1.0f, linr = 0.09f, quadr = 0.032f;
}

PlayerAddedLight::~PlayerAddedLight() {
}

void PlayerAddedLight::OperationShowedGUI() {
	if (Lit) {
		if (ImGui::Button("light turn off")) { if (texSpare && Obj) { Layout* t = Obj; Obj = texSpare; texSpare = t; } Lit = !Lit; }
	}
	else {
		if (ImGui::Button("light turn on")) { if (texSpare && Obj) { Layout* t = Obj; Obj = texSpare; texSpare = t; } Lit = !Lit; }
	}
	ImGui::SliderFloat("X Rotate Speed", &rotvX, -1.0f, 1.0f);
	if (ImGui::Button("clearX speed")) { rotvX = 0; }
	ImGui::SliderFloat("Y Rotate Speed", &rotvY, -1.0f, 1.0f);
	if (ImGui::Button("clearY speed")) { rotvY = 0; }
	ImGui::SliderFloat("Z Rotate Speed", &rotvZ, -1.0f, 1.0f);
	if (ImGui::Button("clearZ speed")) { rotvZ = 0; }
	ImGui::SliderFloat("Extra Stretch in X", &ExtraScales[0], -0.2f, 5.0f);
	if (ImGui::Button("resetX stretch")) { ExtraScales[0] = 1; }
	ImGui::SliderFloat("Extra Stretch in Y", &ExtraScales[1], -0.2f, 5.0f);
	if (ImGui::Button("resetY stretch")) { ExtraScales[1] = 1; }
	ImGui::SliderFloat("Extra Stretch in Z", &ExtraScales[2], -0.2f, 5.0f);
	if (ImGui::Button("resetZ stretch")) { ExtraScales[2] = 1; }
	ImGui::SliderFloat("light R specular", &ltSpec[0], 0.0f, 1.0f);
	ImGui::SliderFloat("light G specular", &ltSpec[1], 0.0f, 1.0f);
	ImGui::SliderFloat("light B specular", &ltSpec[2], 0.0f, 1.0f);
	ImGui::SliderFloat("light R diffusion", &ltDiff[0], 0.0f, 1.0f);
	ImGui::SliderFloat("light G diffusion", &ltDiff[1], 0.0f, 1.0f);
	ImGui::SliderFloat("light B diffusion", &ltDiff[2], 0.0f, 1.0f);
	ImGui::SliderFloat("light R ambient", &ltAmbi[0], 0.0f, 1.0f);
	ImGui::SliderFloat("light G ambient", &ltAmbi[1], 0.0f, 1.0f);
	ImGui::SliderFloat("light B ambient", &ltAmbi[2], 0.0f, 1.0f);
}

class PlayerAddedPrism : public PlayerAddedLayout
// A container for GeneralPrismCreator.
{
public:
	PlayerAddedPrism();
	~PlayerAddedPrism();

protected:
	virtual void SetParams();
	virtual void ExtraParamsShowedGUI();

private:
	float R, H_Rratio, TopR_Rratio;
	int Slices;
};

PlayerAddedPrism::PlayerAddedPrism()
{
	R = 0.5;
	H_Rratio = std::sqrt(2.0);
	TopR_Rratio = 1.0;
	Slices = 4;
}

PlayerAddedPrism::~PlayerAddedPrism()
{
}

void PlayerAddedPrism::SetParams() {
	GeneralPrismCreator::SetFloat(0, R);
	GeneralPrismCreator::SetFloat(1, H_Rratio);
	GeneralPrismCreator::SetFloat(2, TopR_Rratio);
	GeneralPrismCreator::SetInt(0, Slices);
}

void PlayerAddedPrism::ExtraParamsShowedGUI() {
	if (Slices < 200) {
		if (ImGui::Button("Add an Edge")) { Slices++; }
	}
	if (Slices > 3) {
		if (ImGui::Button("Remove an Edge")) { Slices--; }
	}
	if (Slices < 100) { if (ImGui::Button("Set As Cone Frustum (100 slices)")) { Slices = 100; } }
	if (Slices > 4) { if (ImGui::Button("Reset as Cube")) { Slices = 4; } }
	ImGui::SliderFloat("H / R(bottom)", &H_Rratio, 0.05f, 5.0f);
	ImGui::SliderFloat("R(top) / R(bottom)", &TopR_Rratio, 0.01f, 5.0f);
	if (ImGui::Button("Reset H & TopR")) { H_Rratio = std::sqrt(2.0), TopR_Rratio = 1.0; }
}

#undef max
#undef min
#undef PI
#endif