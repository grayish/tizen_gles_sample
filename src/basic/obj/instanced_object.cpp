#include "basic/obj/instanced_object.h"

#include <stdlib.h>
#include <algorithm>
#include "basic/basic_utils.h"
#include "basic/basic_gl_set.h"
#include "basic/basic_shader.h"
#include "basic/obj/basic_camera.h"
#include "basic/basic_container.h"

using namespace glm;
using namespace std;

InstObject::InstObject(const std::string &name, const ABasicMap *list, BasicCamera *camera) :
		BasicObject(name),
		mVertices(),
		mIndices(),
		mInstPosMat(),
		mBufferVertices(0),
		mBufferIndices(0),
		mBufferInstPos(0),
		mVertexArrayObject(0),
		mUniformList(dynamic_cast<const BasicMap<InstObj_U_Elem> *>(list)->mList),
		mCamera(camera),
		mScale(1.0f),
		mInstCount(1) {

}

InstObject::~InstObject() {
	LOGI("destruct name[%s]", mName.c_str());
	glDeleteBuffers(1, &mBufferVertices);
	check_gl_error("glDeleteTextures");
	glDeleteBuffers(1, &mBufferIndices);
	check_gl_error("glDeleteTextures");
	glDeleteVertexArrays(1, &mVertexArrayObject);
	check_gl_error("glDeleteVertexArrays");

}

BasicObject *InstObject::ImportObj(const std::string &objSource, const float &scale) {
	char *buffer = new char[objSource.length() + 1];
	strcpy(buffer, objSource.c_str());

	BasicObject *ret = ImporterScale(buffer, scale);

	delete buffer;

	return ret;
}

BasicObject *InstObject::ImporterScale(char *objSource, const float &scale) {
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
						texCoords.push_back(vec2(x, y));
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

						InstVertex newVertex;
						char *num, *numPtr;
						num = util_strtok(word, "/", &numPtr); // position index
						newVertex.pos = posCoords.at((unsigned int) (atoi(num) - 1));
						num = util_strtok(nullptr, "/", &numPtr); // texture index
//                        newVertex.tex = texCoords.at((unsigned int) (atoi(num) - 1));
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

	return this;
}

void InstObject::CreateVbo() {
	CreateBuffer(GL_ARRAY_BUFFER, &mBufferVertices,
				 (GLsizeiptr) mVertices.size() * sizeof(InstVertex),
				 &(mVertices.at(0)), GL_STATIC_DRAW);

	CreateBuffer(GL_ARRAY_BUFFER, &mBufferInstPos,
				 (GLsizeiptr) mInstPosMat.size() * sizeof(mat4),
				 &(mInstPosMat.at(0)), GL_STATIC_DRAW);

	CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, &mBufferIndices,
				 (GLsizeiptr) mIndices.size() * sizeof(GLushort),
				 &(mIndices.at(0)), GL_STATIC_DRAW);

}

void InstObject::SetupAttribs() {
	if (mVertexArrayObject) {
		glBindVertexArray(mVertexArrayObject);
	} else {
		glGenVertexArrays(1, &mVertexArrayObject);
		check_gl_error("glGenVertexArrays");
		glBindVertexArray(mVertexArrayObject);

		std::vector<VertexAttrib> att;
		att.push_back(VertexAttrib{BASIC_ATTRIB_POS, 3, GL_FLOAT,
								   sizeof(InstVertex),
								   (void *) offsetof(InstVertex, pos)});
		att.push_back(VertexAttrib{BASIC_ATTRIB_NOR, 3, GL_FLOAT,
								   sizeof(InstVertex),
								   (void *) offsetof(InstVertex, nor)});

		SetupVertexAttribs(mBufferVertices, att);

		att.clear();
		int offset = 0;
		att.push_back(VertexAttrib{BASIC_ATTRIB_INSTPOS, 4, GL_FLOAT,
								   sizeof(mat4),
								   (void *) offset});
		offset += sizeof(vec4);
		att.push_back(VertexAttrib{BASIC_ATTRIB_INSTPOS + 1, 4, GL_FLOAT,
								   sizeof(mat4),
								   (void *) offset});
		offset += sizeof(vec4);
		att.push_back(VertexAttrib{BASIC_ATTRIB_INSTPOS + 2, 4, GL_FLOAT,
								   sizeof(mat4),
								   (void *) offset});
		offset += sizeof(vec4);
		att.push_back(VertexAttrib{BASIC_ATTRIB_INSTPOS + 3, 4, GL_FLOAT,
								   sizeof(mat4),
								   (void *) offset});

		SetupVertexAttribs(mBufferInstPos, att);

		glVertexAttribDivisor(BASIC_ATTRIB_INSTPOS, 1);
		glVertexAttribDivisor(BASIC_ATTRIB_INSTPOS + 1, 1);
		glVertexAttribDivisor(BASIC_ATTRIB_INSTPOS + 2, 1);
		glVertexAttribDivisor(BASIC_ATTRIB_INSTPOS + 3, 1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferIndices);
		check_gl_error("glBindBuffer");
	}
}

void InstObject::Draw() {
	SetupAttribs();

	glDrawElementsInstanced(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_SHORT, nullptr, mInstCount);
	check_gl_error("glDrawElementsInstanced");

	ResetAttrib();
}

void InstObject::ResetAttrib() {
	if (mVertexArrayObject)
		glBindVertexArray(0);
}

void InstObject::SetShaderUniforms(BasicShader *sh) {
	sh->SetUniformData(mUniformList[U_INST_MAT_WORLD], translate(mat4_cast(mOrientation), mPosition));
	sh->SetUniformData(mUniformList[U_INST_CAMERA_VIEW], mCamera->GetViewMat());
	sh->SetUniformData(mUniformList[U_INST_CAMERA_PROJ], mCamera->GetPerspectiveMat());

}

void InstObject::SetInstancing(const int &count,
							   const int &row,
							   const int &column,
							   const float &gridSize) {
	mInstPosMat.clear();
	mInstCount = count;

	mat4 eyeMat = mat4();
	vec3 location = vec3(0);

	for (int rowInd = -row / 2; rowInd < row / 2; ++rowInd) {
		for (int colInd = -column / 2; colInd < column / 2; ++colInd) {
			location.z = -rowInd * gridSize;
			location.x = colInd * gridSize;
			mInstPosMat.push_back(scale(translate(eyeMat, location), vec3(0.3f, 0.3f, 0.3f)));
		}

	}
}