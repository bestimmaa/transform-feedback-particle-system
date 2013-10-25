#version 330



layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_Texcoord;

out vec4 normal;
out vec4 position;
out vec4 direction;
out vec2 texcoord;

//Matrix Uniforms as specified with glUniformMatrix4fv. These matrices are the same for every vertex

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;



void main(void)
{

	vec4 vertexPos = vec4(in_Position, 1.0);

	//calculate and set the position of the vertex
	position = ModelViewMatrix * vertexPos;
	gl_Position = ProjectionMatrix * position;
    //transform the normal with the normal matrix (we can't use the modeviewmatrix for this)
	normal      = NormalMatrix * vec4(normalize(in_Normal), 0.0);
    //invert the light position to obtain direction
	direction = -position;
    //pass the texture coordinate for the vertex
	texcoord = in_Texcoord;
}
