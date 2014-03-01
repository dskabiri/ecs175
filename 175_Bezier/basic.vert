// positions are 4 component vectors (opengl will fill in the 4th component)
attribute vec4 position;
uniform vec4 color;

void main()
{
  //output the position for the fragment shader
  gl_Position = position;
  //output the color for the fragment shader
  gl_FrontColor = color;
}
