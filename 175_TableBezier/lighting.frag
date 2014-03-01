varying vec3 transformed_normal;
varying vec3 light_direction;

void main()
{
  float ambient = 0.2;
  vec4 light_color = vec4(1.0,1.0,1.0,1.0);
  vec3 n = normalize(transformed_normal);
  vec3 l = normalize(light_direction);
  float diffuse = max(dot(l, n), 0.0);
  gl_FragColor = light_color*(gl_Color*(diffuse + ambient));
}
