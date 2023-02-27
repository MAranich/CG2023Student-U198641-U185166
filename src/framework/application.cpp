#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

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
	speed = 1;
	Orbiting = false;
	OrbitingPoint = Vector3(0, 0, 0);


	//framebuffer.interpolatedColor = true;
	//framebuffer.occlusion = true;
	//framebuffer.textures = true;


	//camera.SetPerspective(80.0f, (float)framebuffer.width / (float)framebuffer.height, 0.02f, 100.0f);
	camera.SetPerspective(80.0f, 1.6f, 0.02f, 100.0f);

	printf("Camera is set to perspective mode by deafult. \n");


	shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs"); 
	mesh = new Mesh(); 
	mesh->CreateQuad(); 


}

// Render one frame
void Application::Render(void)
{

	shader->Enable();

	mesh->Render();

	shader->Disable();

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
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
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
	/*
	switch (event.keysym.sym) {
	case SDLK_c:
		framebuffer.interpolatedColor = !framebuffer.interpolatedColor;
		if (framebuffer.interpolatedColor) printf("Using interpolated color... \n");
		else printf("Using plain color. \n");
		break;
	case SDLK_z:
		framebuffer.occlusion = !framebuffer.occlusion;
		if (framebuffer.occlusion) printf("Occlusion enabled. \n");
		else printf("Occlusion disabled. \n");
		break;
	case SDLK_t:
		framebuffer.textures = !framebuffer.textures;
		if (framebuffer.textures) printf("Rendering textures... \n");
		else printf("Textures disabled. \n");
		break;
	default:
		break;
	}
	*/
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
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

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
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