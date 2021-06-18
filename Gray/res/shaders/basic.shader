#shader vertex

#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
out vec2 v_texCoord;


void main(){
   gl_Position = position ;
   v_texCoord = texCoord;
}

/* we transfer data from vertex texture to fragment shader using "varying",*/
/* just as we input data from the vertex shader using "in" and output it from fragment shader using "out"*/
/* we can also output data from vertex shader and input it to fragment shader*/



#shader fragment

#version 330 core
layout(location = 0) out vec4 color;


uniform vec4 u_Color;
uniform sampler2D u_Texture;
in vec2 v_texCoord;

void main(){
    vec4 texColor = texture(u_Texture, v_texCoord);
    u_Color;
    color = texColor;
}