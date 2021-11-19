
// Shader-ul de fragment / Fragment shader  
 
 #version 400

out vec4 FragColor;
uniform int codCol;
 
in vec4 out_Color;

void main(void)
  {
	if ( codCol==0 )
		FragColor = out_Color;
	if ( codCol==1 )
		FragColor=vec4 (0.0, 0.0, 1.0, 1.0);
	if ( codCol==2 )
		FragColor=vec4 (1.0, 0.0, 0.0, 1.0);
	if ( codCol==3 )
		FragColor=vec4 (0.0, 1.0, 0.0, 1.0);

  }
 