#pragma once

#include <glm/glm.hpp>

struct RenderParams
{
	glm::mat4 model, view, proj;
	glm::vec3 eye, lightPos, mat_ka, mat_kd, mat_ks;
	glm::vec3 lightColor;
	float     mat_shininess, ambient_intensity, gamma;
};
