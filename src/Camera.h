#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;
class Camera
{
public:
	vec3 _camPosition;
	vec3 _camlookAt;
	vec3 _camUpDir;
	mat4 _projectionMatrix;

	Camera();
	void init(vec3 newPos, vec3 target, vec3 up);
	mat4 GetVP();
	void SetProjection(float fov, float aspect, float nearZ, float farZ);
	void SetPosition(vec3 newPos);
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();
	~Camera();

private:

};

Camera::Camera()
{
	_camPosition = vec3(0.0, 0.0, 0.0);
	_camlookAt = vec3(0.0, 0.0, -1.0);
	_camUpDir = vec3(0.0, 1.0, 0.0);
}
void Camera::init(vec3 newPos, vec3 target, vec3 up)
{
	_camPosition = newPos;
	_camlookAt = target;
	_camUpDir = up;
}
mat4 Camera::GetVP()
{
	mat4 view = lookAt(_camPosition,_camlookAt,_camUpDir);

	mat4 vp = _projectionMatrix * view;
	return vp;
}
void Camera::SetProjection(float fov, float aspect, float nearZ, float farZ)
{
	_projectionMatrix = perspective(fov, aspect, nearZ, farZ);
}
void Camera::SetPosition(vec3 newPos)
{
	_camPosition = newPos;
}
void Camera::MoveForward()
{
	
	if (_camPosition.z >= 0) return;
	vec3 lookDir =  _camlookAt - _camPosition ;
	normalize(lookDir);
	lookDir *= 0.02;
	_camPosition +=  lookDir;
}
void Camera::MoveBackward()
{
	if(_camPosition.z <= -30) return;
	vec3 lookDir =  _camlookAt - _camPosition ;
	normalize(lookDir);
	lookDir *= 0.02;
	_camPosition -= lookDir;
}
void Camera::MoveLeft()
{
	vec3 lookDir =  _camlookAt - _camPosition ; 
	vec3 leftvec = cross(_camUpDir, lookDir);
	normalize(leftvec);
	_camPosition -= leftvec;
}
void Camera::MoveRight()
{
	vec3 lookDir =  _camlookAt - _camPosition ; 
	vec3 leftvec = cross(lookDir,_camUpDir);
	normalize(leftvec);
	_camPosition -= leftvec;
}
Camera::~Camera()
{
}
#endif _CAMERA_H_