#pragma once

#include "main/includes.h"
#include "shader.h"
#include "texture.h"
#include "image.h"
#include "utils.h"
#include "framework.h"


struct Light {
	Vector3 Position; 
	Vector3 IntensityDiffuse; 
	Vector3 IntensitySpecular; 
};

struct sUniformData {
	Matrix44 model;
	Matrix44 ViewProjMatrix;
	Vector3 CameraPosition;
	Vector3 AmbientIntensity;
	float time;

	Texture* NormalText;


	//light
	int NumberOfLights; 
	Vector3* Position;
	Vector3* IntensityDiffuse;
	Vector3* IntensitySpecular;

	bool UseAmbient; 
	bool UseTexture;
	bool UseSpec; 

	float NormCoef; 
	bool isPhong; 

};

class Material {

public: 

	Material(Vector3 Ka, Vector3 Ks, Vector3 Kd, float shiny); 

	Material(Vector3 Ka, Vector3 Ks, Vector3 Kd, float shiny, Shader* s, Texture* t);

	void SetShader(Shader* s) { shader = s; };

	void SetTexture(Texture* t) { texture = t; };

	void Enable(sUniformData data, int index); 
	void Disable() { shader->Disable(); };


private: 
	
	Shader* shader; 
	Texture* texture; 

	Vector3 AmbientReflectionCoef;
	Vector3 SpecularReflectionCoef;
	Vector3 DiffuseReflectionCoef; 

	float Shininess; 



};