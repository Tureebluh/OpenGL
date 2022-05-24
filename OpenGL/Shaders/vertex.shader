#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

out vec4 vColor;
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
	gl_Position = projection * view * model * position;
	vColor = vec4(clamp(position, 0.0f, 1.0f));

	texCoord = tex;

	normal = mat3(transpose(inverse(model))) * norm;

	fragPos = (model * position).xyz;
}