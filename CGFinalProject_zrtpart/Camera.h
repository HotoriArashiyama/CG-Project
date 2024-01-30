#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma comment(lib, "glew32.lib")
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Layout.h"

#define PI 3.1415926535

#define max(a, b) ((a) > (b)? (a): (b))
#define min(a, b) ((a) > (b)? (b): (a))

class Camera {
private:
	float Position[3];	//Position of the Viewpoint
	float R, H;			//Position of the Viewpoint relative to the Center
	float Yaw, Roll;
	float prevX, prevY;
	float relPosSkyBox[3];
	float relPosTerrain[3];
	bool FixedTr;

public:
	Camera(float x, float y, float z);
	void SetAsMainCam();
	void ProcessInput(unsigned char key);
	void glutUpdateLook();
	static void SightMouseFollow(int x, int y);
	void SetSkyBoxFollow(SkyBox* skb, int x, int y, int z);
	void SetTerrainFollow(Terrain* trn, int x, int y, int z, bool fixedTr = false);
	float getX() const { return Position[0]; }
	float getY() const { return Position[1]; }
	float getZ() const { return Position[2]; }
	float getPos(char p) const { p = (p | 0x20) - 'x'; return Position[max(0, min(p, 2))]; }
	float getCntr(char p);
	glm::mat4 glmGetViewMatrix();
};

namespace Camera_h {
	static Camera* ptrMainCam = NULL;
	static SkyBox* ptrSkyBox = NULL;
	static Terrain* ptrTerrain = NULL;
}

Camera::Camera(float x, float y, float z) {
	Position[0] = x;
	Position[1] = y;
	Position[2] = z;

	Yaw = 90;
	Roll = 0;

	R = 1.5;
	H = 0;

	prevX = -1;
	prevY = -1;
}

void Camera::SetAsMainCam() {
	Camera_h::ptrMainCam = this;
}

void Camera::ProcessInput(unsigned char key) {
	switch (key)
	{
	case 'a': {
		Position[0] -= 0.05 * sin(PI / 180.0 * Yaw);
		Position[2] -= 0.05 * cos(PI / 180.0 * Yaw);
		break;
	}
	case 'd': {
		Position[0] += 0.05 * sin(PI / 180.0 * Yaw);
		Position[2] += 0.05 * cos(PI / 180.0 * Yaw);
		break;
	}
	case 'w': {
		Position[0] += 0.05 * cos(PI / 180.0 * Yaw);
		Position[2] -= 0.05 * sin(PI / 180.0 * Yaw);
		break;
	}
	case 's': {
		Position[0] -= 0.05 * cos(PI / 180.0 * Yaw);
		Position[2] += 0.05 * sin(PI / 180.0 * Yaw);
		break;
	}
	case ' ': {
		Position[1] += 0.05;
		break;
	}
	case 'z': {
		if(!Camera_h::ptrTerrain)
			Position[1] -= 0.05;
		else {
			Position[1] = max(Position[1] - 0.05, Camera_h::ptrTerrain->getY() - relPosTerrain[1]);
		}
		break;
	}
	case 'p': {
		//printf("%f\n", Yaw);
		//printf("from %f, %f, %f to %f, %f, %f\n", Position[0], Position[1], Position[2], R * cos(PI / 180.0 * Yaw) + Position[0], H + Position[1], -R * sin(PI / 180.0 * Yaw) + Position[2]);
		glm::vec3 front;
		float radPitch = atan(H / R);
		front.x = cos(glm::radians(Yaw));
		front.y = sin(radPitch);
		front.z = sin(glm::radians(Yaw));
		front = glm::normalize(front);
		printf("%f\n", Yaw);
		printf("at %f, %f, %f head %f, %f, %f\n", Position[0], Position[1], Position[2], front[0], front[1], front[2]);
		break;
	}
	default: break;
	}
	if(Camera_h::ptrSkyBox)
		Camera_h::ptrSkyBox->SetPos(Position[0] + relPosSkyBox[0], Position[1] + relPosSkyBox[1], Position[2] + relPosSkyBox[2]);
	if (Camera_h::ptrTerrain && FixedTr)
		Camera_h::ptrTerrain->SetX(Position[0] + relPosTerrain[0]), Camera_h::ptrTerrain->SetZ(Position[2] + relPosTerrain[2]);
}

void Camera::glutUpdateLook() {
	gluLookAt(Position[0], Position[1], Position[2], R * cos(PI / 180.0 * Yaw) + Position[0], H + Position[1], -R * sin(PI / 180.0 * Yaw) + Position[2], 0.0, 1.0, 0.0);	//从视点看远点
}

glm::mat4 Camera::glmGetViewMatrix() {
	glm::vec3 pos = glm::vec3(Position[0], Position[1], Position[2]);
	glm::vec3 cntr = glm::normalize(glm::vec3(R * cos(PI / 180.0 * Yaw), H, -R * sin(PI / 180.0 * Yaw)));
	glm::vec3 front;
	float radPitch = atan(H / R);
	front.x = cos(glm::radians(Yaw)) * cos(radPitch);
	front.y = sin(radPitch);
	front.z = -sin(glm::radians(Yaw)) * cos(radPitch);
	front = glm::normalize(front);
	glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0, 1.0, 0.0)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	glm::vec3 up = glm::normalize(glm::cross(right, front));
	return glm::lookAt(pos, pos + front, up);
}

float Camera::getCntr(char p) {
	p = (p | 0x20) - 'x';
	p = max(0, min(p, 2));
	if (p == 0) {
		return R * cos(PI / 180.0 * Yaw) + Position[0];
	}
	else if (p == 1) {
		return H + Position[1];
	}
	else if (p == 2) {
		return -R * sin(PI / 180.0 * Yaw) + Position[2];
	}
}

void Camera::SightMouseFollow(int x, int y) {
	if (Camera_h::ptrMainCam) {
		Camera_h::ptrMainCam->Yaw -= 0.4 * (x - Camera_h::ptrMainCam->prevX);
		Camera_h::ptrMainCam->H -= 0.03 * (y - Camera_h::ptrMainCam->prevY);
		if (Camera_h::ptrMainCam->H > 3.0) Camera_h::ptrMainCam->H = 3.0;
		else if (Camera_h::ptrMainCam->H < -1.0) Camera_h::ptrMainCam->H = -1.0;
		Camera_h::ptrMainCam->prevX = x, Camera_h::ptrMainCam->prevY = y;
	}
}

void Camera::SetSkyBoxFollow(SkyBox* skb, int x, int y, int z) {
	Camera_h::ptrSkyBox = skb;
	relPosSkyBox[0] = x, relPosSkyBox[1] = y, relPosSkyBox[2] = z;
}
void Camera::SetTerrainFollow(Terrain* trn, int x, int y, int z, bool fixedTr) {
	Camera_h::ptrTerrain = trn;
	relPosTerrain[0] = x, relPosTerrain[1] = y, relPosTerrain[2] = z;
	FixedTr = fixedTr;
}

#undef max
#undef min
#undef PI
#endif