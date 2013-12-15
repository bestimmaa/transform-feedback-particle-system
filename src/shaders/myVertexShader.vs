#version 330



layout(location=0) in vec3 in_Position;
out vec4 vertexPosition;

//Matrix Uniforms as specified with glUniformMatrix4fv. These matrices are the same for every vertex
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform vec4 LightPosition;

void main(void)
{
	gl_Position = vec4(in_Position,0);

}
