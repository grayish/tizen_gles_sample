#version 300 es

#define TRANSFORM_ATTRIB_POS      0
#define TRANSFORM_ATTRIB_COLOR    1
#define TRANSFORM_ATTRIB_SIZE     2
#define TRANSFORM_ATTRIB_LIFE     3

#define PARTICLE_LIFT_TIME 0.7

uniform float u_time;
uniform float u_step;
uniform vec2 u_touchPt;

layout(location = TRANSFORM_ATTRIB_POS) in vec2 pos;
layout(location = TRANSFORM_ATTRIB_COLOR) in vec3 color;
layout(location = TRANSFORM_ATTRIB_SIZE) in float size;
layout(location = TRANSFORM_ATTRIB_LIFE) in float life;

out vec2 v_pos;
out vec3 v_color;
out float v_size;
out float v_life;

const vec2 eps = vec2(0.002, 0.0);
const vec3 dx = eps.xyy;
const vec3 dy = eps.yxy;
const vec3 dz = eps.yyx;
const float regionLength = 2.0;
const vec3 spherePos = vec3(0);

const vec3 c_yellow = vec3(1.0, 1.0, 0.0);
const vec3 c_lime = vec3(0, 1.0, 0.0);

float noise1f(int x)
{
    x = (x<<13) ^ x;
    return ( 1.0 - float((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float snoise(float x)
{
    int xi = int(floor(x));
    float xf = x - float(xi);

    float h0 = noise1f(xi);
    float h1 = noise1f(xi + 1);

    // Smoothly nterpolate between noise values
    float t = smoothstep(0.0, 1.0, xf);
    return h0 + (h1 - h0) * t;
}

// Forward declarations
void Simplex3D_GetCornerVectors(vec3 P, out vec3 Pi, out vec3 Pi_1, out vec3 Pi_2, out vec4 v1234_x, out vec4 v1234_y, out vec4 v1234_z);
void FAST32_hash_3D(vec3 gridcell, vec3 v2_mask, out vec4 hash_0, out vec4 hash_1, out vec4 hash_2);
vec3 SimplexPerlin3D_Deriv(vec3 P);

float ramp(float r)
{
    r = clamp(r, -1.0, 1.0);
    float r2 = r * r;
    return r * (1.875 - 1.25 * r2 + 0.375 * r2 * r2);
}

// Source potential function
float phi(vec3 p)
{
    vec3 q = p - spherePos;
    return (3.1415) * log(dot(q, q) + eps.x);
}

// The analytic gradient of the above function
vec3 gradphi(vec3 p)
{
    vec3 q = p - spherePos;
    float m = (2.0 * 3.1415) / (dot(q, q) + eps.x);
    return m * q;
}

void main()
{
	float vertexId = float( gl_VertexID );
	float seed = float(vertexId) * 100.0 * u_time;

	// new particle
	if( life <= 0.0 )
	{
		vec2 p;

		// Random offset
		p.x = snoise(seed);
		p.y = snoise(seed + 127.0);

		// Normalize to get sphere distribution
		p = (0.1 + 0.04 * snoise(seed + 491.0)) * normalize(p);

		 vec3 c;
		 c.r = clamp(snoise(seed+233.0), 0.0, 1.0);
		 c.g = clamp(snoise(seed+11.0), 0.0, 1.0);
		 c.b = clamp(snoise(seed+383.0), 0.0, 1.0);

		v_pos = u_touchPt + p;
		v_color = c;
		v_size = 20.0 + 60.0 * snoise(seed + 28.0);
		v_life = (1.0 + 0.25 * snoise(seed)) * PARTICLE_LIFT_TIME;
		}
	else // move particle
	{
		vec3 p = vec3( pos - u_touchPt, 0.0 );

		vec3 seed = vec3(13.0f, 127.0f, 449.0f);
		vec3 scale = vec3(2.0, 2.01, 2.05);
		vec3 q = scale * p + seed + vec3(u_time) * vec3(0.05, 0.07, 0.09);
		vec3 dN = SimplexPerlin3D_Deriv(q).xyz + 0.7 * SimplexPerlin3D_Deriv(q * 5.01).xyz;
		vec3 v1 = vec3(dN.y - dN.z, dN.z - dN.x, dN.x - dN.y);

		// Calculate the gradient of the potential function
		vec3 v2 = gradphi(p);
		// We introduce turbulence after the particle has lived for a while
		float a1 = 1.0 - 0.6 * clamp(0.6 * life / PARTICLE_LIFT_TIME, 0.0, 1.0);
		// The particle is less affected by the sphere when it's far away
		float a2 = 1.0 - ramp(length(p - spherePos) / regionLength);
		// Superposition still gives an incompressible field
		vec3 v = 0.2 * a1 * v1 + 0.14 * a2 * v2;

		// Euler integration
		p += v * u_step;

		v_life = life - u_step;
		v_color = color;
		v_pos = p.xy + u_touchPt;
		v_size = size;
	}
	gl_Position = vec4( v_pos, 0.0, 1.0 );
}


// The following code implements functions for calculating
// analytic derivatives of simplex noise.
// Author:      Brian Sharpe
// Homepage:    http://briansharpe.wordpress.com
// Code:        https://github.com/BrianSharpe/GPU-Noise-Lib/blob/master/gpu_noise_lib.glsl
//
// The code also borrows from the work of Stefan Gustavson and Ian McEwan at
// http://github.com/ashima/webgl-noise
void Simplex3D_GetCornerVectors(vec3 P,                 //  input point
                                out vec3 Pi,            //  integer grid index for the origin
                                out vec3 Pi_1,          //  offsets for the 2nd and 3rd corners.  ( the 4th = Pi + 1.0 )
                                out vec3 Pi_2,
                                out vec4 v1234_x,       //  vectors from the 4 corners to the intput point
                                out vec4 v1234_y,
                                out vec4 v1234_z)
{
    // Simplex math from Stefan Gustavson's and Ian McEwan's work at...
    // http://github.com/ashima/webgl-noise

    // simplex math constants
    const float SKEWFACTOR = 1.0/3.0;
    const float UNSKEWFACTOR = 1.0/6.0;
    const float SIMPLEX_CORNER_POS = 0.5;
    const float SIMPLEX_PYRAMID_HEIGHT = 0.70710678118654752440084436210485; // sqrt( 0.5 ) height of simplex pyramid.

    P *= SIMPLEX_PYRAMID_HEIGHT; // scale space so we can have an approx feature size of 1.0  ( optional )

    // Find the vectors to the corners of our simplex pyramid
    Pi = floor( P + dot( P, vec3( SKEWFACTOR) ) );
    vec3 x0 = P - Pi + dot(Pi, vec3( UNSKEWFACTOR ) );
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    Pi_1 = min( g.xyz, l.zxy );
    Pi_2 = max( g.xyz, l.zxy );
    vec3 x1 = x0 - Pi_1 + UNSKEWFACTOR;
    vec3 x2 = x0 - Pi_2 + SKEWFACTOR;
    vec3 x3 = x0 - SIMPLEX_CORNER_POS;

    // pack them into a parallel-friendly arrangement
    v1234_x = vec4( x0.x, x1.x, x2.x, x3.x );
    v1234_y = vec4( x0.y, x1.y, x2.y, x3.y );
    v1234_z = vec4( x0.z, x1.z, x2.z, x3.z );
}

void FAST32_hash_3D(vec3 gridcell,
                    vec3 v1_mask, //    user definable v1 and v2.  ( 0's and 1's )
                    vec3 v2_mask,
                    out vec4 hash_0,
                    out vec4 hash_1,
                    out vec4 hash_2) // generates 3 random numbers for each of the 4 3D cell corners.  cell corners:  v0=0,0,0  v3=1,1,1  the other two are user definable
{
    // gridcell is assumed to be an integer coordinate

    // TODO:    these constants need tweaked to find the best possible noise.
    //          probably requires some kind of brute force computational searching or something....
    const vec2 OFFSET = vec2( 50.0, 161.0 );
    const float DOMAIN = 69.0;
    const vec3 SOMELARGEFLOATS = vec3( 635.298681, 682.357502, 668.926525 );
    const vec3 ZINC = vec3( 48.500388, 65.294118, 63.934599 );

    // truncate the domain
    gridcell.xyz = gridcell.xyz - floor(gridcell.xyz * ( 1.0 / DOMAIN )) * DOMAIN;
    vec3 gridcell_inc1 = step( gridcell, vec3( DOMAIN - 1.5 ) ) * ( gridcell + 1.0 );

    // compute x*x*y*y for the 4 corners
    vec4 P = vec4( gridcell.xy, gridcell_inc1.xy ) + OFFSET.xyxy;
    P *= P;
    vec4 V1xy_V2xy = mix( P.xyxy, P.zwzw, vec4( v1_mask.xy, v2_mask.xy ) ); // apply mask for v1 and v2
    P = vec4( P.x, V1xy_V2xy.xz, P.z ) * vec4( P.y, V1xy_V2xy.yw, P.w );

    // get the lowz and highz mods
    vec3 lowz_mods = vec3( 1.0 / ( SOMELARGEFLOATS.xyz + gridcell.zzz * ZINC.xyz ) );
    vec3 highz_mods = vec3( 1.0 / ( SOMELARGEFLOATS.xyz + gridcell_inc1.zzz * ZINC.xyz ) );

    // apply mask for v1 and v2 mod values
    v1_mask = ( v1_mask.z < 0.5 ) ? lowz_mods : highz_mods;
    v2_mask = ( v2_mask.z < 0.5 ) ? lowz_mods : highz_mods;

    // compute the final hash
    hash_0 = fract( P * vec4( lowz_mods.x, v1_mask.x, v2_mask.x, highz_mods.x ) );
    hash_1 = fract( P * vec4( lowz_mods.y, v1_mask.y, v2_mask.y, highz_mods.y ) );
    hash_2 = fract( P * vec4( lowz_mods.z, v1_mask.z, v2_mask.z, highz_mods.z ) );
}

vec3 SimplexPerlin3D_Deriv(vec3 P)
{
    // calculate the simplex vector and index math
    vec3 Pi;
    vec3 Pi_1;
    vec3 Pi_2;
    vec4 v1234_x;
    vec4 v1234_y;
    vec4 v1234_z;
    Simplex3D_GetCornerVectors( P, Pi, Pi_1, Pi_2, v1234_x, v1234_y, v1234_z );

    // generate the random vectors
    vec4 hash_0;
    vec4 hash_1;
    vec4 hash_2;
    FAST32_hash_3D( Pi, Pi_1, Pi_2, hash_0, hash_1, hash_2 );
    hash_0 -= 0.49999;
    hash_1 -= 0.49999;
    hash_2 -= 0.49999;

    // normalize random gradient vectors
    vec4 norm = inversesqrt( hash_0 * hash_0 + hash_1 * hash_1 + hash_2 * hash_2 );
    hash_0 *= norm;
    hash_1 *= norm;
    hash_2 *= norm;

    // evaluate gradients
    vec4 grad_results = hash_0 * v1234_x + hash_1 * v1234_y + hash_2 * v1234_z;

    // evaluate the surflet f(x)=(0.5-x*x)^3
    vec4 m = v1234_x * v1234_x + v1234_y * v1234_y + v1234_z * v1234_z;
    m = max(0.5 - m, 0.0); // The 0.5 here is SIMPLEX_PYRAMID_HEIGHT^2
    vec4 m2 = m*m;
    vec4 m3 = m*m2;

    // calc the deriv
    vec4 temp = -6.0 * m2 * grad_results;
    float xderiv = dot( temp, v1234_x ) + dot( m3, hash_0 );
    float yderiv = dot( temp, v1234_y ) + dot( m3, hash_1 );
    float zderiv = dot( temp, v1234_z ) + dot( m3, hash_2 );

    const float FINAL_NORMALIZATION = 37.837227241611314102871574478976; // scales the final result to a strict 1.0->-1.0 range

    // sum with the surflet and return
    return vec3(xderiv, yderiv, zderiv) * FINAL_NORMALIZATION;
}

