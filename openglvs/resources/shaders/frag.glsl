#version 400 core 

out vec4 color;
uniform vec4 u_Color;


in vec2 texCoord;
in vec3 outColor;

uniform sampler2D ourTexture;

void main(){
	//color = u_Color;
	color = texture(ourTexture, texCoord);

	//color = texture(ourTexture, texCoord) * vec4(outColor, 1.0f);
	//color = vec4(outColor, 1.0f);
}
