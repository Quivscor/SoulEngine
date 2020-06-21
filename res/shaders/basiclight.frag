#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 normal;
in vec4 FragPosLightSpace;
in vec3 pPos;
struct Material
{
	sampler2D texture_diffuse1;

	sampler2D texture_specular1;
	
	 sampler2D shadowMap;

	float shininess;
	float transparency;
};
struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight 
{
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 

    float constant;
    float linear;
    float quadratic;      
};

uniform Material material;
uniform PointLight point_light;
uniform DirLight dir_light;
uniform vec3 view_pos;
uniform vec3 lightPos;
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(material.shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(normal);
   vec3 lightDir = normalize(-dir_light.direction);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(material.shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(material.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
	vec3 color = texture(material.texture_diffuse1,TexCoords).rgb;
  //  return (ambient + diffuse + specular);
	  float shadow = ShadowCalculation(FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;  
	return 	lighting;

}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir, vec3 WorldPos)
{
	vec3 light_dir = normalize(light.position - WorldPos);
	//Diffuse
	float diff = max(dot(normal, light_dir), 0.0);
	//Specular 
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	//Attenuation
	float distance = length(light.position - WorldPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));


	return (ambient + diffuse + specular);
}

void main()
{
    float transparency = 1.0;
    //if((WorldPos.z > pPos.z) && (WorldPos.y > 0.5))
    //{
    //    transparency = 0.2;//smoothstep(pPos.z, pPos.z + 2.0, WorldPos.z);
    //}
	vec3 view_dir = normalize(view_pos - WorldPos);
    vec3 result = CalcDirLight(dir_light, normal, view_dir);
	vec4 calc_color = vec4( result, transparency);
	calc_color *= material.transparency;
	
	FragColor = calc_color * vec4(Color, 1.0f);

}