#version 330



layout(location=0) in vec3 in_Position;
out vec3 out_Position;

void main(void)
{
    out_Position = in_Position+vec3(0,-0.0001,0);
}