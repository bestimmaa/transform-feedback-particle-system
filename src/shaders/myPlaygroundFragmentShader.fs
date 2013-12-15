#version 330

uniform sampler2D colorMap;
uniform sampler2D normalMap;

in  vec4 normal;
in  vec4 vertexPosition;
in  vec3 vertexPosition_worldspace;
in  vec4 eye_direction_cameraspace;
in  vec4 eye_direction_tangentspace;
in  vec4 light_direction_cameraspace;
in  vec4 light_direction_tangentspace;
in  vec3 light_position_worldspace;
in  vec2 texcoord;
in vec3 debug;

out vec4 out_Color;



void main(void)
{
    
    out_Color = vec4(1,1,1,1);
    

}
