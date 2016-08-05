#ifndef __BASIC_TYPE_H__
#define __BASIC_TYPE_H__

#include <glm/glm.hpp>

#define BASIC_ATTRIB_POS        0
#define BASIC_ATTRIB_NOR        1
#define BASIC_ATTRIB_TEX        2
#define BASIC_ATTRIB_TANGENT    3
#define BASIC_ATTRIB_INSTPOS    5

// type definition, equal to GL* type
typedef void Bvoid;
typedef char Bchar;
typedef unsigned int Benum;
typedef unsigned char Bboolean;
typedef unsigned int Bbitfield;
typedef signed char Bbyte;       // Changed khronos_int8_t
typedef short Bshort;
typedef int Bint;
typedef int Bsizei;
typedef unsigned char Bubyte;      // Changed khronos_uint8_t
typedef unsigned short Bushort;
typedef unsigned int Buint;
typedef float Bfloat;      // Changed khronos_float_t
typedef float Bclampf;     // Changed khronos_float_t
typedef signed int Bfixed;      // Changed khronos_int32_t

/* GL types for handling large vertex buffer objects */
typedef signed long int Bintptr;     // Changed khronos_intptr_t
typedef signed long int Bsizeiptr;   // Changed khronos_ssize_t

/* TextureMagFilter */
#define BGL_NEAREST                        0x2600
#define BGL_LINEAR                         0x2601

/* TextureMinFilter */
/*      GL_NEAREST */
/*      GL_LINEAR */
#define BGL_NEAREST_MIPMAP_NEAREST         0x2700
#define BGL_LINEAR_MIPMAP_NEAREST          0x2701
#define BGL_NEAREST_MIPMAP_LINEAR          0x2702
#define BGL_LINEAR_MIPMAP_LINEAR           0x2703

/* TextureParameterName */
#define BGL_TEXTURE_MAG_FILTER             0x2800
#define BGL_TEXTURE_MIN_FILTER             0x2801
#define BGL_TEXTURE_WRAP_S                 0x2802
#define BGL_TEXTURE_WRAP_T                 0x2803

/* TextureWrapMode */
#define BGL_REPEAT                         0x2901
#define BGL_CLAMP_TO_EDGE                  0x812F
#define BGL_MIRRORED_REPEAT                0x8370

/* TextureTarget */
/*      GL_TEXTURE_2D */
#define BGL_TEXTURE_CUBE_MAP               0x8513
#define BGL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515

/* DataType */
#define BGL_BYTE                           0x1400
#define BGL_UNSIGNED_BYTE                  0x1401
#define BGL_SHORT                          0x1402
#define BGL_UNSIGNED_SHORT                 0x1403
#define BGL_INT                            0x1404
#define BGL_UNSIGNED_INT                   0x1405
#define BGL_FLOAT                          0x1406
#define BGL_FIXED                          0x140C


typedef struct _VertexAttrib {
	Buint index;
	Bint size;
	Benum type;
	Bsizei stride;
	Bvoid *pointer;
} VertexAttrib;

typedef enum _TexData_Type {
	TEXDATA_GENERAL, // general texture
	TEXDATA_NORMAL_MAP, // normal map texture
	TEXDATA_CUBE_MAP, // cube map texture
	TEXDATA_HEIGHT_MAP, // height map texture
	TEXDATA_MIPMAP, // mipmap texture
	TEXDATA_TEX_ARRAY, // 2D texture array
	TEXDATA_DYNAMIC_CUBE_MAP, // cube map texture
} TexData_Type;

typedef enum _BasicObject_Type {
	PHONG_OBJ,
	INST_OBJ
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
	INST_U_CAMERA_VIEW,
	INST_U_CAMERA_PROJ,
	INST_U_MAT_WORLD,
} InstObj_U_Elem;

#endif //__BASIC_TYPE_H__
