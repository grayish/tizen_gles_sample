#version 300 es

#define NUM_PARTICLES           200
#define ATTRIBUTE_POSITION      0
#define ATTRIBUTE_VELOCITY      1
#define ATTRIBUTE_SIZE          2
#define ATTRIBUTE_CURTIME       3
#define ATTRIBUTE_LIFETIME      4

uniform float u_time;
uniform float u_emissionRate;
uniform mediump sampler3D s_noiseTex;

layout(location = ATTRIBUTE_POSITION) in vec2 a_position;
layout(location = ATTRIBUTE_VELOCITY) in vec2 a_velocity;
layout(location = ATTRIBUTE_SIZE) in float a_size;
layout(location = ATTRIBUTE_CURTIME) in float a_curtime;
layout(location = ATTRIBUTE_LIFETIME) in float a_lifetime;

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
     v_position = a_position;
     v_velocity = a_velocity;
     v_size = a_size;
     v_curtime = a_curtime;
     v_lifetime = a_lifetime;
  }
  gl_Position = vec4( v_position, 0.0, 1.0 );
}
