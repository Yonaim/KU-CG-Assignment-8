#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 fragPos;
out vec3 fragNor;

void main()
{
	fragPos     = vec3(model * vec4(aPos, 1.0));
	fragNor     = mat3(transpose(inverse(model))) * aNor;
	gl_Position = proj * view * vec4(fragPos, 1.0);
}
