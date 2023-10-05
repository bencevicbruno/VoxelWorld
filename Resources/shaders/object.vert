#version 330 core
layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec3 normalIn;
layout (location = 3) in vec4 colorIn;

uniform mat4 projection_view;
uniform mat4 model;
uniform float waveAmplifier;
uniform float time;

out vec2 uv;
out vec3 normal;
out vec3 fragment_position;
out vec4 color;

void main()
{
    float globalX = positionIn.x + model[3].x;
    float globalZ = positionIn.z + model[3].z;
    float waveHeight = abs(waveAmplifier * sin(globalX * 0.5 + time) * cos(globalZ * 0.5 + time));

    uv = uvIn;
    normal = mat3(transpose(inverse(model))) * normalIn;
    fragment_position = vec3(model * vec4(positionIn.x, positionIn.y + waveHeight, positionIn.z, 1.0));
    color = colorIn;

    gl_Position = projection_view * model * vec4(positionIn.x, positionIn.y + waveHeight, positionIn.z, 1.0);
} 