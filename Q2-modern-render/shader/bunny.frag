#version 330 core
in vec3  fragPos;
in vec3  fragNor;
out vec4 FragColor;

uniform vec3 lightDir; // normalize(-1,-1,-1)
uniform vec3 viewPos;  // (0,0,0)

const vec3  ka        = vec3(1.0);
const vec3  kd        = vec3(1.0);
const vec3  ks        = vec3(0.0); // Q2 기본값: Specular 없음
const float shininess = 20.0;
const vec3  Ia        = vec3(0.2);

void main()
{
	vec3 N = normalize(fragNor);
	vec3 L = normalize(-lightDir);
	vec3 V = normalize(viewPos - fragPos);

	vec3  ambient  = Ia * ka;
	float diff     = max(dot(N, L), 0.0);
	vec3  diffuse  = kd * diff;
	vec3  H        = normalize(L + V);
	float spec     = pow(max(dot(N, H), 0.0), shininess);
	vec3  specular = ks * spec;

	vec3 color = ambient + diffuse + specular;
	FragColor  = vec4(color, 1.0);
}
