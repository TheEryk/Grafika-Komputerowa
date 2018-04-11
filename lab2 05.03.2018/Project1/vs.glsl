// definicja wersji
#version 330

layout (location = 0) in vec3 inPosition; 
layout (location = 1) in vec3 inColor;
out vec3 kolorek;
uniform float dX;
uniform float dY;
void main()
{	
	// przypisz pozycje do zmiennej wbudowanej OpenGL
	gl_Position = vec4(inPosition.x+dX,inPosition.y+dY,inPosition.z, 1.0); 
	kolorek = inColor;
}