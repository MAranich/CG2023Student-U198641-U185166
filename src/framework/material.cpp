#include "material.h"




Material::Material(Vector3 Ka, Vector3 Ks, Vector3 Kd, float shiny) {


	AmbientReflectionCoef = Ka;
	SpecularReflectionCoef = Ks;
	DiffuseReflectionCoef = Kd;

	Shininess = shiny;

	shader = NULL;
	texture = NULL;

}


Material::Material(Vector3 Ka, Vector3 Ks, Vector3 Kd, float shiny, Shader* s, Texture* t) {


	AmbientReflectionCoef = Ka;
	SpecularReflectionCoef = Ks;
	DiffuseReflectionCoef = Kd;

	Shininess = shiny;

	shader = s;
	texture = t;


}

void Material::Enable(sUniformData data) {

	//shader->SetVector3("u_resolution", Vector3(window_width, window_height, 0));
	shader->SetFloat("u_time", data.time);
	shader->SetVector3("u_camerapos", data.CameraPosition);
	shader->SetMatrix44("u_viewprojection", data.ViewProjMatrix);
	shader->SetMatrix44("u_model", data.model);
	shader->SetTexture("u_tex", texture);
	shader->SetVector3("u_ambientintensity", data.AmbientIntensity);
	shader->SetVector3("u_ks", SpecularReflectionCoef);
	shader->SetVector3("u_kd", DiffuseReflectionCoef);
	shader->SetVector3("u_ka", AmbientReflectionCoef);
	shader->SetFloat("u_shiny", Shininess);
	shader->SetVector3("u_lightpos", data.Position);
	shader->SetVector3("u_intensitydiff", data.IntensityDiffuse);
	shader->SetVector3("u_intensityspec", data.IntensitySpecular);
	shader->SetFloat("u_mode", (float)data.mode);



}















































