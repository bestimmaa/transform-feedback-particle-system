#version 330



layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_Texcoord;

out vec4 normal;
out vec4 position;
out vec4 direction;
out vec2 texcoord;

//Matrix Uniforms as specified with glUniformMatrix4fv

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;



void main(void)
{

	vec4 vertexPos = vec4(in_Position, 1.0);

		//^= old ModelViewProjectionMatrix
	position = ModelViewMatrix * vertexPos;
	gl_Position = ProjectionMatrix * position;
	normal      = NormalMatrix * vec4(normalize(in_Normal), 0.0);
	direction = -position;
	texcoord = in_Texcoord;
}
