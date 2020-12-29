#version 410 core

// Pass-through
in vec3 inPosition;
in vec3 inNormal;
out vec3 exNormal;

uniform mat4 modelViewMatrix;

void main()
{
    mat3 normalMatrix1 = mat3(modelViewMatrix);
	exNormal = normalMatrix1 * inNormal; // Phong

    gl_Position = modelViewMatrix * vec4(inPosition, 1.0);
}

