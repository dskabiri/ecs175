// positions are 4 component vectors (opengl will fill in the 4th component with 1)
attribute vec4 position;
attribute vec3 normal;
varying vec3 transformed_normal;
varying vec3 light_direction;
varying vec3 eye_vec;
varying vec4 spec_color;
uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 model_mat;
uniform mat3 normal_mat;
uniform vec4 material_color;
uniform bool point_light;

void main()
{
  //varyingput the position for the fragment shader
  gl_Position = proj_mat * view_mat * model_mat * position;

  //varyingput the color for the fragment shader
  gl_FrontColor = material_color;

  //varyingput the transformed normal
  transformed_normal = normal_mat*normal;
  
  //vector from the eye to the position in world space
  eye_vec = -(view_mat * model_mat * position).xyz;

  //transform the light to view space
  if(point_light)
  {
        light_direction = (view_mat * vec4(-1.5,1.4,0,1)).xyz + eye_vec;
  }
  else
  {
	light_direction = (view_mat * normalize(vec4(0.8,1,0.5,0))).xyz;
  }

}
