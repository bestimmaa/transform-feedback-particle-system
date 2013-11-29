#version 330

uniform sampler2D colorMap;
uniform sampler2D normalMap;

in  vec4 normal;
in  vec4 position_cameraspace;
in  vec4 eye_direction_cameraspace;
in  vec4 light_direction_cameraspace;
in  vec2 texcoord;


out vec4 out_Color;



void main(void)
{

        vec4 color = texture(colorMap, texcoord);
        //vec4 color = vec4(0.3,0.2,1.0,1.0);
    
        vec3 lightPosition = vec3(0.0,10.0,0.0);
        vec4 AmbientColor = vec4(0.0, 0.0, 0.0, 1.0);
        vec4 DiffuseColor = vec4(0.0, 0.0, 0.0, 1.0);

        float AmbientFactor = 0.1;

        out_Color = color * AmbientFactor; // set the color to the color of the texture at the corresponding texture coordinate and aspply the ambient factor

        
        vec3 vertexToLightVector =  normalize(lightPosition - position_cameraspace.xyz);
        float DiffuseFactor = clamp(dot(normal.xyz, vertexToLightVector), 0.0, 1.0);

        out_Color = out_Color +  color*DiffuseFactor; // apply diffuse term to color


        vec3 normalizedNormal = normalize(normal.xyz);
        vec3 viewDirection = normalize(eye_direction_cameraspace.xyz);
        vec3 lightDirection = normalize(lightPosition - position_cameraspace.xyz);
        vec3 reflectedLightDirection = reflect(-lightDirection, normalizedNormal);

        int phongReflectionRadius = 90; // radius of the phong reflections


        float phongFactor = pow(max(dot(viewDirection, reflectedLightDirection),0.0),phongReflectionRadius);

        out_Color = out_Color + color * phongFactor; // add phong highlight

        

}
