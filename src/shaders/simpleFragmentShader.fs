#version 330

in  vec4 normal;

out vec4 out_Color;



void main(void)
{
	//in absence of any material, set everything just red
	out_Color = vec4(1.0,0.0,0.0,1.0);
}
