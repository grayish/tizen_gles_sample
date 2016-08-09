#version 300 es

#define TRANSFORM_ATTRIB_POS      0
#define TRANSFORM_ATTRIB_COLOR    1
#define TRANSFORM_ATTRIB_SIZE     2
#define TRANSFORM_ATTRIB_LIFE     3

#define PARTICLE_LIFT_TIME 0.7

layout(location = TRANSFORM_ATTRIB_POS) in vec2 pos;
layout(location = TRANSFORM_ATTRIB_COLOR) in vec3 color;
layout(location = TRANSFORM_ATTRIB_SIZE) in float size;
layout(location = TRANSFORM_ATTRIB_LIFE) in float life;

out vec4 v_color;

void main()
{
	gl_Position = vec4( pos , 0.0, 1.0);
	gl_PointSize = size;
	float span = life / PARTICLE_LIFT_TIME * 1.72 + 1.0; // x*(e-1)+1
	v_color = vec4(color, log(span));
}