
#include "shader.h"
#include "texture.h"





class Material {

public: 

	void Material(Vector3 Ka, Vector3 Ks, Vector3 Kd, float shiny) {


		AmbirentReflectionCoef = Ka;
		SpecularReflectionCoef = Ks;
		DiffuseReflectionCoef = Kd;

		Shininess = shiny; 

	}

	void Material(Vector3 Ka, Vector3 Ks, Vector3 Kd, float shiny, Shader s, Texture t) {


		AmbirentReflectionCoef = Ka;
		SpecularReflectionCoef = Ks;
		DiffuseReflectionCoef = Kd; 

		Shininess = shiny;

		shader = s;
		texture = t;

	}

	void SetShader(Shader s) { shader = s; };

	void SetTexture(Texture t) { texture = t; };

	void Enable() { s.Enable(); };
	void Disable() { s.Disable(); };


private: 
	
	Shader shader; 
	Texture texture; 

	Vector3 AmbirentReflectionCoef;
	Vector3 SpecularReflectionCoef;
	Vector3 DiffuseReflectionCoef; 

	float Shininess; 



};



















































