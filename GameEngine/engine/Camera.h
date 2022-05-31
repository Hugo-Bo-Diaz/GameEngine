#ifndef CAMERA__H
#define CAMERA__H

#include <glm/glm.hpp>

#include <glm/gtc/type_ptr.hpp>
class Camera {
private:
	float left;
	float right;
	float top;
	float bottom;
	float znear;
	float zfar;

	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	float rotation = 0.0f;
	glm::vec2 zoom = { 1.0f, 1.0f };

	//camera transform
	glm::mat4 ViewMatr = glm::mat4(1.0);
	//orhto
	glm::mat4 OrthProjMatr = glm::mat4(1.0);

	//combined
	glm::mat4 ViewProjMatr = glm::mat4(1.0);;


public:

	Camera();
	void SetOrthoMat(float left, float right,float bottom, float top, float znear, float zfar);
	void SetPosition(glm::vec3 pos);
	void SetRotation(float rotation);
	void SetZoom(glm::vec2 zoom);


	void CalculateMat() {

		//viewmatrix
		ViewMatr = glm::mat4(1.0);
		ViewMatr = glm::translate(ViewMatr, position);
		ViewMatr = glm::rotate(ViewMatr,rotation, {0,0,1});
		ViewMatr = glm::scale(ViewMatr, glm::vec3(zoom, 1.0f));

		ViewMatr = glm::inverse(ViewMatr);

		//projmatrix
		OrthProjMatr = glm::ortho(left, right, bottom, top,znear,zfar);

		ViewProjMatr =  OrthProjMatr * ViewMatr;
	};

	glm::mat4 GetProjMat() {
		return ViewProjMatr;
	};
};



#endif