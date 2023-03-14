#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"
#include "Entity.h"

Entity::Entity(Vector3 Position, Vector3 Rotation, Vector3 Scale, Mesh* _mesh)
{
	
	material = NULL; 
	mesh = _mesh;
	model = Matrix44();

	position = Position;
	rotation = Rotation;
	scale = Scale;

	Matrix44 translation = Matrix44();
	translation._41 = position.x;
	translation._42 = position.y;
	translation._43 = position.z;

	Matrix44 rot = Matrix44();

	Matrix44 Pitch = Matrix44();

	Pitch._22 = cos(rotation.x);
	Pitch._23 = -sin(rotation.x);
	Pitch._32 = sin(rotation.x);
	Pitch._33 = cos(rotation.x);


	Matrix44 Yaw = Matrix44();

	Yaw._11 = cos(rotation.y);
	Yaw._13 = sin(rotation.y);
	Yaw._31 = -sin(rotation.y);
	Yaw._33 = cos(rotation.y);



	Matrix44 Roll = Matrix44();

	Roll._11 = cos(rotation.z);
	Roll._12 = -sin(rotation.z);
	Roll._21 = sin(rotation.z);
	Roll._22 = cos(rotation.z);

	rot = (Pitch * Yaw) * Roll;

	Matrix44 _scale = Matrix44();

	_scale._11 = Scale.x;
	_scale._22 = Scale.y;
	_scale._33 = Scale.z;

	//model = translation * rot * _scale;
	model = (_scale * rot) * translation;
	//this should be in the reversed order (like the commented line)
	//but it works correcly likt this. Seems like the multiplication of A * B returns 
	//B * A instead of A * B


}

Entity::Entity(Vector3 Position, Vector3 Rotation, Mesh* _mesh)
{

	material = NULL;
	mesh = _mesh;
	model = Matrix44();

	position = Position;
	rotation = Rotation;
	scale = Vector3(1, 1, 1);

	Matrix44 translation = Matrix44();
	translation._41 = position.x;
	translation._42 = position.y;
	translation._43 = position.z;

	Matrix44 rot = Matrix44();

	Matrix44 Pitch = Matrix44();

	Pitch._22 = cos(rotation.x);
	Pitch._23 = -sin(rotation.x);
	Pitch._32 = sin(rotation.x);
	Pitch._33 = cos(rotation.x);


	Matrix44 Yaw = Matrix44();

	Yaw._11 = cos(rotation.y);
	Yaw._13 = sin(rotation.y);
	Yaw._31 = -sin(rotation.y);
	Yaw._33 = cos(rotation.y);



	Matrix44 Roll = Matrix44();

	Roll._11 = cos(rotation.z);
	Roll._12 = -sin(rotation.z);
	Roll._21 = sin(rotation.z);
	Roll._22 = cos(rotation.z);

	rot = (Pitch * Yaw) * Roll;

	//model = translation * rot;
	model = rot * translation;




}

Entity::Entity(Vector3 Position, Mesh* _mesh, Vector3 Scale)
{

	material = NULL;
	mesh = _mesh;
	model = Matrix44();

	position = Position;
	rotation = Vector3(0, 0, 0);
	scale = Scale;

	Matrix44 translation = Matrix44();
	translation._41 = position.x;
	translation._42 = position.y;
	translation._43 = position.z;

	//Rotation matix is identity

	Matrix44 _scale = Matrix44();

	_scale._11 = Scale.x;
	_scale._22 = Scale.y;
	_scale._33 = Scale.z;

	//model = translation * _scale;
	model = _scale * translation;



}

Entity::Entity(Vector3 Position, Mesh* _mesh)
{

	material = NULL;
	mesh = _mesh;
	model = Matrix44();

	position = Position;
	rotation = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);

	model._41 = position.x;
	model._42 = position.y;
	model._43 = position.z;

}


void Entity::SetPosition(Vector3 Position)
{

	position = Position;

	model._41 = position.x;
	model._42 = position.y;
	model._43 = position.z;

}

void Entity::SetRotation(Vector3 Rotation)
{
	/////////RECHECK
	rotation = Rotation;

	Matrix44 translation = Matrix44();
	translation._41 = position.x;
	translation._42 = position.y;
	translation._43 = position.z;

	Matrix44 rot = Matrix44();

	Matrix44 Pitch = Matrix44();

	Pitch._22 = cos(rotation.x);
	Pitch._23 = -sin(rotation.x);
	Pitch._32 = sin(rotation.x);
	Pitch._33 = cos(rotation.x);


	Matrix44 Yaw = Matrix44();

	Yaw._11 = cos(rotation.y);
	Yaw._13 = sin(rotation.y);
	Yaw._31 = -sin(rotation.y);
	Yaw._33 = cos(rotation.y);



	Matrix44 Roll = Matrix44();

	Roll._11 = cos(rotation.z);
	Roll._12 = -sin(rotation.z);
	Roll._21 = sin(rotation.z);
	Roll._22 = cos(rotation.z);

	rot = (Pitch * Yaw) * Roll;

	Matrix44 _scale = Matrix44();

	_scale._11 = scale.x;
	_scale._22 = scale.y;
	_scale._33 = scale.z;

	model = (translation * rot) * _scale;

}

void Entity::SetScale(Vector3 Scale)
{

	SetTransform(position, rotation, Scale);


	/*
	Matrix44 _scale = Matrix44();
	_scale.SetIdentity();

	_scale._11 = 1/ scale.x;
	_scale._22 = 1/ scale.y;
	_scale._33 = 1/ scale.z;

	model = model * _scale; //undo last matrix multiplication

	scale = Scale;

	_scale._11 = scale.x;
	_scale._22 = scale.y;
	_scale._33 = scale.z;

	model = model * _scale; //redo matrix
	*/

}

void Entity::SetTransform(Vector3 Position, Vector3 Rotation, Vector3 Scale) {


	position = Position;
	rotation = Rotation;
	scale = Scale;

	Matrix44 translation = Matrix44();
	translation._41 = position.x;
	translation._42 = position.y;
	translation._43 = position.z;

	Matrix44 rot = Matrix44();

	Matrix44 Pitch = Matrix44();

	Pitch._22 = cos(rotation.x);
	Pitch._23 = -sin(rotation.x);
	Pitch._32 = sin(rotation.x);
	Pitch._33 = cos(rotation.x);


	Matrix44 Yaw = Matrix44();

	Yaw._11 = cos(rotation.y);
	Yaw._13 = sin(rotation.y);
	Yaw._31 = -sin(rotation.y);
	Yaw._33 = cos(rotation.y);



	Matrix44 Roll = Matrix44();

	Roll._11 = cos(rotation.z);
	Roll._12 = -sin(rotation.z);
	Roll._21 = sin(rotation.z);
	Roll._22 = cos(rotation.z);

	rot = Pitch * Yaw * Roll;

	Matrix44 _scale = Matrix44();

	_scale._11 = Scale.x;
	_scale._22 = Scale.y;
	_scale._33 = Scale.z;

	//model = translation * rot * _scale;
	model = _scale * rot * translation;
	//model = _scale * translation * rot;


}

void Entity::Render(sUniformData data)
{
	
	
	data.model = model;

	material->Enable(data); 
	mesh->Render();
	material->Disable();


}