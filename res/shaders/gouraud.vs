// Global variables from the CPU
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

// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec3 v_intensity;

//here create uniforms for all the data we need here
float div255 = 0.003921568; // = 1/255

void main()
{	
	v_uv = gl_MultiTexCoord0.xy;

	// Convert local position to world space
	vec3 world_position = (u_model * vec4( gl_Vertex.xyz, 1.0)).xyz;
	//world_position.z += sin(world_position.x); 

	// Convert local normal to world space
	vec3 world_normal = (u_model * vec4( gl_Normal.xyz, 0.0)).xyz;

	// Pass them to the fragment shader interpolated
	v_world_position = world_position;
	v_world_normal = world_normal; 

	// Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader
	
	v_intensity = u_ka * u_ambientintensity; //la

	vec3 L = normalize(u_lightpos - v_world_position); //ld
	v_intensity = v_intensity + u_kd * max(dot(L, v_world_normal), 0) * u_intensitydiff;

	vec3 V = normalize(u_camerapos - v_world_position); //ls
	vec3 R = reflect(-L, v_world_normal); 
	v_intensity = v_intensity + u_ks * u_intensityspec * pow(max(dot(V, R), 0), u_shiny);


}