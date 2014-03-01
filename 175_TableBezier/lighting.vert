// positions are 4 component vectors (opengl will fill in the 4th component with 1)
attribute vec4 position;
attribute vec3 normal;

varying vec3 transformed_normal;
varying vec3 light_direction;

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 model_mat;
uniform mat3 normal_mat;
uniform vec4 material_color;

void main()
{
  //output the position for the fragment shader
  gl_Position = proj_mat * view_mat * model_mat * position;

  //output the color for the fragment shader
  gl_FrontColor = material_color;

  //output the transformed normal
  transformed_normal = normal_mat*normal;

  //direction of the light in camera coordinates
  light_direction = (view_mat * normalize(vec4(0.8,1,0.5,0))).xyz;
}
