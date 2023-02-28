varying vec2 v_uv;
uniform float u_exercise; 
uniform sampler2D u_fruits; 
uniform float u_time; 

float pi = 3.1415922654; 
void main()
{

	if(u_exercise == 0){

		gl_FragColor = vec4(v_uv.x, 0, 1 - v_uv.x, 1);

	}else if(u_exercise == 1) {
		//works best if the winfow width = window height
		vec2 v = vec2(v_uv.x - 0.5, v_uv.y - 0.5);
		float d = dot(v, v);
		d = sqrt(d);
		d = clamp(d, 0, 1);
    
		gl_FragColor = vec4(d, d, d, 1);

	}else if(u_exercise == 2){
		float n = 5.0; //number of stripes, can be customized
		float invSqrt2 = 0.7071067812; // 1/sqrt(2), other values can be selected
		float c = 3.414213562; // 2 + sqrt(2) // 1/(1 - invSqrt2)
		n = n * pi; //precalculating (n * pi)

		float r = c*max(pow(sin(v_uv.x * n), 2) - invSqrt2, 0); 
		float b = c*max(pow(sin(v_uv.y * n), 2) - invSqrt2, 0);
	
		//n = n * 2; 
		//float r = c*max(sin(v_uv.x * n) - invSqrt2, 0); 
		//float b = c*max(sin(v_uv.y * n) - invSqrt2, 0); 


		gl_FragColor = vec4(r, 0, b, 1); 
	} else if(u_exercise == 3){
		//works best if the winfow width = window height
		float n = 10; 
		float invN = 0.1; 
		gl_FragColor = vec4(floor(v_uv * n) * invN, 0, 1);
	} else if(u_exercise == 4){
		//easy implementation: 
		float siny = sin(v_uv.x * 2 * pi) * 0.2 + 0.5; // 0.2 is an arbitrary chosen constant it could be = to [0, 0.5]
		
		//easy, normal method: 
		//if(0 < siny - v_uv.y) gl_FragColor = vec4(0, v_uv.y, 0, 1);
		//else gl_FragColor = vec4(0, 1-v_uv.y, 0, 1); 


		float s = step(0, siny - v_uv.y); 
		gl_FragColor = vec4(0, mix(1-v_uv.y, v_uv.y, s), 0, 1); //mix(a, b, x) = b*x + (1-x)*a 
		
	} else if(u_exercise == 5){

		float n = 20; 

		float c = fract(max(v_uv.x, v_uv.y) * n) * 2 - 1; 
		float d = fract(min(v_uv.x, v_uv.y) * n) * 2 - 1; 

		c = step(0, c * d); 

		gl_FragColor = vec4(c, c, c, 1); 
	} else if(u_exercise == 6){
		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		float x = pix.x + pix.y + pix.z; 
		x = x * 0.33333333333333; 
		pix = vec3(x, x, x); 

		gl_FragColor = vec4(pix, 1);

	} else if(u_exercise == 7){
		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		pix =  1 - pix; 

		gl_FragColor = vec4(pix, 1);

	} else if(u_exercise == 8){
		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		float ftime = fract(u_time); 

		// xyz : yzx : zxy : yxz : xzy : zyx
		//pix = 1-pix.zxy; 
		//pix = vec3(pix.x * 0.13333333, pix.y, pix.z * 0.39215686); 
		//pix = mix(pix.zxy, pix.yzx, pix.x); 
		pix = vec3(pix.x * ftime, pix.y, pix.z); 
		pix = pix.zyx;

		gl_FragColor = vec4(pix, 1);

	} else if(u_exercise == 9){
		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		

		gl_FragColor = vec4(pix, 1);

	} else if(u_exercise == 10){
		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		

		gl_FragColor = vec4(pix, 1);

	} else if(u_exercise == 11){
		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		

		gl_FragColor = vec4(pix, 1);

	}else{
		gl_FragColor = vec4(v_uv, 0, 1);

	}

	/*
	EXTRA SHADERS: 
		1) 

	float n = 20; 
	float d = v_uv.x + v_uv.y; 
	d = d * 0.5; 
	float c = step(0.5, fract(d * n)); //fract() returns fractional part ( fract() = x - floor(x) )


	gl_FragColor = vec4(c, c, c, 1); 

		2) 

	float n = 20; 
	//float d = v_uv.x + v_uv.y; 
	//d = d * 0.5; 
	float c = step(fract(v_uv.y * n), fract(v_uv.x * n)); 


	gl_FragColor = vec4(c, c, c, 1); 

		3) 
	float n = 20; 
	//float d = v_uv.x + v_uv.y; 
	//d = d * 0.5; 
	//float c = step(fract(v_uv.y * n), fract(v_uv.x * n)); 
	float c = step(0.5, fract(max(v_uv.x, v_uv.y) * n)); 
		


	gl_FragColor = vec4(c, c, c, 1); 

		4) 

		5) 



	*/

}
