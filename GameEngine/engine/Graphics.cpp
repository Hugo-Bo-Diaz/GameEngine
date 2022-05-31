#include "../Globals.h"

#include "Graphics.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/matrix_decompose.hpp>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#pragma comment(lib, "glfw3.lib")

#include "Engine.h"
#include "Resources.h"
#include "Scene.h"

#include "GameObject/GameObject.h"

#include "Camera.h"

#include "../MessageManager.h"

Graphics::Graphics()
{
}

bool Graphics::InitWin32(HWND parwindow)
{

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "Title", NULL, NULL);

	if (window == NULL)
	{
		return false;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return false;
	}

	Whandler = glfwGetWin32Window(window);
	SetWindowLong(Whandler, GWL_STYLE, WS_VISIBLE);
	//MoveWindow(hwndGLFW, 0, 0, 400, 400, TRUE);

	SetWindowPos(Whandler,HWND_BOTTOM,0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	SetParent(Whandler, parwindow);

	vertices_vec.insert(vertices_vec.end(), {
	-0.5f, -0.5f, 0.0f,		0.0f, 1.0f,
	0.5f, -0.5f, 0.0f,		1.0f, 1.0f,
	0.5f, 0.5f, 0.0f,		1.0f, 0.0f,
	-0.5f, 0.5f, 0.0f,		0.0f, 0.0f

		});

	int indices[] = 
	{0,1,2,0,2,3};


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glColor4f(1.0f, 0.0f, 1.0f,1.0f);


	glGenBuffers(1, &EBO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1, &TexCoordsBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_vec.size()*sizeof(float), &vertices_vec[0], GL_DYNAMIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, TexCoordsBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texcoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glViewport(0, 0, 640, 480);

	LoadBase();

	updateresourceviewermsg* m1 = new updateresourceviewermsg();
	m1->dest = EDITOR;
	m1->id = base_texture;
	MessageManager::Get().AddMessage(m1);

	updateresourceviewermsg* m2 = new updateresourceviewermsg();
	m2->dest = EDITOR;
	m2->id = base_shader;
	MessageManager::Get().AddMessage(m2);

	// = glm::perspective(glm::radians(60.0f), float(640 / 480), 0.1f, 100.0f);

	glm::mat4 lookat = glm::lookAt(glm::vec3(0, 0, -1.0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	lookat = glm::inverse(lookat);
	glm::mat4 orthop;
	OrthProjection = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f, -10.0f, 10.0f);

	editor_camera = new Camera();
	editor_camera->SetOrthoMat(0.0f, 640.0f, 480.0f, 0.0f, -10.0f, 10.0f);
	editor_camera->CalculateMat();

	current_camera = editor_camera;

	return true;
}

bool Graphics::Render()
{
	if (!glfwWindowShouldClose(window))
	{
		// Actualizar el búfer de color y la profundidad
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		double  timeValue = glfwGetTime();
		float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
		float redValue = static_cast<float>(cos(timeValue) / 2.0 + 0.5);
		//editor_camera->SetPosition({ 0.5,0,0 });
		//editor_camera->SetPosition({redValue* 100,greenValue*100,greenValue });
		




		//glBindVertexArray(VAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glMatrixMode(GL_MODELVIEW);
		int i = 0;
		for (BlitItem* bit : Blitqueue)
		{
			/*3  4  -> top_left		(0.0f, 1.0f)
			  8  9  -> top_right	(1.0f, 1.0f)
			 13 14 -> bottom_right	(1.0f, 0.0f)
			 18 19 -> bottom_left	(0.0f, 0.0f)*/
			//vertices_vec[3] = bit->rect_in_tex[0];
			//vertices_vec[4] = bit->rect_in_tex[1] + bit->rect_in_tex[3];
			//
			//vertices_vec[8] = bit->rect_in_tex[0] + bit->rect_in_tex[2];
			//vertices_vec[9] = bit->rect_in_tex[1] + bit->rect_in_tex[3];
			//
			//vertices_vec[13] = bit->rect_in_tex[0] + bit->rect_in_tex[2];
			//vertices_vec[14] = bit->rect_in_tex[1];
			//
			//vertices_vec[18] = bit->rect_in_tex[0];
			//vertices_vec[19] = bit->rect_in_tex[1];

			vertices_vec[3] = bit->rect_in_tex[0];
			vertices_vec[4] = bit->rect_in_tex[1];

			vertices_vec[8] = bit->rect_in_tex[0] + bit->rect_in_tex[2];
			vertices_vec[9] = bit->rect_in_tex[1];

			vertices_vec[13] = bit->rect_in_tex[0] + bit->rect_in_tex[2];
			vertices_vec[14] = bit->rect_in_tex[1] + bit->rect_in_tex[3];

			vertices_vec[18] = bit->rect_in_tex[0];
			vertices_vec[19] = bit->rect_in_tex[1] + bit->rect_in_tex[3];

			glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_vec.size() * sizeof(float), &vertices_vec[0]);

			GLuint shader = 0;
			if (bit->shader >= 0)
			{
				shader = Engine::Get().rs->GetShader(bit->shader);
			}
			else
			{
				shader = Engine::Get().rs->GetShader(base_shader);
			}
			//MessageBox(NULL, std::to_string(shader).c_str(), "shader", MB_OK);
			glUseProgram(shader);

			GLuint texture;
			int image_is_active = false;
			if (bit->texture >= 0)
			{
				texture = Engine::Get().rs->GetTexture(bit->texture);
				image_is_active = true;

				int imagelocation = glGetUniformLocation(shader, "image");
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture);
				glUniform1i(imagelocation, 0);

			}
//			else
//				texture = Engine::Get().rs->GetTexture(base_texture);

			int hasimagelocation = glGetUniformLocation(shader, "hasimage");
			glUniform1i(hasimagelocation,image_is_active);

			int TransformLocation = glGetUniformLocation(shader, "ObjectTransform");
			glUniformMatrix4fv(TransformLocation,1,GL_FALSE,glm::value_ptr(bit->transform));

			int ProjectionLocation = glGetUniformLocation(shader, "ObjectProjection");
			glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(current_camera->GetProjMat()));

			int vertexColorLocation = glGetUniformLocation(shader, "ourColor");
			glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

			++i;
		}
		ClearBlitItemList(&Blitqueue);
		//glDrawElementsInstanced(GL_TRIANGLES,6,GL_UNSIGNED_INT,0,Blitqueue.size());
		//

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	return true;
}


bool Graphics::ShutDown()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return true;
}

void Graphics::LoadBase()
{

	base_shader = Engine::Get().rs->LoadShader(base_shader_name.c_str());
	base_texture = Engine::Get().rs->LoadTexture(base_texture_name.c_str());
}

void Graphics::MoveEngineWindow(int x, int y, int w, int h)
{
	editor_camera->SetOrthoMat(0.0f, w, h, 0.0f, -10.0f, 10.0f);
	editor_camera->CalculateMat();

	glViewport(0, 0, w, h);
	MoveWindow(Whandler, x, y, w, h, TRUE);
	//glViewport(x, y, w, h);

}

void Graphics::SetVsync(bool set)
{
	if (set)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

bool Graphics::DrawSprite(glm::mat4 transform, glm::vec4 rec_on_tex, int TexResId, int ShaResId)
{
	BlitItem* bItm = new BlitItem(transform,rec_on_tex,TexResId,ShaResId);
	Blitqueue.push_back(bItm);

	return true;
}

void Graphics::ClearBlitItemList(std::list<BlitItem*>* list)
{
	for (BlitItem* bit : *list)
	{
		delete bit;
	}
	list->clear();
}
