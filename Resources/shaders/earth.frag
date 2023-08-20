#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 fragment_position;

uniform vec3 view_position;
uniform sampler2D earth_texture;
uniform sampler2D clouds_texture;
uniform sampler2D night_texture;
uniform sampler2D specular_texture;
uniform vec4 highlight;

out vec4 FragColor;

const vec3 LIGHT_POSITION = vec3(0);

const float AMBIENT_STRENGTH = 1.0;
const vec3 AMBIENT_LIGHT = vec3(0.2, 0.2, 0.3);

const vec3 DIFFUSE_LIGHT = vec3(1.0);

const float SPECULAR_STRENGTH = 0.5;
const vec3 SPECULAR_LIGHT = vec3(1.0);
const int SPECULAR_INTESITY = 8; // always power of 2

vec4 getAmbientLight()
{
    return vec4(AMBIENT_LIGHT * AMBIENT_STRENGTH, 1.0);
}

vec4 getDiffuseLight(vec3 norm, vec3 lightDirection)
{
    float diff = max(dot(norm, lightDirection), 0.0);

    return vec4(diff * DIFFUSE_LIGHT, 1.0);
}

vec4 getSpecularLight(vec3 norm, vec3 lightDirection)
{
    vec3 viewDir = normalize(view_position - fragment_position);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), SPECULAR_INTESITY);

    vec4 mask = texture(specular_texture, uv);

    return vec4(SPECULAR_STRENGTH * spec * SPECULAR_LIGHT, 1.0) * mask;
}

float getLuminance(vec4 color)
{
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

vec4 getTexture(float luminance)
{
    vec4 dayTexture = (texture(earth_texture, uv) + texture(clouds_texture, uv)) * luminance;
    vec4 nightTexture = texture(night_texture, uv) * (1 - luminance);

    return dayTexture + nightTexture;
}

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(LIGHT_POSITION - fragment_position);

    vec4 ambientLight = getAmbientLight();
    vec4 diffuseLight = getDiffuseLight(norm, lightDirection);
    vec4 specularLight = getSpecularLight(norm, lightDirection);
    
    vec4 light = ambientLight + diffuseLight + specularLight;
    float luminance = getLuminance(light);

    FragColor = light * getTexture(luminance) + highlight;
}