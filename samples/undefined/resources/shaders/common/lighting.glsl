#include "essentials.glsl"
#include "material.glsl"

// Light type
const int DirectionalLightType = 0;
const int PointLightType       = 1;
const int SpotLightType        = 2;

struct Light {
    // Properties
    int type;
    float ambientProperty;
    float diffuseProperty;
    float specularProperty;

    vec3 position;      // Point and spot lights only
    float _padding1;
    vec3 direction;     // Directional and spot lights only
    float _padding2;
    vec3 color;
    
    // Attenuation
    float constantAttenuation;  // Point and spot lights only
    float linearAttenuation;    // Point and spot lights only
    float quadraticAttenuation; // Point and spot lights only

    // Cutoff
    float spotInnerCutoff;  // Spot light only
    float spotOuterCutoff;  // Spot light only
};

vec3 CalcDirectionalLight(in FragProperties fragProperties, in Light light, in Material material) {
    // Ambient lighting
    // TODO: Implement one place where completeness is being checked?
    vec3 ambient = light.color * light.ambientProperty * (material.complete ? texture(material.diffuse, fragProperties.texCoords).rgb : vec3(0.7f));

    // Diffuse lighting
    vec3 normal = normalize(fragProperties.normal);
    vec3 lightDirection = normalize(-light.direction);
    float diffuseValue = max(0.0f, dot(normal, lightDirection));
    vec3 diffuse = light.color * light.diffuseProperty * diffuseValue * (material.complete ? texture(material.diffuse, fragProperties.texCoords).rgb : vec3(0.7f));

    // Specular lighting
    vec3 viewDirection = normalize(-fragProperties.position);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(0.0f, dot(viewDirection, reflectDirection)), material.shininess);
    vec3 specular = light.color * light.specularProperty * specularValue * vec3(texture(material.specular, fragProperties.texCoords));
    
    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 CalcPointLight(in FragProperties fragProperties, in Light light, in Material material) {
    // Ambient lighting
    vec3 ambient = light.color * light.ambientProperty * (material.complete ? texture(material.diffuse, fragProperties.texCoords).rgb : vec3(0.7f));

    // Diffuse lighting
    vec3 normal = normalize(fragProperties.normal);
    vec3 lightDirection = normalize(light.position - fragProperties.position);
    float diffuseValue = max(0.0f, dot(lightDirection, normal));
    vec3 diffuse = light.color * light.diffuseProperty * diffuseValue * (material.complete ? texture(material.diffuse, fragProperties.texCoords).rgb : vec3(0.7f));

    // Specular lighting
    vec3 viewDirection = normalize(-fragProperties.position);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(0.0f, dot(viewDirection, reflectDirection)), material.shininess);
    vec3 specular = light.color * light.specularProperty * specularValue * texture(material.specular, fragProperties.texCoords).rgb;
    
    // Attenuation
    float distanceToFrag = length(light.position - fragProperties.position);
    float linear = light.linearAttenuation * distanceToFrag;
    float quadratic = light.quadraticAttenuation * (distanceToFrag * distanceToFrag);
    float divider = light.constantAttenuation + linear + quadratic;
    float attenuation = divider == 0.0f ? 0.0f : 1.0f / divider;

    vec3 result = ambient + diffuse + specular;
    return result * attenuation;
}

vec3 CalcSpotLight(in FragProperties fragProperties, in Light light, in Material material) {
    // Ambient lighting
    vec3 ambient = light.color * light.ambientProperty * (material.complete ? texture(material.diffuse, fragProperties.texCoords).rgb : vec3(0.7f));

    // Diffuse lighting
    vec3 normal = normalize(fragProperties.normal);
    vec3 lightDirection = normalize(light.position - fragProperties.position);
    float diffuseValue = max(0.0f, dot(normal, lightDirection));
    vec3 diffuse = light.color * light.diffuseProperty * diffuseValue * (material.complete ? texture(material.diffuse, fragProperties.texCoords).rgb : vec3(0.7f));

    // Specular lighting
    vec3 viewDirection = normalize(-fragProperties.position);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(0.0f, dot(viewDirection, reflectDirection)), material.shininess);
    vec3 specular = light.color * light.specularProperty * specularValue * texture(material.specular, fragProperties.texCoords).rgb;
    
    // Spot lighting
    float theta = dot(normalize(-light.direction), lightDirection);
    float spotlight = smoothstep(light.spotOuterCutoff, light.spotInnerCutoff, theta);

    // Attenuation
    float distanceToFrag = length(light.position - fragProperties.position);
    float linear = light.linearAttenuation * distanceToFrag;
    float quadratic = light.quadraticAttenuation * (distanceToFrag * distanceToFrag);
    float divider = light.constantAttenuation + linear + quadratic;
    float attenuation = divider == 0.0f ? 0.0f : 1.0f / divider;

    vec3 result = ambient + (diffuse + specular) * spotlight;
    return result * attenuation;
}
