// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_world_normal; 

varying vec2 v_uv;
varying vec3 v_world_position; 

uniform sampler2D u_tex;

uniform float u_time; 


void main()
{
	// Set the ouput color per pixel
	vec3 color = normalize(v_world_normal);

	if(v_uv.x <= 1 && 0 <= v_uv.x && v_uv.y <= 1 && 0 <= v_uv.y){
		color = texture2D(u_tex, v_uv).xyz; 
	}


	gl_FragColor = vec4( color, 1.0 );
}
