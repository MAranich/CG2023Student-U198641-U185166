#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "mesh.h"


class Entity
{

private:

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Mesh* mesh;
	Matrix44 model; 
	Image* Texture; 

public:

	Entity(Vector3 Position, Vector3 Rotation, Vector3 Scale, Mesh* _mesh); //Rotation in RADS 
	Entity(Vector3 Position, Vector3 Rotation, Mesh* _mesh); //Rotation in RADS and no scale
	Entity(Vector3 Position, Mesh* _mesh, Vector3 Scale); //Identity rotation
	Entity(Vector3 Position, Mesh* _mesh); //Identity rotation and no scale

	void SetPosition(Vector3 Position);
	void SetRotation(Vector3 Rotation);
	void SetScale(Vector3 Scale);

	void SetTransform(Vector3 Position, Vector3 Rotation, Vector3 Scale);

	Vector3 GetPosition() { return position; }
	Vector3 GetRotation() { return rotation; }
	Vector3 GetScale() { return scale; }

	Matrix44 GetModel() { return model; }

	void SetTexture(Image* texture) { Texture = texture; }
	Image* GetTexture() { return Texture; }

	void SetMesh(Mesh* _mesh) { mesh = _mesh; }
	Mesh* GetMesh() { return mesh; }


	void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* Zbuffer); 


};