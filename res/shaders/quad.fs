varying vec2 v_uv;
uniform float u_exercise; 
uniform sampler2D u_fruits; 
uniform float u_time; 
uniform vec3 u_resolution; 

float pi = 3.1415922654; 

float rand(vec2 n); 

void main()
{
	//auxiliar variables
	float ft = fract(u_time * 0.25); //[0, 1]
	float fft = fract(u_time * 0.25) * 2 - 1; //[-1, 1]
	vec2 res = vec2(u_resolution); // resolution of the screen
	vec2 InvRes = vec2(1/res.x, 1/res.y); 
	float AspectRatio = res.x/res.y; 
	float InvAspectRatio = res.y/res.x; 
	
	//TODO: squares exe 3, 5, gaussian filter

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
		float n = 8.0; //number of stripes, can be customized
		float invSqrt2 = 0.7071067812; // 1/sqrt(2), other values can be selected
		float c = 3.414213562; // 2 + sqrt(2) // 1/(1 - invSqrt2)
		n = n * pi; //precalculating (n * pi)

		vec2 uv = v_uv; 
		float displacement = (1-AspectRatio) * 0.5; 
		uv.x = uv.x * AspectRatio + displacement; 


		float r = c*max(pow(sin(uv.x * n), 2) - invSqrt2, 0); 
		float b = c*max(pow(sin(uv.y * n), 2) - invSqrt2, 0);
	
		//n = n * 2; 
		//float r = c*max(sin(v_uv.x * n) - invSqrt2, 0); 
		//float b = c*max(sin(v_uv.y * n) - invSqrt2, 0); 


		gl_FragColor = vec4(r, 0, b, 1); 
	} else if(u_exercise == 3){

		float n = 10; 
		float invN = 0.1; // 1/n

		vec2 uv = v_uv; 
		float displacement = (1-AspectRatio) * 0.5; 
		uv.x = uv.x * AspectRatio + displacement; 

		uv = floor(uv * n) * invN; 

		uv.x = (uv.x - displacement) * InvAspectRatio; 
		//uv = fract(uv); 

		gl_FragColor = vec4(uv, 0, 1);
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

		vec2 uv = v_uv; 
		float displacement = (1-AspectRatio) * 0.5; 
		uv.x = uv.x * AspectRatio + displacement; 

		float c = fract(max(uv.x, uv.y) * n) * 2 - 1; 
		float d = fract(min(uv.x, uv.y) * n) * 2 - 1; 

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

		pix = vec3(1-pix.z, pix.x, pix.y * 0.5); 

		gl_FragColor = vec4(pix, 1);
	} else if(u_exercise == 9){
		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		
		float x = pix.x + pix.y + pix.z; 
		x = x * 0.33333333333333; 

		x = step(0.435, x); //[0.4, 0.425]

		pix = vec3(x, x, x); 

		gl_FragColor = vec4(pix, 1);
	} else if(u_exercise == 10){
		
		//gaussian blurr

		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		pix = pix * 41;  //center value

		//inner ring //8 vals
		vec2 Xvals = vec2(clamp(v_uv.x + InvRes.x, 0, 1), clamp(v_uv.x - InvRes.x, 0, 1)); //precalculate values
		vec2 Yvals = vec2(clamp(v_uv.y + InvRes.y, 0, 1), clamp(v_uv.y - InvRes.y, 0, 1)); 

		vec3 pixAux = texture2D(u_fruits, vec2(Xvals.x, v_uv.y)).xyz; 
		pix += pixAux * 26; 

		pixAux = texture2D(u_fruits, vec2(v_uv.x, Yvals.x)).xyz; 
		pix += pixAux * 26; 

		pixAux = texture2D(u_fruits, vec2(Xvals.y, v_uv.y)).xyz; 
		pix += pixAux * 26; 

		pixAux = texture2D(u_fruits, vec2(v_uv.x, Yvals.y)).xyz; 
		pix += pixAux * 26; 
		//
		pixAux = texture2D(u_fruits, vec2(Xvals.x, Yvals.x)).xyz; 
		pix += pixAux * 16; 
		
		pixAux = texture2D(u_fruits, vec2(Xvals.x, Yvals.y)).xyz; 
		pix += pixAux * 16; 
		
		pixAux = texture2D(u_fruits, vec2(Xvals.y, Yvals.x)).xyz; 
		pix += pixAux * 16; 
		
		pixAux = texture2D(u_fruits, vec2(Xvals.y, Yvals.y)).xyz; 
		pix += pixAux * 16; 

		//outer ring // 16 vals
		vec2 _Xvals = vec2(clamp(v_uv.x + 2 * InvRes.x, 0, 1), clamp(v_uv.x - 2 * InvRes.x, 0, 1)); //precalculate values
		vec2 _Yvals = vec2(clamp(v_uv.y + 2 * InvRes.y, 0, 1), clamp(v_uv.y - 2 * InvRes.y, 0, 1)); 

		pixAux = texture2D(u_fruits, vec2(_Xvals.x, v_uv.y)).xyz; 
		pix += pixAux * 7; 
		
		pixAux = texture2D(u_fruits, vec2(_Xvals.y, v_uv.y)).xyz; 
		pix += pixAux * 7; 
		
		pixAux = texture2D(u_fruits, vec2(v_uv.x, _Yvals.x)).xyz; 
		pix += pixAux * 7; 
		
		pixAux = texture2D(u_fruits, vec2(v_uv.x, _Yvals.y)).xyz; 
		pix += pixAux * 7; 
		//

		pixAux = texture2D(u_fruits, vec2(Xvals.x, _Yvals.x)).xyz; 
		pix += pixAux * 4; 
		
		pixAux = texture2D(u_fruits, vec2(_Xvals.x, Yvals.x)).xyz; 
		pix += pixAux * 4; 

		pixAux = texture2D(u_fruits, vec2(Xvals.y, _Yvals.x)).xyz; 
		pix += pixAux * 4; 

		pixAux = texture2D(u_fruits, vec2(_Xvals.y, Yvals.x)).xyz; 
		pix += pixAux * 4; 

		pixAux = texture2D(u_fruits, vec2(Xvals.x, _Yvals.y)).xyz; 
		pix += pixAux * 4; 

		pixAux = texture2D(u_fruits, vec2(_Xvals.x, Yvals.y)).xyz; 
		pix += pixAux * 4; 

		pixAux = texture2D(u_fruits, vec2(Xvals.y, _Yvals.y)).xyz; 
		pix += pixAux * 4; 

		pixAux = texture2D(u_fruits, vec2(_Xvals.y, Yvals.y)).xyz; 
		pix += pixAux * 4; 
		////

		pixAux = texture2D(u_fruits, vec2(_Xvals.x, _Yvals.x)).xyz; 
		pix += pixAux; 
		
		pixAux = texture2D(u_fruits, vec2(_Xvals.y, _Yvals.x)).xyz; 
		pix += pixAux; 

		pixAux = texture2D(u_fruits, vec2(_Xvals.x, _Yvals.y)).xyz; 
		pix += pixAux; 

		pixAux = texture2D(u_fruits, vec2(_Xvals.y, _Yvals.y)).xyz; 
		pix += pixAux; 



		pix = pix * 0.003663003663; // 1/273

		//I just realized that texture2D() already clamps the values.  :(
		


		gl_FragColor = vec4(pix, 1);
	} else if(u_exercise == 11){
		vec3 pix = texture2D(u_fruits, v_uv).xyz; 
		
		vec2 v = vec2(v_uv.x - 0.5, v_uv.y - 0.5);
		float d = dot(v, v);
		d = sqrt(d);
		d = clamp(d, 0, 1);

		gl_FragColor = vec4(pix * (0.8 - d), 1);
	} else if(u_exercise == 12){

		float k = 0.35; 
		float c = cos(u_time * k); 
		float s = sin(u_time * k); 
		mat2 rot; 
		rot[0][0] = c; 
		rot[1][0] = s; 
		rot[0][1] = -s; 
		rot[1][1] = c; 

		vec2 center = vec2(0.5);

		vec2 uv = v_uv; 
		float displacement = (1-AspectRatio) * 0.5; 
		uv.x = uv.x * AspectRatio + displacement; 

		uv = rot * (uv - center) + center; 

		uv.x = (uv.x - displacement) * InvAspectRatio; 
		uv = fract(uv); 


		vec3 pix = texture2D(u_fruits, uv).xyz; 

		gl_FragColor = vec4(pix, 1);
	} else if(u_exercise == 13){
		float n = 20; 
		float invN = 0.05; 

		vec2 uv = v_uv; 
		float displacement = (1-AspectRatio) * 0.5; 
		uv.x = uv.x * AspectRatio + displacement; 
		
		uv = floor(uv * n) * invN; 

		uv.x = (uv.x - displacement) * InvAspectRatio; 

		vec3 pix = texture2D(u_fruits, uv).xyz; 


		gl_FragColor = vec4(pix, 1);
	} else if(u_exercise == 14){
		vec2 uv = v_uv; 
		float n = 20; 
		float invN = 0.05; 

		float displacement = (1-AspectRatio) * 0.5; 
		uv.x = uv.x * AspectRatio + displacement; 
		
		uv = floor(uv * n) * invN; 

		uv.x = (uv.x - displacement) * InvAspectRatio; 

		vec3 pix = vec3(rand(uv + u_time), rand(uv.yx + u_time + 1), rand(uv + u_time + 2)); 
		//vec3 pix = vec3(uv, 0); 
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

float rand(vec2 n) { 
	//return fract(sin(mod(dot(n, vec2(12.9898, 4.1414)), pi)) * 43758.5453);
	return fract(sin(dot(n, vec2(12.9898, 78.233))) * 43758.5453); 
}
