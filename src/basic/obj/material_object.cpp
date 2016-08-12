#include "basic/obj/material_object.h"

#include <stdlib.h>
#include <algorithm>

#include "basic/basic_container.h"
#include "basic/basic_gl_set.h"
#include "basic/mgr/basic_light_mgr.h"
#include "basic/mgr/basic_texture_mgr.h"
#include "basic/obj/basic_camera.h"
#include "basic/basic_file_loader.h"
#include "basic/basic_shader.h"
#include "basic/basic_utils.h"

using namespace std;
using namespace glm;

MaterialObject::MaterialObject(const std::string &name, const ABasicMap *list, BasicCamera *camera) :
		BasicObject(name),
		mScale(1.0f),
		mVertices(),
		mIndices(),
		mBufferVertices(0),
		mBufferIndices(0),
		mVertexArrayObject(0),
		mUniformList(dynamic_cast<const BasicMap<MaterialObj_U_Elem> *>(list)->mList),
		mCamera(camera) {}

MaterialObject::~MaterialObject() {
	LOGI("destruct name[%s]", mName.c_str());
	glDeleteBuffers(1, &mBufferVertices);
	check_gl_error("glDeleteTextures");
	glDeleteBuffers(1, &mBufferIndices);
	check_gl_error("glDeleteTextures");
	glDeleteVertexArrays(1, &mVertexArrayObject);
	check_gl_error("glDeleteVertexArrays");

}

BasicObject *MaterialObject::ImportObj(const std::string &objFilename, const float &scale) {
	FileStream fs(objFilename);

	LOGI("%s", mName.c_str());

	const char *delim = " \n\r\t";

	vec3 sVec = vec3(scale);

	unsigned int posOffset = 0;
	unsigned int norOffset = 0;
	unsigned int idxOffset = 0;
	vector<vec3> posCoords;
	vector<vec3> norCoords;
	vector<string> strIndexer;

	string newmtl;
	Material *mtl;
	std::map<std::string, Material *> mtlItems;

	float x, y, z;
	char line[512];
	while (fs.GetLine(line, 512)) {
		char *word, *wordPtr;
		word = util_strtok(line, delim, &wordPtr);
		if (!word)
			continue;

		if (word && !strcmp(word, "mtllib")) {
			char *mtlPath = util_strtok(nullptr, delim, &wordPtr);
			ImporterMtl(mtlPath, mtlItems);
		}
		else if (word && !strcmp(word, "usemtl")) {
			newmtl = util_strtok(nullptr, delim, &wordPtr);
			if (mtlItems.find(newmtl) == mtlItems.end()) {
				LOGE("newmtl[%s] doesn't exist!", newmtl.c_str());
			}
			mtl = mtlItems[newmtl];
			LOGI("%s,ka[%f,%f,%f],kd[%f,%f,%f],ks[%f,%f,%f],ke[%f,%f,%f]",
				 newmtl.c_str(),
				 mtl->ka.x, mtl->ka.y, mtl->ka.z,
				 mtl->kd.x, mtl->kd.y, mtl->kd.z,
				 mtl->ks.x, mtl->ks.y, mtl->ks.z,
				 mtl->ke.x, mtl->ke.y, mtl->ke.z);
		}

		switch (word[0]) {
			case 'v':
				switch (word[1]) {
					case 'n': //normal coordinate
						word = util_strtok(nullptr, delim, &wordPtr);
						x = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						y = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						z = static_cast<float>(atof(word));
						norCoords.push_back(vec3(x, y, z));
						break;
					case 't': //skip texture coordinate
						break;
					default: //vertex position
						word = util_strtok(nullptr, delim, &wordPtr);
						x = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						y = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						z = static_cast<float>(atof(word));
						posCoords.push_back(sVec * vec3(x, y, z));
						break;
				}

				break;

			case 'f':
				while ((word = util_strtok(nullptr, delim, &wordPtr))) {
					string face = word;
					/*
					vector<string>::iterator findIter = find(strIndexer.begin(),
															 strIndexer.end(), word);

					if (findIter != strIndexer.end()) {
						// this face already in vertex data
						mIndices.push_back((unsigned short &&) std::distance(strIndexer.begin(), findIter) + idxOffset);
					}
					else*/
					{
						unsigned int face_index = strIndexer.size();
						mIndices.push_back(face_index + idxOffset);
						strIndexer.push_back(face);

						MaterialVertex newVertex;
						char *num, *numPtr;
						num = util_strtok(word, "/", &numPtr); // position index
						newVertex.pos = posCoords.at((atoi(num) - 1 - posOffset));
//						num = util_strtok(nullptr, "/", &numPtr); // tex index
//						newVertex.tex = posCoords.at((unsigned int) (atoi(num) - 1));
						num = util_strtok(nullptr, "/", &numPtr); // normal index
						newVertex.nor = norCoords.at((atoi(num) - 1 - norOffset));
						newVertex.ka = mtl->ka;
						newVertex.kd = mtl->kd;
						newVertex.ks = mtl->ks;
						newVertex.ke = mtl->ke;


						mVertices.push_back(newVertex);
					}
				}

				break;

			case 'g':
				LOGI("obj name : %s", util_strtok(nullptr, delim, &wordPtr));
				break;

			case '#':
				word = util_strtok(nullptr, delim, &wordPtr);
				if (word && !strcmp(word, "object")) {
					posOffset += posCoords.size();
					norOffset += norCoords.size();
					posCoords.clear();
					norCoords.clear();

					idxOffset += strIndexer.size();
					strIndexer.clear();
				}

				break;

			default:
				break;
		}
	}
	LOGI("vert, idx = %d, %d", mVertices.size(), mIndices.size());

	std::map<std::string, Material *>::iterator it = mtlItems.begin();
	for (; it != mtlItems.end(); it++) {
		delete it->second;
	}

	return this;
}

void MaterialObject::ImporterMtl(const string &mtlFilename, std::map<std::string, Material *> &mtlItems) {
	FileStream fs(mtlFilename);
	LOGI("%s", mName.c_str());

	string newmtl;

	const char *delim = " \n\r\t";

	float r, g, b;
	char line[512];
	while (fs.GetLine(line, 512)) {
		char *word, *wordPtr;
		word = util_strtok(line, delim, &wordPtr);
		if (!word)
			continue;

		if (word && !strcmp(word, "newmtl")) {
			newmtl = util_strtok(nullptr, delim, &wordPtr);
			mtlItems[newmtl] = new Material();
			LOGI("newmtl[%s]", newmtl.c_str());
		}

		switch (word[0]) {
			// Ka, Kd, Ks, Ke: color
			// Ns: Specular exponent (0~1000)
			// d: desolve == 1-Tr
			// Tr: Transparent
			case 'K':
				switch (word[1]) {
					case 'a': //ambient color
						word = util_strtok(nullptr, delim, &wordPtr);
						r = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						g = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						b = static_cast<float>(atof(word));
						mtlItems[newmtl]->ka = vec3(r, g, b);
						LOGI("Ka[%f,%f,%f]", mtlItems[newmtl]->ka.r, mtlItems[newmtl]->ka.g, mtlItems[newmtl]->ka.b);
						break;
					case 'd': //diffuse color
						word = util_strtok(nullptr, delim, &wordPtr);
						r = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						g = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						b = static_cast<float>(atof(word));
						mtlItems[newmtl]->kd = vec3(r, g, b);
						LOGI("Kd[%f,%f,%f]", mtlItems[newmtl]->kd.r, mtlItems[newmtl]->kd.g, mtlItems[newmtl]->kd.b);
						break;
					case 's': //specular color
						word = util_strtok(nullptr, delim, &wordPtr);
						r = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						g = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						b = static_cast<float>(atof(word));
						mtlItems[newmtl]->ks = vec3(r, g, b);
						LOGI("Ks[%f,%f,%f]", mtlItems[newmtl]->ks.r, mtlItems[newmtl]->ks.g, mtlItems[newmtl]->ks.b);
						break;
					case 'e': //emissive color
						word = util_strtok(nullptr, delim, &wordPtr);
						r = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						g = static_cast<float>(atof(word));
						word = util_strtok(nullptr, delim, &wordPtr);
						b = static_cast<float>(atof(word));
						mtlItems[newmtl]->ke = vec3(r, g, b);
						LOGI("Ke[%f,%f,%f]", mtlItems[newmtl]->ke.r, mtlItems[newmtl]->ke.g, mtlItems[newmtl]->ke.b);
						break;
				}
				break;
			default:
				break;
		}
	}

	return;
}

void MaterialObject::CreateVbo() {
	CreateBuffer(GL_ARRAY_BUFFER, &mBufferVertices,
				 (GLsizeiptr) mVertices.size() * sizeof(MaterialVertex),
				 &(mVertices.at(0)), GL_STATIC_DRAW);

	CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, &mBufferIndices,
				 (GLsizeiptr) mIndices.size() * sizeof(GLuint),
				 &(mIndices.at(0)), GL_STATIC_DRAW);

}

void MaterialObject::SetupAttribs() {
	if (mVertexArrayObject) {
		glBindVertexArray(mVertexArrayObject);
	} else {
		glGenVertexArrays(1, &mVertexArrayObject);
		check_gl_error("glGenVertexArrays");
		glBindVertexArray(mVertexArrayObject);
		check_gl_error("glBindVertexArray");

		std::vector<VertexAttrib> att;
		att.push_back(VertexAttrib{MRT_ATTRIB_POS, 3, GL_FLOAT,
								   sizeof(MaterialVertex),
								   (void *) offsetof(MaterialVertex, pos)});
		att.push_back(VertexAttrib{MRT_ATTRIB_NOR, 3, GL_FLOAT,
								   sizeof(MaterialVertex),
								   (void *) offsetof(MaterialVertex, nor)});
		att.push_back(VertexAttrib{MRT_ATTRIB_KA, 3, GL_FLOAT,
								   sizeof(MaterialVertex),
								   (void *) offsetof(MaterialVertex, ka)});
		att.push_back(VertexAttrib{MRT_ATTRIB_KD, 3, GL_FLOAT,
								   sizeof(MaterialVertex),
								   (void *) offsetof(MaterialVertex, kd)});
		att.push_back(VertexAttrib{MRT_ATTRIB_KS, 3, GL_FLOAT,
								   sizeof(MaterialVertex),
								   (void *) offsetof(MaterialVertex, ks)});
		att.push_back(VertexAttrib{MRT_ATTRIB_KE, 3, GL_FLOAT,
								   sizeof(MaterialVertex),
								   (void *) offsetof(MaterialVertex, ke)});

		SetupVertexAttribs(mBufferVertices, att);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferIndices);
		check_gl_error("glBindBuffer");
	}
}

void MaterialObject::Draw() {
	SetupAttribs();

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	check_gl_error("glDrawElements");

	ResetAttrib();
}

void MaterialObject::ResetAttrib() {
	if (mVertexArrayObject)
		glBindVertexArray(0);
}

void MaterialObject::SetShaderUniforms(BasicShader *sh) {
	sh->SetUniformData(mUniformList[MTL_U_MAT_WORLD], translate(mat4_cast(mOrientation), mPosition));
	sh->SetUniformData(mUniformList[MTL_U_CAMERA_VIEW], mCamera->GetViewMat());
	sh->SetUniformData(mUniformList[MTL_U_CAMERA_PROJ], mCamera->GetPerspectiveMat());
	sh->SetUniformData(mUniformList[MTL_U_CAMERA_POS], mCamera->GetEye());

	if (mUniformList.find(MTL_U_MAT_SHINESS) != mUniformList.end())
		sh->SetUniformData(mUniformList[MTL_U_MAT_SHINESS], 10.0f);

	Light_Mgr.PassUniforms(sh, this, mCamera);
	Texture_Mgr.ActiveTextures(sh, this);

}
