#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


#define WAVE_COUNT 2
const float PI = 3.14159265359;

uniform float t;
uniform vec2 direction[WAVE_COUNT];
float steepness[WAVE_COUNT];
uniform float amplitude[WAVE_COUNT];
uniform float waveLength[WAVE_COUNT];
uniform float speed[WAVE_COUNT];

void main()
{   
    float xPos = aPos.x;
    float yPos = aPos.y;
    float zPos = 0.0;
    float xNor = 0.0;
    float yNor = 0.0;
    float zNor = 0.0;
    for (int i = 0; i < WAVE_COUNT; i++) {
        float frequency = 6 * PI / waveLength[i];
        float phase = speed[i] * frequency;
        steepness[i] = 1 / frequency / amplitude[i] / 3;

        float theta = dot(direction[i], vec2(xPos, yPos));

        float A = steepness[i] * amplitude[i] * direction[i].x;
        xPos += A * cos(theta * frequency + t * phase);

        A = steepness[i] * amplitude[i] * direction[i].y;
        yPos += A * cos(theta * frequency + t * phase);

        zPos += amplitude[i] * sin(theta * frequency + t * phase);

    }

    for (int i = 0; i < WAVE_COUNT; i++) {
        float frequency = 2 * PI / waveLength[i];
        float WA = frequency * amplitude[i];
        float phase = speed[i] * frequency;
        float DP = dot(direction[i], vec2(xPos, yPos));
        float S = sin(frequency * DP + phase * t);
        float C = cos(frequency * DP + phase * t);

        xNor += direction[i].x * WA * C;
        yNor += direction[i].y * WA * C;
        zNor += steepness[i] * WA * S;

    }
    vec3 pos = vec3(xPos, zPos, yPos);
    vec3 normal = vec3(-xNor, 1 - zNor, -yNor);
    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * normalize(normal);  
	gl_Position = projection * view * model * vec4(pos, 1.0);
}