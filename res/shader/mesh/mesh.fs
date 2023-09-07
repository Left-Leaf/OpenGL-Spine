#version 330 core

out vec4 FragColor;

void main()
{    
    
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1-smoothstep(0.99,1.0,gl_FragCoord.z));
}