#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "material.h"

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	//this->framebuffer.Resize(w, h);
}

Application::~Application()
{
	//SDL_DestroyWindow(window);
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;


	camera = Camera();
	eye = Vector3(0, 0, -1);
	center = Vector3(0, 0, 0);
	camera.LookAt(eye, center, Vector3(0, 1, 0));

	// Y axis is up
	CamDir = -1 * eye;
	Delta = Vector2(0, 0);
	Sensibility = 0.3f * (float)PI / 180.0f;
	LastPos = Vector2(0, 0);
	LastPosEye = Vector2(0, 0);
	speed = 30;
	Orbiting = false;
	OrbitingPoint = Vector3(0, 0, 0);
	cumulativeTime = 0;

	//framebuffer.interpolatedColor = true;
	//framebuffer.occlusion = true;
	//framebuffer.textures = true;


	//camera.SetPerspective(80.0f, (float)framebuffer.width / (float)framebuffer.height, 0.02f, 100.0f);
	camera.SetPerspective(80.0f, 1.6f, 0.02f, 100.0f);

	printf("Camera is set to perspective mode by deafult. \n");

	light.Position = Vector3(-2, 2, 0); 
	light.IntensityDiffuse = Vector3(255, 255, 255); 
	light.IntensitySpecular = Vector3(255, 255, 255); 
	


	Mesh* leeMesh = new Mesh();
	leeMesh->LoadOBJ("meshes/lee.obj");


	object = new Entity(Vector3(0, 0, 0), Vector3(0, PI, 0), leeMesh);

	//object->SetMaterial(new Material())

	//shading time! 

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	rendering = true;
	exercise = 0;

	//rendshader = Shader::Get("shaders/gouraud.vs", "shaders/gouraud.fs");
	rendshader = Shader::Get("shaders/phong.vs", "shaders/phong.fs");
	//shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");

	mesh = new Mesh();
	mesh->CreateQuad();

	material = new Material(Vector3(255, 255, 255), Vector3(255, 255, 255), Vector3(200, 200, 200), 50);
	
	tex = new Texture();
	//tex->Load("textures/lee_normal.tga");
	tex->Load("textures/lee_color_specular.tga");
	SpecTex = new Texture(); 
	SpecTex->Load("textures/lee_color_specular.tga");


	material->SetTexture(tex);
	material->SetShader(rendshader);
	object->SetMaterial(material);
	mode = 1;
	AmbientIntensity = Vector3(20, 20, 30);



}

// Render one frame
void Application::Render(void)
{
	
	//data._lights = &light; 


	rendshader->Enable();
	data.CameraPosition = camera.eye;
	data.ViewProjMatrix = camera.GetViewProjectionMatrix();
	data.time = cumulativeTime;
	data.AmbientIntensity= AmbientIntensity;
	data.Position = light.Position;
	data.IntensityDiffuse = light.IntensityDiffuse;
	data.IntensitySpecular = light.IntensitySpecular; 
	data.SpecularText = SpecTex; 

	//shader->SetVector3("u_resolution", Vector3(window_width, window_height, 0));

	object->Render(data);

	//rendshader->Disable();




}

// Called after render
void Application::Update(float seconds_elapsed)
{
	secElapsed = seconds_elapsed;
	cumulativeTime += seconds_elapsed;

	if (Orbiting) {
		camera.LookAt(eye, OrbitingPoint, Vector3(0, 1.0f, 0));
	}
	else {

		//Sphere parametritzation
		float SDy = sin((float)Delta.y); //little optimitzation
		CamDir = Vector3(SDy * cos((float)Delta.x), cos((float)Delta.y), SDy * sin((float)Delta.x));
		camera.LookAt(eye, eye + CamDir, Vector3(0, 1, 0));

	}


}

//keyboard press event 
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	if (rendering) {


		if (Orbiting) {
			Vector3 Dist;
			Vector3 DistRot;
			float angle = 0;
			float dist = 0;;
			float SpeedOverDist = 0;
			float c = 0; //cos
			float s = 0; //sin

			switch (event.keysym.sym) {
			case SDLK_p:
				camera.SetPerspective(100, 1.6f, 0.02f, 200.0);
				printf("Camera is set to perspective mode. \n");
				break;
			case SDLK_o:
				camera.SetOrthographic(-1, 1, 1, -1, -1, 1);
				printf("Camera is set to ortographic mode. \n");
				break;
			case SDLK_w: //forward
				Dist = OrbitingPoint - eye; //vector from eye to Orb.Point
				Dist.Normalize();
				eye = eye + Dist * speed * secElapsed;
				break;
			case SDLK_a:


				Dist = eye - OrbitingPoint;
				dist = sqrtf(Dist.Dot(Dist));
				SpeedOverDist = speed / dist;//rotate speed units of distance in a circle of dist radius
				angle = SpeedOverDist * 0.2f; //rad
				c = cos(angle);
				s = sin(angle);

				DistRot = Vector3(Dist.x * c - Dist.z * s, 0, Dist.x * s + Dist.z * c);
				//multyply Distance by rotation matrix

				eye = OrbitingPoint + DistRot;
				break;
			case SDLK_d:


				Dist = eye - OrbitingPoint;
				dist = sqrtf(Dist.Dot(Dist));
				SpeedOverDist = speed / dist; //rotate speed units of distance in a circle of dist radius
				angle = SpeedOverDist * 0.2f; //rad
				c = cos(angle);
				s = sin(-angle);

				DistRot = Vector3(Dist.x * c - Dist.z * s, 0, Dist.x * s + Dist.z * c);
				//multyply Distance by rotation matrix

				eye = OrbitingPoint + DistRot;
				break;
			case SDLK_s:
				Dist = OrbitingPoint - eye; //vector from eye to Orb.Point
				Dist.Normalize();
				eye = eye - Dist * speed * secElapsed;
				break;
			case SDLK_PLUS:
				if (camera.type == camera.PERSPECTIVE) {
					camera.SetPerspective(camera.fov - 5, 1.6f, 0.01f, 200.0f);
					printf("Camera fov: %0.3lf\n", camera.fov);
				}
				break;
			case SDLK_MINUS:
				if (camera.type == camera.PERSPECTIVE) {
					camera.SetPerspective(camera.fov + 5, 1.6f, 0.02f, 200.0f);
					printf("Camera fov: %0.3lf\n", camera.fov);
				}
				break;
			default:
				break;
			}


		}
		else {
			Vector3 KeyDir = Vector3(1, 0, 0);
			float s = sin(Delta.x);
			float c = cos(Delta.x);
			switch (event.keysym.sym) {
			case SDLK_p:
				camera.SetPerspective(90.0f, 1.6f, 0.02f, 200.0f);
				printf("Camera is set to perspective mode. \n");
				break;
			case SDLK_o:
				camera.SetOrthographic(-1, 1, 1, -1, -1, 1);
				printf("Camera is set to ortographic mode. \n");
				break;
			case SDLK_w: //forward (pos Z dir)
				KeyDir = Vector3(1, 0, 0);
				KeyDir = Vector3(KeyDir.x * c - KeyDir.z * s, 0, KeyDir.x * s + KeyDir.z * c);
				eye = eye + KeyDir * speed * secElapsed;
				break;
			case SDLK_a:
				KeyDir = Vector3(0, 0, -1);
				KeyDir = Vector3(KeyDir.x * c - KeyDir.z * s, 0, KeyDir.x * s + KeyDir.z * c);
				eye = eye + KeyDir * speed * secElapsed;
				break;
			case SDLK_s:
				KeyDir = Vector3(-1, 0, 0);
				KeyDir = Vector3(KeyDir.x * c - KeyDir.z * s, 0, KeyDir.x * s + KeyDir.z * c);
				eye = eye + KeyDir * speed * secElapsed;
				break;
			case SDLK_d:
				KeyDir = Vector3(0, 0, 1);
				KeyDir = Vector3(KeyDir.x * c - KeyDir.z * s, 0, KeyDir.x * s + KeyDir.z * c);
				eye = eye + KeyDir * speed * secElapsed;
				break;
			case SDLK_SPACE: //go up 
				eye = eye + Vector3(0, 1, 0) * speed * secElapsed;
				break;
			case SDLK_v://go down
				eye = eye + Vector3(0, -1, 0) * speed * secElapsed;
				break;
			case SDLK_PLUS:
				if (camera.type == camera.PERSPECTIVE) {
					camera.SetPerspective(camera.fov - 5, 1.6f, 0.01f, 200.0f);
					printf("Camera fov: %0.3lf\n", camera.fov);
				}
				break;
			case SDLK_MINUS:
				if (camera.type == camera.PERSPECTIVE) {
					camera.SetPerspective(camera.fov + 5, 1.6f, 0.02f, 200.0f);
					printf("Camera fov: %0.3lf\n", camera.fov);
				}
				break;
			default:
				break;
			}


		}



		if (event.keysym.sym == SDLK_RETURN) {
			//rendering = false;
			//camera.eye = Vector3(0, 0, -1);
			//camera.center = Vector3(0, 0, 0);
			//camera.LookAt(eye, center, Vector3(0, 1, 0));
		}

	}
	else
	{


		if (event.keysym.sym == SDLK_RETURN) {
			//rendering = true;
		}
		else if (event.keysym.sym == SDLK_RIGHT) exercise++;
		else if (event.keysym.sym == SDLK_LEFT) exercise--;

		//do a clamp()
		if (exercise < 0) exercise = 0;
		else if (15 < exercise) exercise = 15; 

	}

	//printf("\tCurrenly displaying exercise %d. \n\n", exercise);

}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	//LMB = 1	MMB = 2		RMB = 3
	if (event.button == 1) { //if LMB is clicked
		Orbiting = !Orbiting;
		if (Orbiting) {
			eye.y = 0;
			OrbitingPoint = Vector3(0, 0, 0); //Set Orbiting point, Could be changed
			printf("Now orbiting arround (%0.3lf, %0.3lf, %0.3lf) \nPress A and D to orbit! \n", OrbitingPoint.x, OrbitingPoint.y, OrbitingPoint.z);
		}
		else {
			printf("Now NOT orbiting. \n");
		}
	}
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{

}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{

	Vector2 NDelt = Vector2((float)event.x, (float)event.y) - LastPos;

	if (Orbiting) {
		if (event.button == 4) { //RMB = 4? //for some reason???
			Vector3 Dist;
			Vector3 DistRot;
			float dist = 0;

			Dist = eye - OrbitingPoint;
			dist = sqrtf(Dist.Dot(Dist));
			float angle = Sensibility * NDelt.x / dist; //rad
			float c = cos(angle);
			float s = sin(angle);

			DistRot = Vector3(Dist.x * c - Dist.z * s, 0, Dist.x * s + Dist.z * c);
			//multyply Distance by rotation matrix

			eye = OrbitingPoint + DistRot;
		}
	}
	else {

		Delta = Delta + NDelt * Sensibility;
		float y = Delta.y;

		if (y < 0) y = 0;
		else if (PI < y) y = (float)PI;

		Delta.y = y;
	}

	LastPos = Vector2((float)event.x, (float)event.y);
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = (float)event.preciseY * -0.25f;
	camera.near_plane += dy;
	camera.far_plane += dy;
	if (camera.type == camera.PERSPECTIVE) {
		camera.SetPerspective(camera.fov, camera.aspect, camera.near_plane, camera.far_plane);
	}
	else if (camera.type == camera.ORTHOGRAPHIC) {
		camera.SetOrthographic(camera.left, camera.right, camera.top, camera.bottom, camera.near_plane, camera.far_plane);
	}
}

void Application::OnFileChanged(const char* filename)
{
	Shader::ReloadSingleShader(filename);
}