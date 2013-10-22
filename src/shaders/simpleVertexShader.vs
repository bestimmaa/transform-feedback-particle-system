#version 330

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
out vec4 normal;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;


void main(void)
{
	//^= old ModelViewProjectionMatrix

	gl_Position = (ProjectionMatrix * ModelViewMatrix) * vec4(in_Position,1.0);
	normal      = NormalMatrix * vec4(normalize(in_Normal), 0.0);
}
