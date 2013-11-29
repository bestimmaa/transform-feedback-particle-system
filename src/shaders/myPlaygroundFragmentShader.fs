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
    
    // compute derivations of the world position
    vec3 p_dx = dFdx(vertexPosition_worldspace);
    vec3 p_dy = dFdy(vertexPosition_worldspace);
    // compute derivations of the texture coordinate
    vec2 tc_dx = dFdx(texcoord);
    vec2 tc_dy = dFdy(texcoord);
    // compute initial tangent and bi-tangent
    vec3 t = normalize( tc_dy.y * p_dx - tc_dx.y * p_dy );
    vec3 b = normalize( tc_dy.x * p_dx - tc_dx.x * p_dy ); // sign inversion
    // get new tangent from a given mesh normal
    vec3 norm = normalize(vec3(normal.xyz));
    vec3 x = cross(norm, t);
    t = cross(x, norm);
    t = normalize(t);
    // get updated bi-tangent
    x = cross(b, norm);
    b = cross(norm, x);
    b = normalize(b);
    mat3 tbn = mat3(t, b, norm);

    
    
    vec3 color = texture(colorMap, texcoord).rgb;
	vec3 LightColor = vec3(1,1,1);

	float LightPower = 40.0;

    // Material properties
	vec3 MaterialDiffuseColor = texture( colorMap, texcoord ).rgb;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(1,1,1) * 0.3;
    vec2 normalXY = vec2(texture(normalMap,texcoord).rg * 2.0f -1.0f);
    vec3 normalFromMap = normalize(texture(normalMap, texcoord).rgb* 2.0 - 1.0);
    vec3 correctNormal = vec3(tbn * normalFromMap);
    vec3 combinedNormal = normalize(correctNormal);

    vec3 lightDirection = normalize(light_direction_cameraspace.xyz);
    float NdotL = max(dot(lightDirection, combinedNormal), 0.0);
    //specular term
    float specular = 0.3;
    float I_n = 1.0f;
    //Blinn-Phong coefficients
    float I_a = 0.3f;
    float I_p = 1.0f;
    float I_d = 0.3f;
    float k_a = 0.3f;
    float k_d = 1.0f;
    float k_s = 0.8f;
    float m   = 50;
    
    if(NdotL > 0.0)
    {
        //determine viewdirection
        vec3 viewDir = normalize(-eye_direction_cameraspace.xyz);

        // half direction as approximation
        vec3 H = normalize(light_direction_cameraspace.xyz + viewDir);
        vec3 H2 = normalize(H*combinedNormal);
        vec3 halfDir = normalize(light_direction_cameraspace.xyz + viewDir);
        
        //angle of specular light
        float specAngle = max(dot(halfDir, combinedNormal), 0.0);
        
        //smooth and large specular area
        specular = pow(specAngle, m);
    }
    
    //lookup of texture color at uv-position "texCoords"
    vec4 textureColor = vec4(texture(colorMap, texcoord).xyz, 0.2f ) ;
    
    vec4 I_ambient = textureColor*k_a;
    vec4 I_diffus = k_d*NdotL*textureColor;
    //vec4 I_specular = textureColor*k_s*;
    
    //blend texture colors
    out_Color = I_ambient;
    out_Color += I_diffus;
    //out_Color += I_specular;
    

}
