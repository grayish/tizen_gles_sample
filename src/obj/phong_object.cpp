#include "basic/obj/phong_object.h"

#include <stdlib.h>
#include <algorithm>

#include "basic/basic_container.h"
#include "basic/basic_gl_set.h"
#include "basic/mgr/basic_light_mgr.h"
#include "basic/mgr/basic_texture_mgr.h"
#include "basic/obj/basic_camera.h"
#include "basic/basic_shader.h"
#include "basic/basic_utils.h"

using namespace std;
using namespace glm;

PhongObject::PhongObject(const std::string &name, const ABasicMap *list, BasicCamera *camera) :
		BasicObject(name),
		mScale(1.0f),
		mDiffColor(0.9f, 0.1f, 0.1f),
		mSpecColor(1.0f),
		mAmbiColor(0),
		mEmitColor(0),
		mVertices(),
		mIndices(),
		mBufferVertices(0),
		mBufferIndices(0),
		mVertexArrayObject(0),
		mUniformList(dynamic_cast<const BasicMap<PhongObj_U_Elem> *>(list)->mList),
		mCamera(camera) {}

PhongObject::~PhongObject() {
	LOGI("destruct name[%s]", mName.c_str());
	glDeleteBuffers(1, &mBufferVertices);
	check_gl_error("glDeleteTextures");
	glDeleteBuffers(1, &mBufferIndices);
	check_gl_error("glDeleteTextures");
	glDeleteVertexArrays(1, &mVertexArrayObject);
	check_gl_error("glDeleteVertexArrays");

}

BasicObject *PhongObject::ImportObj(const std::string &objSource, const float &scale) {
	char *buffer = new char[objSource.length() + 1];
	strcpy(buffer, objSource.c_str());

	BasicObject *ret = ImporterScale(buffer, scale);

	delete buffer;

	return ret;
}

BasicObject *PhongObject::ImporterScale(char *objSource, const float &scale) {
	LOGI("%s", mName.c_str());

	vec3 sVec = vec3(scale);

	vector<vec3> posCoords;
	vector<vec3> norCoords;
	vector<vec2> texCoords;

	vector<string> strIndexer;

	float x, y, z;
	char *line, *linePtr;
	line = util_strtok(objSource, "\r\n", &linePtr);
	while (line) {
		char *word, *wordPtr;
		word = util_strtok(line, " ", &wordPtr);

		switch (word[0]) {
			case 'v':
				switch (word[1]) {
					case 'n': //normal coordinate
						word = util_strtok(nullptr, " ", &wordPtr);
						x = static_cast<float>(atof(word));
						word = util_strtok(nullptr, " ", &wordPtr);
						y = static_cast<float>(atof(word));
						word = util_strtok(nullptr, " ", &wordPtr);
						z = static_cast<float>(atof(word));
						norCoords.push_back(vec3(x, y, z));
						break;
					case 't': //texture coordinate
						word = util_strtok(nullptr, " ", &wordPtr);
						x = static_cast<float>(atof(word));
						word = util_strtok(nullptr, " ", &wordPtr);
						y = static_cast<float>(atof(word));
						texCoords.push_back(vec2(x, 1.0f - y));
						break;
					default: //vertex position
						word = util_strtok(nullptr, " ", &wordPtr);
						x = static_cast<float>(atof(word));
						word = util_strtok(nullptr, " ", &wordPtr);
						y = static_cast<float>(atof(word));
						word = util_strtok(nullptr, " ", &wordPtr);
						z = static_cast<float>(atof(word));
						posCoords.push_back(sVec * vec3(x, y, z));
						break;
				}

				break;

			case 'f':
				while ((word = util_strtok(nullptr, " ", &wordPtr))) {
					string face = word;

					vector<string>::iterator findIter = find(strIndexer.begin(),
															 strIndexer.end(), word);

					if (findIter != strIndexer.end()) {
						// this face already in vertex data
						mIndices.push_back((unsigned short &&) std::distance(strIndexer.begin(), findIter));
					}
					else {
						unsigned short face_index = static_cast<unsigned short>(strIndexer.size());
						mIndices.push_back(face_index);
						strIndexer.push_back(face);

						PhongVertex newVertex;
						char *num, *numPtr;
						num = util_strtok(word, "/", &numPtr); // position index
						newVertex.pos = posCoords.at((unsigned int) (atoi(num) - 1));
						num = util_strtok(nullptr, "/", &numPtr); // texture index
						newVertex.tex = texCoords.at((unsigned int) (atoi(num) - 1));
						num = util_strtok(nullptr, "/", &numPtr); // normal index
						newVertex.nor = norCoords.at((unsigned int) (atoi(num) - 1));

						mVertices.push_back(newVertex);
					}
				}

				break;

			case 'g':
				LOGI("obj name : %s", util_strtok(nullptr, " ", &wordPtr));
				break;

			default:
				break;
		}
		line = util_strtok(nullptr, "\r\n", &linePtr);
	}
	LOGI("vert, idx = %d, %d",mVertices.size(), mIndices.size());

	ComputeTangent();

	return this;
}

void PhongObject::ComputeTangent() {
	std::vector<vec3> triTangents;

	// Compute Tangent Basis
	for (int i = 0; i < mIndices.size(); i += 3) {
		vec3 p0 = mVertices.at(mIndices.at(i)).pos;
		vec3 p1 = mVertices.at(mIndices.at(i + 1)).pos;
		vec3 p2 = mVertices.at(mIndices.at(i + 2)).pos;

		vec3 uv0 = vec3(mVertices.at(mIndices.at(i)).tex, 0);
		vec3 uv1 = vec3(mVertices.at(mIndices.at(i + 1)).tex, 0);
		vec3 uv2 = vec3(mVertices.at(mIndices.at(i + 2)).tex, 0);

		vec3 deltaPos1 = p1 - p0;
		vec3 deltaPos2 = p2 - p0;

		vec3 deltaUV1 = uv1 - uv0;
		vec3 deltaUV2 = uv2 - uv0;

		// Compute the tangent
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		vec3 computedTangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

		triTangents.push_back(computedTangent);
		triTangents.push_back(computedTangent);
		triTangents.push_back(computedTangent);
	}

	// Accumulate tangents by indices
	for (int i = 0; i < mIndices.size(); ++i) {
		mVertices.at(mIndices.at(i)).tan = mVertices.at(mIndices.at(i)).tan + triTangents.at(i);
	}
}

void PhongObject::CreateVbo() {
	CreateBuffer(GL_ARRAY_BUFFER, &mBufferVertices,
				 (GLsizeiptr) mVertices.size() * sizeof(PhongVertex),
				 &(mVertices.at(0)), GL_STATIC_DRAW);

	CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, &mBufferIndices,
				 (GLsizeiptr) mIndices.size() * sizeof(GLushort),
				 &(mIndices.at(0)), GL_STATIC_DRAW);

}

void PhongObject::SetupAttribs() {
	if (mVertexArrayObject) {
		glBindVertexArray(mVertexArrayObject);
	} else {
		glGenVertexArrays(1, &mVertexArrayObject);
		check_gl_error("glGenVertexArrays");
		glBindVertexArray(mVertexArrayObject);
		check_gl_error("glBindVertexArray");

		std::vector<VertexAttrib> att;
		att.push_back(VertexAttrib{BASIC_ATTRIB_POS, 3, GL_FLOAT,
								   sizeof(PhongVertex),
								   (void *) offsetof(PhongVertex, pos)});
		att.push_back(VertexAttrib{BASIC_ATTRIB_NOR, 3, GL_FLOAT,
								   sizeof(PhongVertex),
								   (void *) offsetof(PhongVertex, nor)});
		att.push_back(VertexAttrib{BASIC_ATTRIB_TEX, 2, GL_FLOAT,
								   sizeof(PhongVertex),
								   (void *) offsetof(PhongVertex, tex)});
		att.push_back(VertexAttrib{BASIC_ATTRIB_TANGENT, 3, GL_FLOAT,
								   sizeof(PhongVertex),
								   (void *) offsetof(PhongVertex, tan)});

		SetupVertexAttribs(mBufferVertices, att);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferIndices);
		check_gl_error("glBindBuffer");
	}
}

void PhongObject::Draw() {
	SetupAttribs();

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_SHORT, 0);
	check_gl_error("glDrawElements");

	ResetAttrib();
}

void PhongObject::ResetAttrib() {
	if (mVertexArrayObject)
		glBindVertexArray(0);
}

void PhongObject::SetShaderUniforms(BasicShader *sh) {
	sh->SetUniformData(mUniformList[U_MAT_WORLD], translate(mat4_cast(mOrientation), mPosition));
	sh->SetUniformData(mUniformList[U_CAMERA_VIEW], mCamera->GetViewMat());
	sh->SetUniformData(mUniformList[U_CAMERA_PROJ], mCamera->GetPerspectiveMat());
	sh->SetUniformData(mUniformList[U_CAMERA_POS], mCamera->GetEye());

	if (mUniformList.find(U_MAT_DIFFUSE) != mUniformList.end())
		sh->SetUniformData(mUniformList[U_MAT_DIFFUSE], mDiffColor);
	if (mUniformList.find(U_MAT_SPECULAR) != mUniformList.end())
		sh->SetUniformData(mUniformList[U_MAT_SPECULAR], vec3(1.0));
	if (mUniformList.find(U_MAT_AMBIENT) != mUniformList.end())
		sh->SetUniformData(mUniformList[U_MAT_AMBIENT], vec3(0));
	if (mUniformList.find(U_MAT_EMITION) != mUniformList.end())
		sh->SetUniformData(mUniformList[U_MAT_EMITION], vec3(0));
	if (mUniformList.find(U_MAT_SHINESS) != mUniformList.end())
		sh->SetUniformData(mUniformList[U_MAT_SHINESS], 10.0f);

	Light_Mgr.PassUniforms(sh, this, mCamera);
	Texture_Mgr.ActiveTextures(sh, this);

}
