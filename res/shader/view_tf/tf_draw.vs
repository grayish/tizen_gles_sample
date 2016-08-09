#version 300 es

#define TRANSFORM_ATTRIB_POSITION      0
#define TRANSFORM_ATTRIB_VELOCITY      1
#define TRANSFORM_ATTRIB_SIZE          2
#define TRANSFORM_ATTRIB_CURTIME       3
#define TRANSFORM_ATTRIB_LIFETIME      4

layout(location = TRANSFORM_ATTRIB_POSITION) in vec2 position;
layout(location = TRANSFORM_ATTRIB_VELOCITY) in vec2 velocity;
layout(location = TRANSFORM_ATTRIB_SIZE) in float size;
layout(location = TRANSFORM_ATTRIB_CURTIME) in float curtime;
layout(location = TRANSFORM_ATTRIB_LIFETIME) in float lifetime;

uniform float u_time;
uniform vec2 u_acceleration;

void main()
{
	float deltaTime = u_time - curtime;
			vec2 velocity = velocity + deltaTime * u_acceleration;
    		vec2 position = position + deltaTime * velocity;
	if ( deltaTime <= lifetime )
	{
		gl_Position = vec4( position, 0.0, 1.0 );
		gl_PointSize = 200.0;

//		gl_PointSize = size * ( 1.0 - deltaTime / lifetime );
	}
	else
	{
		gl_Position = vec4( -1000, -1000, 0, 0 );
		gl_PointSize = 0.0;
//gl_Position = vec4( 0, 0, 0, 1.0 );
//gl_PointSize = 20.0;
	}
	gl_Position = vec4( 0.0, 0.0, 0, 1.0 );
	gl_PointSize = position.x;

}