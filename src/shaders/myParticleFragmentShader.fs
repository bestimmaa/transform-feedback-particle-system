#version 330

in vec4 particleColor;

out vec4 out_Color;

void main(void)
{

    out_Color = vec4(particleColor.x,particleColor.y,particleColor.z,particleColor.w);

}
