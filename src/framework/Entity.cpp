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

	Texture = NULL; 
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

	Texture = NULL;
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

	Texture = NULL;
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

	Texture = NULL;
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

void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* Zbuffer)
{
	/*
	//int nVertices = mesh->vertices.size();
	size_t nVertices = mesh->GetVertices().size();
	std::vector<Vector3> vertices = mesh->GetVertices();
	camera->UpdateViewMatrix(); // AKA camera matrix
	camera->UpdateProjectionMatrix();
	camera->UpdateViewProjectionMatrix();
	int width = framebuffer->width;
	int height = framebuffer->height;

	bool negZ1 = false;
	bool negZ2 = false;
	bool negZ3 = false;

	Vector4 v1;
	Vector4 v2;
	Vector4 v3;

	Vector3 p1;
	Vector3 p2;
	Vector3 p3;

	Vector2 zero = Vector2(0, 0); 
	
	std::vector<Vector2> uv = mesh->GetUVs(); 
	Color c; 

	for (int i = 0; i < nVertices; i += 3) {

		v1 = Vector4(vertices[i].x, vertices[i].y, vertices[i].z, 1);
		v1 = model * v1;
		p1 = camera->ProjectVector(v1.GetVector3(), negZ1);//// &negZ
		if (negZ1) continue; 

		v2 = Vector4(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z, 1);
		v2 = model * v2;
		p2 = camera->ProjectVector(v2.GetVector3(), negZ2);//// &negZ
		if (negZ2) continue;

		v3 = Vector4(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z, 1);
		v3 = model * v3;
		p3 = camera->ProjectVector(v3.GetVector3(), negZ3);//// &negZ
		if (negZ3) continue;

		//clip to screen

		p1.x = (++p1.x / 2) * (width - 1);
		p1.y = (++p1.y / 2) * (height - 1);

		p2.x = (++p2.x / 2) * (width - 1);
		p2.y = (++p2.y / 2) * (height - 1);

		p3.x = (++p3.x / 2) * (width - 1);
		p3.y = (++p3.y / 2) * (height - 1);

		//draw line bresenham 

		if (framebuffer->textures) {

			//framebuffer->DrawTriangleInterpolated(p1, p2, p3, Color(255, 0, 0), Color(0, 255, 0), Color(0, 0, 255), Zbuffer, Texture, uv[i], uv[i + 1], uv[i + 2]);
			
			
			Image::TriInfo triangle; 
			triangle.p0 = p1; 
			triangle.p1 = p2;
			triangle.p2 = p3;

			triangle.c0 = c.RED;
			triangle.c1 = c.GREEN;
			triangle.c2 = c.BLUE;

			triangle.texture = Texture;
			triangle.uv0 = uv[i]; 
			triangle.uv1 = uv[i + 1];
			triangle.uv2 = uv[i + 2];

			//framebuffer->DrawTriangleInterpolated(p1, p2, p3, c.RED, c.GREEN, c.BLUE, Zbuffer, Texture, uv[i], uv[i + 1], uv[i + 2]);
			framebuffer->DrawTriangleInterpolated(triangle, Zbuffer); 

			//framebuffer->DrawTriangleInterpolated(p1, p2, p3, Color(255, 0, 0), Color(0, 255, 0), Color(0, 0, 255), Zbuffer);

		}
		else if (framebuffer->interpolatedColor) {

			framebuffer->DrawTriangleInterpolated(p1, p2, p3, Color(255, 0, 0), Color(0, 255, 0), Color(0, 0, 255), Zbuffer, NULL, zero, zero, zero);

		}
		else {

			framebuffer->DrawTriangle(Vector2(p1.x, p1.y), Vector2(p2.x, p2.y), Vector2(p3.x, p3.y), Color(252, 118, 18)); 

		}




		//last = Vector2(point.x, point.y); 

	}


	//printf("Rendering finished. \n"); 
	*/

}