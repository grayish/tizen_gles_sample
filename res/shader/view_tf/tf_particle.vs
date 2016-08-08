#version 300 es

#define NUM_PARTICLES           200
#define TRANSFORM_ATTRIB_POSITION      0
#define TRANSFORM_ATTRIB_VELOCITY      1
#define TRANSFORM_ATTRIB_SIZE          2
#define TRANSFORM_ATTRIB_CURTIME       3
#define TRANSFORM_ATTRIB_LIFETIME      4

uniform float u_time;
uniform float u_emissionRate;
uniform mediump sampler3D s_noiseTex;

layout(location = TRANSFORM_ATTRI_POSITION) in vec2 position;
layout(location = TRANSFORM_ATTRI_VELOCITY) in vec2 velocity;
layout(location = TRANSFORM_ATTRI_SIZE) in float size;
layout(location = TRANSFORM_ATTRI_CURTIME) in float curtime;
layout(location = TRANSFORM_ATTRI_LIFETIME) in float lifetime;

out vec2 v_position;
out vec2 v_velocity;
out float v_size;
out float v_curtime;
out float v_lifetime;

float randomValue( inout float seed )
{
   float vertexId = float( gl_VertexID ) / float( NUM_PARTICLES );
   vec3 texCoord = vec3( u_time, vertexId, seed );
   seed += 0.1;
   return texture( s_noiseTex, texCoord ).r;
}

void main()
{
  float seed = u_time;
  float lifetime = a_curtime - u_time;
  if( lifetime <= 0.0 && randomValue(seed) < u_emissionRate )
  {
     v_position = vec2( 0.0, -1.0 );
     v_velocity = vec2( randomValue(seed) * 2.0 - 1.00,
                        randomValue(seed) * 1.4 + 1.0 );
     v_size = randomValue(seed) * 20.0 + 60.0;
     v_curtime = u_time;
     v_lifetime = 2.0;
  }
  else
  {
     v_position = position;
     v_velocity = velocity;
     v_size = size;
     v_curtime = curtime;
     v_lifetime = lifetime;
  }
  gl_Position = vec4( v_position, 0.0, 1.0 );
}
