#version 330

in vec3 in_Position;
in vec3 in_Velocity;
in int in_Age;


out vec3 out_Position;
out vec3 out_Velocity;
out int out_Age;

uniform float Random;

float rand(float f) {
    return fract(456.789*sin(789.123*f*Random)*(1.+f));
}

void main(void)
{
    vec3 gravity = vec3(0,-1,0);
    out_Position = in_Position+(in_Velocity)+(gravity);
    out_Velocity = in_Velocity*0.999;
    out_Age = in_Age+1;
    if (out_Age > 5000) {
        out_Position = vec3(0.0,0.0,0.0);
        out_Age = 0;
        out_Velocity = vec3(0.0,1.0,0.0);
    }

}