#type vertex
#version 410 core
			
layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_TexCords;

uniform mat4 u_ProjectionViewMatrix;

out vec2 v_TexCords;

void main(){
	gl_Position =  u_ProjectionViewMatrix * vec4(a_Position, 1.0);
	v_TexCords = a_TexCords;
}

#type fragment
#version 410 core

layout(location=0) out vec4 o_Color;
in vec2 v_TexCords;

uniform vec3 u_LightColor;
uniform sampler2D u_Texture;

void main(){
	o_Color = vec4(u_LightColor, 1.0f) * texture(u_Texture, v_TexCords);
}