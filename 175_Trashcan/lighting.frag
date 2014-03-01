varying vec3 transformed_normal;
varying vec3 light_direction;
varying vec3 eye_vec;
uniform float ambient;
uniform float specular_coefficient;
uniform vec4 material_specular_color;
uniform vec4 light_color;
uniform bool specular_on;

void main()
{
  vec3 n = normalize(transformed_normal);
  vec3 l = normalize(light_direction);
  vec3 e = normalize(eye_vec);
  float diffuse = max(dot(l, n), 0.0);
  float specular = 0.0;
  if(diffuse > 0.0 && specular_on)
  {
	vec3 r = reflect(-l, n);
        specular = pow(max(dot(r, e), 0.0), specular_coefficient);
  }
  gl_FragColor = light_color*(gl_Color*(diffuse + ambient) + specular*material_specular_color);
}
