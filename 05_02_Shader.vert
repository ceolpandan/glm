// Shader-ul de varfuri  
 
 #version 400

in vec4 in_Position;
in vec4 in_Color;

out vec4 gl_Position; 
out vec4 out_Color;

uniform mat4 myMatrix;
 


void main(void)
  {
    gl_Position = myMatrix*in_Position;
    out_Color = in_Color;
   } 
 