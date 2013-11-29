#version 330



layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_Texcoord;
layout(location=3) in vec3 in_Tangent;
layout(location=4) in vec3 in_Bitangent;


out vec4 normal;
out vec4 position_cameraspace;
out vec4 eye_direction_cameraspace;
out vec2 texcoord;
out vec4 tangent;
out vec4 bitangent;
out vec4 light_direction_cameraspace;

//Matrix Uniforms as specified with glUniformMatrix4fv. These matrices are the same for every vertex

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform mat3 MVP3; // mvp matrix without translation part for transforming tangents?
uniform vec4 LightPosition;




void main(void)
{

	vec4 vertexPos = vec4(in_Position, 1.0);

	//calculate and set the position of the vertex
	position_cameraspace = ModelViewMatrix * vertexPos;
	gl_Position = ProjectionMatrix * position_cameraspace;
    vec4 lightPosition_cameraspace = ModelViewMatrix * LightPosition;
    light_direction_cameraspace = lightPosition_cameraspace + eye_direction_cameraspace;
    //transform the normal with the normal matrix (we can't use the modeviewmatrix for this)
	normal      = NormalMatrix * vec4(normalize(in_Normal), 0.0);
    tangent = vec4(normalize(in_Tangent), 0.0);
    bitangent = vec4(normalize(in_Bitangent), 0.0);;
    //invert the light position to obtain direction
	eye_direction_cameraspace = -position_cameraspace;
    //pass the texture coordinate for the vertex
	texcoord = in_Texcoord;
    vec3 normal_cameraspace = MVP3*normalize(in_Normal);
    vec3 tangent_cameraspace = MVP3*normalize(in_Tangent);
    vec3 bitangent_cameraspace = MVP3*normalize(in_Bitangent);
    mat3 TBN = transpose(mat3(tangent_cameraspace,bitangent_cameraspace,normal_cameraspace));
    
    
    
    
}
