#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform float A;

vec4 U;
vec4 V;
vec4 N;



layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

smooth out vec3 vNormal; 
smooth out vec4 kolorek;

void main()
{
	gl_Position = vec4(inPosition.x, (10,0-sqrt(inPosition.x*inPosition.x+inPosition.z*inPosition.z))*sin(A*(10.0 - sqrt(inPosition.x*inPosition.x + inPosition.z * inPosition.z)))  , inPosition.z, 1.0);
	
	U.x =inPosition.x+0.001;
	U.y =(10,0-sqrt((inPosition.x+0.001)*(inPosition.x+0.001)+inPosition.z*inPosition.z))*sin(A*(10.0 - sqrt((inPosition.x+0.001)*(inPosition.x+0.001) + inPosition.z * inPosition.z)));
	U.z =inPosition.z;
	U.w = 1.0;

	V.x =inPosition.x;
	V.y =(10,0-sqrt(inPosition.x*inPosition.x+(inPosition.z+0.001)*(inPosition.z+0.001)))*sin(A*(10.0 - sqrt(inPosition.x*inPosition.x + (inPosition.z+0.001) * (inPosition.z+0.001))));
	V.z =inPosition.z+0.001;
	V.w = 1.0;

	N=U*V;

	float L = sqrt(N.x * N.x + N.y * N.y + N.z * N.z);
	if (L < 0.01) L = 0.01;

	// wyznacz wspolrzedne normalnej 
	N.x /= L;
	N.y /= L;
	N.z /= L;


	N= N*modelViewMatrix*gl_Position;

	gl_Position = projectionMatrix*modelViewMatrix*gl_Position;	
	//vec4 vRes = normalMatrix*vec4(inNormal, 0.0);
	vNormal = N.xyz; 
	kolorek = vec4(inColor,1.0);	
}