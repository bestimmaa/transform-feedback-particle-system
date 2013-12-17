#version 330

in vec3 in_Position;
in vec3 in_Velocity;
in int in_Age;


out vec3 out_Position;
out vec3 out_Velocity;
out int out_Age;

uniform int time;

void main(void)
{
    out_Position = in_Position+in_Velocity*0.001;
    out_Velocity = in_Velocity*0.99;
    int age = in_Age+1;
    out_Age = age;

}