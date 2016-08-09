#ifndef __BASIC_TYPE_H__
#define __BASIC_TYPE_H__

#include <glm/glm.hpp>
#include "basic/basic_gl_set.h"

#define BASIC_ATTRIB_POS        0
#define BASIC_ATTRIB_NOR        1
#define BASIC_ATTRIB_TEX        2
#define BASIC_ATTRIB_TANGENT    3
#define BASIC_ATTRIB_INSTPOS    5

#define TRANSFORM_ATTRIB_POS      0
#define TRANSFORM_ATTRIB_COLOR    1
#define TRANSFORM_ATTRIB_SIZE     2
#define TRANSFORM_ATTRIB_LIFE     3

#define MRT_ATTRIB_POS      0
#define MRT_ATTRIB_NOR      1
#define MRT_ATTRIB_KA       2
#define MRT_ATTRIB_KD       3
#define MRT_ATTRIB_KS       4
#define MRT_ATTRIB_KE       5

typedef struct _VertexAttrib {
	GLuint index;
	GLint size;
	GLenum type;
	GLsizei stride;
	GLvoid *pointer;
} VertexAttrib;

typedef enum _TexContainer_Type {
	TEX_2D_FILE,
	TEX_2D_PTR,
	TEX_3D_FILE,
	TEX_3D_PTR,
	TEX_CUBE_FILE,
	TEX_CUBE_PTR
} TexContainer_Type;

typedef enum _BasicObject_Type {
	PHONG_OBJ,
	INST_OBJ,
	TRANSFORM_OBJ,
	MATERIAL_OBJ
} BasicObject_Type;

typedef enum _BasicLight_Type {
	POINT_LT,
	SPOT_LT,
	NONE
} BasicLight_Type;

typedef struct _PhongVertex {
	glm::vec3 pos;
	glm::vec3 nor;
	glm::vec2 tex;
	glm::vec3 tan;
} PhongVertex;

typedef enum _PhongObj_U_Elem {
	U_CAMERA_VIEW,
	U_CAMERA_PROJ,
	U_CAMERA_POS,
	U_MAT_WORLD,
	U_MAT_DIFFUSE,
	U_MAT_SPECULAR,
	U_MAT_AMBIENT,
	U_MAT_EMITION,
	U_MAT_SHINESS,
} PhongObj_U_Elem;

typedef enum _PointLt_U_Elem {
	U_PL_DIFFUSE,
	U_PL_SPECULAR,
	U_PL_AMBIENT,
	U_PL_POS,
} PointLt_U_Elem;

typedef struct _InstVertex {
	glm::vec3 pos;
	glm::vec3 nor;
} InstVertex;

typedef enum _InstObj_U_Elem {
	U_INST_CAMERA_VIEW,
	U_INST_CAMERA_PROJ,
	U_INST_MAT_WORLD,
} InstObj_U_Elem;

// Transformfeedback
typedef struct _Particle {
	glm::vec2 pos;
	glm::vec3 color;
	float size;
	float life;
} Particle;

typedef enum _TransformObj_U_Elem {
	U_TF_TIME,
	U_TF_STEP,
	U_TF_TOUCH_PT
} TransformObj_U_Elem;

typedef struct _Material {
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 ke;
} Material;

typedef struct _MaterialVertex {
	glm::vec3 pos;
	glm::vec3 nor;
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 ke;
} MaterialVertex;

typedef enum _MaterialObj_U_Elem {
	MTL_U_CAMERA_VIEW,
	MTL_U_CAMERA_PROJ,
	MTL_U_CAMERA_POS,
	MTL_U_MAT_WORLD,
	MTL_U_MAT_SHINESS,
} MaterialObj_U_Elem;

#endif //__BASIC_TYPE_H__
