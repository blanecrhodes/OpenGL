#version 400 core

out vec4 color;
uniform vec4 lineColor;
uniform vec4 floorColor;

void main(){
	if(floorColor.x != 0.0f){
		color = floorColor;	
	} else{
		color = lineColor;
	}
}