varying vec2 v_uv;
uniform float u_exercise; 
uniform sampler2D u_fruits; 
uniform float u_time; 
uniform vec3 u_resolution; 

float pi = 3.1415922654; 
void main()
{
	//auxiliar variables
	float ft = fract(u_time * 0.25); //[0, 1]
	float fft = fract(u_time * 0.25) * 2 - 1; //[-1, 1]
	vec2 res = vec2(u_resolution); // resolution of the screen
	vec2 InvRes = vec2(1/res.x, 1/res.y); 


	if(u_exercise == 0){

		gl_FragColor = vec4(v_uv.x, 0, 1 - v_uv.x, 1);
	} else if(u_exercise == 1){
		//works best if the winfow width = window height
		vec2 v = vec2(v_uv.x - 0.5, v_uv.y - 0.5);
		float d = dot(v, v);
		d = sqrt(d);
		d = clamp(d, 0, 1);
    
		gl_FragColor = vec4(d, d, d, 1);
	} else if(u_exercise == 2){
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


		// xyz : yzx : zxy : yxz : xzy : zyx
		//pix = 1-pix.zxy; 
		//pix = vec3(pix.x * 0.13333333, pix.y, pix.z * 0.39215686); 
		//pix = mix(pix.zxy, pix.yzx, pix.x); 
		//pix = vec3(0, pix.y, 1-pix.z); 
		//pix = vec3(1-pix.z, pix.y, 0); 
		//pix = vec3(1-pix.x, pix.z, ft*1-pix.z); 
		//pix = pix.zyx; 

		pix = vec3(1-pix.z, pix.x * 0.7, 0);
		// We think we did a pretty good aproximation

		gl_FragColor = vec4(pix, 1);
	} else if(u_exercise == 9){
		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		
		float x = pix.x + pix.y + pix.z; 
		x = x * 0.33333333333333; 

		x = step(0.435, x); //[0.4, 0.425]

		pix = vec3(x, x, x); 

		gl_FragColor = vec4(pix, 1);
	} else if(u_exercise == 10){
		/*
		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		pix = pix * 0.2; 

		vec3 pixAux = texture2D(u_fruits, vec2(clamp(v_uv.x + InvRes.x, 0, 1), v_uv.y)).xyz; 
		pix = pix + pixAux * 0.2; 

		pixAux = texture2D(u_fruits, vec2(v_uv.x, clamp(v_uv.y + InvRes.y, 0, 1))).xyz; 
		pix = pix + pixAux * 0.2; 

		pixAux = texture2D(u_fruits, vec2(clamp(v_uv.x - InvRes.x, 0, 1), v_uv.y)).xyz; 
		pix = pix + pixAux * 0.2; 

		pixAux = texture2D(u_fruits, vec2(v_uv.x, clamp(v_uv.x - InvRes.y, 0, 1))).xyz; 
		pix = pix + pixAux * 0.2; 
		*/

		//float w = 0.111111111111; // 1/25
		float w = 0.04; // 1/25

		vec3 pix = vec3(0, 0, 0); 
		vec2 p = v_uv; 
		float r = 2.0; 
		p.x += InvRes.x * -r; 
		p.y += InvRes.y * -r; 
		float dx = InvRes.x; 
		float dy = InvRes.y; 
		float x_; 
		float y_; 
		for(x_ = -r; x_ <= r; x_++, p.x += dx){
			for(y_ = -r; y_ <= r; y_++, p.y += dy){
			
				p.x = clamp(p.x, 0, 1); 
				p.y = clamp(p.y, 0, 1); 
				pix += w * texture2D(u_fruits, p).xyz; 
			
			}
		}



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
