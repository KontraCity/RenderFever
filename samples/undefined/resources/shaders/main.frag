#version 330 core

struct FragProperties {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
};

struct Material {
    sampler2D texture;
    sampler2D specular;
    float shininess;
};

struct LightingProperties {
    float ambient;
    float diffuse;
    float specular;
};

struct LightingAttenuation {
    float constant;
    float linear;
    float quadratic;
};

struct LightingCutoff {
    float inner;
    float outer;
};

struct PointLight {
    vec3 position;
    vec3 color;
    LightingProperties properties;
    LightingAttenuation attenuation;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    LightingProperties properties;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    LightingProperties properties;
    LightingAttenuation attenuation;
    LightingCutoff cutoff;
};

in FragProperties ioFragProperties;
in DirectionalLight ioDirectionalLight;
in PointLight ioPointLight;
in SpotLight ioSpotLight;

out vec4 ioFragColor;

uniform Material uMaterial;

vec3 CalcDirectionalLight(DirectionalLight light) {
    // Ambient lighting
    vec3 ambient = light.color * light.properties.ambient * vec3(texture(uMaterial.texture, ioFragProperties.texCoords));

    // Diffuse lighting
    vec3 normal = normalize(ioFragProperties.normal);
    vec3 lightDirection = normalize(-light.direction);
    float diffuseValue = max(0.0f, dot(normal, lightDirection));
    vec3 diffuse = light.color * light.properties.diffuse * diffuseValue * vec3(texture(uMaterial.texture, ioFragProperties.texCoords));

    // Specular lighting
    vec3 viewDirection = normalize(-ioFragProperties.position);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(0.0f, dot(viewDirection, reflectDirection)), uMaterial.shininess);
    vec3 specular = light.color * light.properties.specular * specularValue * vec3(texture(uMaterial.specular, ioFragProperties.texCoords));
    
    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 CalcPointLight(PointLight light) {
    // Ambient lighting
    vec3 ambient = light.color * light.properties.ambient * texture(uMaterial.texture, ioFragProperties.texCoords).rgb;

    // Diffuse lighting
    vec3 normal = normalize(ioFragProperties.normal);
    vec3 lightDirection = normalize(light.position - ioFragProperties.position);
    float diffuseValue = max(0.0f, dot(lightDirection, normal));
    vec3 diffuse = light.color * light.properties.diffuse * diffuseValue * texture(uMaterial.texture, ioFragProperties.texCoords).rgb;

    // Specular lighting
    vec3 viewDirection = normalize(-ioFragProperties.position);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(0.0f, dot(viewDirection, reflectDirection)), uMaterial.shininess);
    vec3 specular = light.color * light.properties.specular * specularValue * texture(uMaterial.specular, ioFragProperties.texCoords).rgb;
    
    // Attenuation
    float distanceToFrag = length(light.position - ioFragProperties.position);
    float linear = light.attenuation.linear * distanceToFrag;
    float quadratic = light.attenuation.quadratic * (distanceToFrag * distanceToFrag);
    float divider = light.attenuation.constant + linear + quadratic;
    float attenuation = divider == 0.0f ? 0.0f : 1.0f / divider;

    vec3 result = ambient + diffuse + specular;
    return result * attenuation;
}

vec3 CalcSpotLight(SpotLight light) {
    // Ambient lighting
    vec3 ambient = light.color * light.properties.ambient * texture(uMaterial.texture, ioFragProperties.texCoords).rgb;

    // Diffuse lighting
    vec3 normal = normalize(ioFragProperties.normal);
    vec3 lightDirection = normalize(light.position - ioFragProperties.position);
    float diffuseValue = max(0.0f, dot(normal, lightDirection));
    vec3 diffuse = light.color * light.properties.diffuse * diffuseValue * texture(uMaterial.texture, ioFragProperties.texCoords).rgb;

    // Specular lighting
    vec3 viewDirection = normalize(-ioFragProperties.position);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(0.0f, dot(viewDirection, reflectDirection)), uMaterial.shininess);
    vec3 specular = light.color * light.properties.specular * specularValue * texture(uMaterial.specular, ioFragProperties.texCoords).rgb;
    
    // Spot lighting
    float theta = dot(normalize(-light.direction), lightDirection);
    float spotlight = smoothstep(light.cutoff.outer, light.cutoff.inner, theta);

    // Attenuation
    float distanceToFrag = length(light.position - ioFragProperties.position);
    float linear = light.attenuation.linear * distanceToFrag;
    float quadratic = light.attenuation.quadratic * (distanceToFrag * distanceToFrag);
    float divider = light.attenuation.constant + linear + quadratic;
    float attenuation = divider == 0.0f ? 0.0f : 1.0f / divider;

    vec3 result = ambient + (diffuse + specular) * spotlight;
    return result * attenuation;
}

void main() {
    vec3 directionalLight = CalcDirectionalLight(ioDirectionalLight);
    vec3 pointLight = CalcPointLight(ioPointLight);
    vec3 spotLight = CalcSpotLight(ioSpotLight);
    ioFragColor = vec4(directionalLight + pointLight + spotLight, 1.0f);
}
