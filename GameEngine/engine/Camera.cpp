#include "Camera.h"

Camera::Camera()
{
}

void Camera::SetOrthoMat(float _left, float _right, float _bottom, float _top, float _znear, float _zfar)
{
	left = _left;
	right = _right;
	top = _top;
	bottom = _bottom;
	znear = _znear;
	zfar = _zfar;

	OrthProjMatr = glm::ortho(left, right, bottom, top, znear, zfar);
}

void Camera::SetPosition(glm::vec3 pos)
{
	position = pos;
	CalculateMat();
}

void Camera::SetRotation(float _rotation)
{
	rotation = _rotation;
	CalculateMat();
}

void Camera::SetZoom(glm::vec2 _zoom)
{
	zoom = _zoom;
	CalculateMat();
}
