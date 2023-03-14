// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_world_normal; 
varying vec2 v_uv;
varying vec3 v_world_position; 

uniform sampler2D u_tex;
uniform sampler2D u_normals;
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

//float div255 = 0.0039215686; // = 1/255

void main()
{
	// Set the ouput color per pixel
	
	float ft = fract(u_time * 0.333); 
	float fft = fract(u_time * 0.333) * 2 - 1; 

	//vec3 color = normalize(v_world_normal);
	vec4 color = vec4(0);

	vec4 normal = texture2D(u_normals, v_uv); 
	normal = normal * 2 - 1; 
	normal.a = 0; 
	normal = u_model * normal; 
	vec3 N = normalize(normal.xyz); 

	float lerpVal = 0.5718281828; 

	N = mix(v_world_normal, N, lerpVal); 


	
	color = texture2D(u_tex, v_uv); 
	
	//La
	vec3 intensity = u_ka * color.xyz * u_ambientintensity; 

	//Ld
	vec3 L = normalize(u_lightpos - v_world_position);
	intensity = intensity + color.xyz * max(dot(L, N), 0) * u_intensitydiff; 

	//Ls
	vec3 V = normalize(u_camerapos - v_world_position);
	vec3 R = reflect(-L, N); 
	R = normalize(R); 
	vec3 ks = vec3(color.a); 
	intensity = intensity + ks * pow(max(dot(R, V), 0), u_shiny) * u_intensityspec;


	//gl_FragColor = vec4(intensity, 1.0 );
	gl_FragColor = vec4( intensity, 1.0 );
	//gl_FragColor = vec4( texture2D(u_spec_tex, v_uv).xyz, 1.0 );
}
