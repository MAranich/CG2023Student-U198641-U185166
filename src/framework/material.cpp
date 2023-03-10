#include "material.h"




Material::Material(Vector3 Ka, Vector3 Ks, Vector3 Kd, float shiny) {


	AmbirentReflectionCoef = Ka;
	SpecularReflectionCoef = Ks;
	DiffuseReflectionCoef = Kd;

	Shininess = shiny;

	shader = NULL;
	texture = NULL;

}


Material::Material(Vector3 Ka, Vector3 Ks, Vector3 Kd, float shiny, Shader* s, Texture* t) {


	AmbirentReflectionCoef = Ka;
	SpecularReflectionCoef = Ks;
	DiffuseReflectionCoef = Kd;

	Shininess = shiny;

	shader = s;
	texture = t;


}

void Material::Enable(sUniformData data) {

	shader->SetFloat("u_time", data.time);
	//shader->SetVector3("u_resolution", Vector3(window_width, window_height, 0));
	shader->SetMatrix44("u_viewprojection", data.ViewProjMatrix);
	shader->SetMatrix44("u_model", data.model);
	shader->SetTexture("u_tex", texture);

}















































