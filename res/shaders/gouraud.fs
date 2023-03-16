// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_world_normal; 

varying vec2 v_uv;
varying vec3 v_world_position; 

varying vec3 v_intensity;


void main()
{

	gl_FragColor = vec4( v_intensity, 1.0 );

}
