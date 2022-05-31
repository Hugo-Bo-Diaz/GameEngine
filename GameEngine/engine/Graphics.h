#ifndef GRAPHICS__H
#define GRAPHICS__H

#ifdef _WIN32
#include<Windows.h>
#endif

#include <list>
#include <vector>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera;

struct BlitItem
{
	BlitItem(glm::mat4 _transform,glm::vec4 _rect_in_tex,int tex, int sh) {
		transform = _transform;
		rect_in_tex = _rect_in_tex;

		texture = tex;
		shader = sh;
	};
	//POSITION / TRANSFORM
	glm::mat4 transform;
	glm::vec4 rect_in_tex;

	//TEXTUREID (INSIDE RESOURCE MOD)
	int texture;

	//SHADERID (INSIDE RESOURCE MOD)
	int shader;
};

class Graphics 
{
public:

	Graphics();
	bool InitWin32(HWND window);
	bool Render();
	bool ShutDown();

	void MoveEngineWindow(int x, int y, int w, int h);
	void SetVsync(bool set);

	GLFWwindow* window;
	HWND Whandler;


	int base_shader;
	std::string base_shader_name = "default.glsl";
	int base_texture;
	std::string base_texture_name = "test.png";
	void LoadBase();

	//CAMERA COMPONENT LATER
	glm::mat4 OrthProjection;
	glm::mat4 viewProjection;

	glm::vec3 position = glm::vec3(0,0,0);
	float rotation;
	
	//SQUARE
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	unsigned int TexCoordsBO;

	std::vector<float> vertices_vec;
	int vertices_size = 4 * 5;//4 vert 5 attributes per vert

	Camera* current_camera;
	Camera* editor_camera;

	std::list<BlitItem*> Blitqueue;
	bool DrawSprite(glm::mat4 transform, glm::vec4 uv_on_tex, int TexResId = -1, int ShaResId = -1);
	void ClearBlitItemList(std::list<BlitItem*>*);
};

#endif // !ENGINE__H
