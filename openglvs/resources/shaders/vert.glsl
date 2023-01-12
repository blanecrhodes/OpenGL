#version 400 core 

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTexCoord;  
//layout (location = 2) in vec2 inTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
//out vec3 outColor;

void main(){
	texCoord = inTexCoord;
	//outColor = inColor;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}