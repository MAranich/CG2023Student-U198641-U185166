// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_world_normal; 
varying vec2 v_uv;
varying vec3 v_world_position; 

uniform sampler2D u_tex;
uniform sampler2D u_spec_tex;
uniform float u_time;
uniform mat4 u_model;
uniform vec3 u_camerapos;
uniform mat4 u_viewprojection;
uniform vec3 u_ambientintensity;
uniform vec3 u_ks;
uniform vec3 u_kd;
uniform vec3 u_ka;
uniform float u_shiny;
uniform vec3 u_lightpos;
uniform vec3 u_intensitydiff;
uniform vec3 u_intensityspec;

float div255 = 0.003921568; // = 1/255

void main()
{
	// Set the ouput color per pixel
	
	float ft = fract(u_time * 0.333); 
	float fft = fract(u_time * 0.333) * 2 - 1; 

	vec3 color = normalize(v_world_normal);

	
	if(v_uv.x <= 1 && 0 <= v_uv.x && v_uv.y <= 1 && 0 <= v_uv.y){
		color = texture2D(u_tex, v_uv).xyz; 
	}
	
	//La
	vec3 intensity = mix(u_ka * div255, color, ft) * u_ambientintensity * div255; 

	//Ld
	vec3 L = normalize(u_lightpos - v_world_position);
	intensity = intensity + color * max(dot(L, v_world_normal), 0) * u_intensitydiff * div255;
	// * 255 is to adjust the range from [0, 1] to [0, 255] (Kd must be in [0, 255] to work properly)

	//Ls
	vec3 V = normalize(v_world_position - u_camerapos);
	vec3 R = reflect(L, v_world_normal); 
	vec4 aux = texture2D(u_spec_tex, v_uv); 
	vec3 ks = aux.rgb * aux.a * 4; 
	intensity = intensity + ks * u_intensityspec * pow(max(dot(R, V), 0), u_shiny) * div255;


	//gl_FragColor = vec4(intensity, 1.0 );
	gl_FragColor = vec4( color * intensity, 1.0 );
	//gl_FragColor = vec4( texture2D(u_spec_tex, v_uv).xyz, 1.0 );
}
