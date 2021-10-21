#version 330

in vec3 t_position;
in vec2 t_uv;
in vec4 t_color;
in vec3 t_normal;

out vec4 o_color;

uniform vec3 camera_position;
uniform sampler2D samp;

void main() {
    vec3 point_light = vec3(0.0, 0.5, -1);
    vec3 direction = t_position - point_light;
    float distance = length(direction);
    direction = normalize(direction); 
    float diffuse = max(dot(-direction, t_normal), 0.0) * 10;

    vec3 camera_to_light = normalize(point_light - camera_position);
    vec3 reflection = reflect(-camera_to_light, t_normal);
    float specular_factor = max(0.0, dot(reflection, -direction));
    specular_factor = pow(specular_factor, specular_factor) * 10;

    float attenuation = distance * distance * 0.43 + distance * 0.1 + 0.5;

    o_color = texture(samp, t_uv) * t_color * (diffuse + specular_factor) / attenuation;
}
