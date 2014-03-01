// positions and colors are 4 component vectors (opengl will fill in the 4th component)
in vec4 position;
in vec4 color;

void main()
{
  //output the position for the fragment shader
  gl_Position = position;
  //output the color for the fragment shader
  gl_FrontColor = vec4(1,0,0,1);
}
