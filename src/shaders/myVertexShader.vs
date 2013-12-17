#version 330



in vec3 in_Position;
in vec3 in_Velocity;
in int in_Age;
out vec4 vertexPosition;

//Matrix Uniforms as specified with glUniformMatrix4fv. These matrices are the same for every vertex
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform vec4 LightPosition;
uniform float heightOfNearPlane;

void main(void)
{
    vec4 vertexPosition_modelspace = vec4(in_Position, 1.0);
    vec3 eye = vec3(0.0,0.0,4.0);
    float dist = length(vertexPosition_modelspace.xyz - eye);
	vertexPosition = (ViewMatrix * ModelMatrix) * vertexPosition_modelspace;
	gl_Position = ProjectionMatrix * vertexPosition;
    gl_PointSize = dist*0.1;

}
