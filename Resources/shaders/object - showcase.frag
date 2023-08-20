#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 fragment_position;

uniform vec3 view_position;
uniform sampler2D object_texture;
uniform vec3 flags;

out vec4 FragColor;

const vec3 LIGHT_POSITION = vec3(5.0);

vec4 getAmbientLight()
{
    const float AMBIENT_STRENGTH = 1.0;
    const vec3 AMBIENT_LIGHT = vec3(0.15, 0.15, 0.15);

    return vec4(AMBIENT_LIGHT * AMBIENT_STRENGTH, 1.0);
}

vec4 getDiffuseLight(vec3 norm, vec3 lightDirection)
{
    const vec3 DIFFUSE_LIGHT = vec3(1.0);

    float diff = max(dot(norm, lightDirection), 0.0);

    return vec4(diff * DIFFUSE_LIGHT, 1.0);
}

vec4 getSpecularLight(vec3 norm, vec3 lightDirection)
{
    const float SPECULAR_STRENGTH = 0.5;
    const vec3 SPECULAR_LIGHT = vec3(1.0);
    const int SPECULAR_INTESITY = 2 << 5;

    vec3 viewDir = normalize(view_position - fragment_position);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), SPECULAR_INTESITY);

    return vec4(SPECULAR_STRENGTH * spec * SPECULAR_LIGHT, 1.0);
}

float getLuminance(vec4 color)
{
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(LIGHT_POSITION - fragment_position);

    vec4 ambientLight = getAmbientLight();
    vec4 diffuseLight = getDiffuseLight(norm, lightDirection);
    vec4 specularLight = getSpecularLight(norm, lightDirection);
    
    vec4 light = ambientLight * flags.x + diffuseLight * flags.y + specularLight * flags.z;
    float luminance = getLuminance(light);

    FragColor = light * texture(object_texture, uv);
}