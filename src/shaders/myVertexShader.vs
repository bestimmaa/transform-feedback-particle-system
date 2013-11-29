#version 330



layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_Texcoord;
layout(location=3) in vec3 in_Tangent;
layout(location=4) in vec3 in_Bitangent;


out vec4 normal;
out vec4 vertexPosition;
out vec3 vertexPosition_worldspace;
out vec4 eye_direction_cameraspace;
out vec4 eye_direction_tangentspace;
out vec2 texcoord;
out vec4 tangent;
out vec4 bitangent;
out vec4 light_direction_cameraspace;
out vec4 light_direction_tangentspace;
out vec3 light_position_worldspace;
out vec3 debug;

//Matrix Uniforms as specified with glUniformMatrix4fv. These matrices are the same for every vertex
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform vec4 LightPosition;




void main(void)
{

	vec4 vertexPosition_modelspace = vec4(in_Position, 1.0);
    vec4 vertexPosition_cameraspace = ViewMatrix * vec4(in_Position, 1.0);
    vertexPosition_worldspace = (ModelMatrix * vertexPosition_modelspace).xyz;
	vertexPosition = (ViewMatrix * ModelMatrix) * vertexPosition_modelspace;
	gl_Position = ProjectionMatrix * vertexPosition;
    //transform the normal with the normal matrix (we can't use the modeviewmatrix for this)
	normal      = NormalMatrix * vec4(normalize(in_Normal), 0.0);
    tangent = vec4(normalize(in_Tangent), 0.0);
    bitangent = vec4(normalize(in_Bitangent), 0.0);;

	eye_direction_cameraspace = -vertexPosition_cameraspace;

    vec4 lightPosition_cameraspace = ViewMatrix * LightPosition;
    light_direction_cameraspace = lightPosition_cameraspace + eye_direction_cameraspace;

    //pass the texture coordinate for the vertex
	texcoord = in_Texcoord;

    vec3 normal_cameraspace = mat3(ModelViewMatrix)*normalize(in_Normal);
    vec3 tangent_cameraspace = mat3(ModelViewMatrix)*normalize(in_Tangent);
    vec3 bitangent_cameraspace = mat3(ModelViewMatrix)*normalize(in_Bitangent);
    mat3 TBN = transpose(mat3(tangent_cameraspace,bitangent_cameraspace,normal_cameraspace));
    debug = bitangent_cameraspace;
    light_direction_tangentspace = vec4(TBN * vec3(light_direction_cameraspace.xyz),0);
    eye_direction_tangentspace = vec4(TBN * vec3(eye_direction_cameraspace.xyz),0);
    light_position_worldspace = LightPosition.xyz;
    
}
