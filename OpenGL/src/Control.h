#pragma once

#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Layout.h"
#include "ScreenShot.h"

#define BUFSIZE 512
#define max(a, b) ((a) > (b)? (a): (b))
#define min(a, b) ((a) > (b)? (b): (a))

#define KEY_ANTI_JITTER_TIME 0.5
#define MOUSE_ANTI_JITTER_TIME 0.25
#define PI 3.1415926535
#define PICK_ANGLE_LIMIT 20

namespace Control_h {
	//SystemStatus in Control_h and CameraStatus in Camera_h is related and one-to-one. Pay attention when adjusting.
	enum SystemStatus { WANDERING = 0, PLACING, INTERACTING, BACKPACK_GUI, EDITING };
	static Camera* ptrMouseCamera;
	static OperableLayout* ptrCtrlObj = NULL;
	static Camera* ptrKeyCamera = NULL;
	static GLfloat GlobalZoomRate = 75.0;// GlobalZoomRate is now fovy
	static int GLOBAL_CONTROL_STATUS = WANDERING;
	GLuint selectBuf[BUFSIZE]; // a selection buffer
	bool GUIMousePressedSetTrue = false; // pass the mouse status to GUI.h
	std::vector<std::pair<std::string, OperableLayout*>>::iterator Obj2Add; // obj selected in GUI
	int Obj2AddId = -1;
	float prevImguiMouseWheel;
	double prevKeyTime = 0, curKeyTime = 0;
	bool pressedMouseLeft = false, pressedMouseRight = false;
	double viewportX = 0, viewportY = 0;
	bool PIPELINE_INTERRUPTION = false;
	bool HideMouse = false;
	PlayerAddedLight* AcquireAddLight = NULL;
	bool UsePickMatrixFirst = false;
}

class MouseCtrl {
private:
	Camera* Cam;

	static void ProcessHit(GLint hits, GLuint buffer[]);
	static void ProcessHit4GLFW();

public:
	MouseCtrl(Camera* cam);
	void SetAsMouseCtrlCamera() { Control_h::ptrMouseCamera = Cam; }
	static void MouseClickProc(int button, int state, int x, int y);
	static void MouseClickProc(int button, int state, double x, double y);
	static void MouseDragProc(int x, int y);
	static void MouseClickProc4GLFWLoop(GLFWwindow* window);
	static void MouseScrollProc(int button, int dir, int x, int y);
	static void MouseScrollProc(GLFWwindow* window, double xoffset, double yoffset);
	static void MouseScrollProc4GLFWLoop(GLFWwindow* window);
};

MouseCtrl::MouseCtrl(Camera *cam) {
	Cam = cam;
}

extern void renderScene(void);
extern void reshape(GLFWwindow *, int, int);
extern GLFWwindow* ptrWindow;
extern int SCRN_W, SCRN_H;

void MouseCtrl::MouseClickProc4GLFWLoop(GLFWwindow* window) {
	if ((!Control_h::pressedMouseLeft) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		Control_h::pressedMouseLeft = true;
		MouseClickProc(GLFW_PRESS, GLFW_MOUSE_BUTTON_LEFT, Control_h::viewportX, Control_h::viewportY);
	}
	else if ((!Control_h::pressedMouseRight) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		Control_h::pressedMouseRight = true;
		if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::BACKPACK_GUI) {
			MouseClickProc(GLFW_PRESS, GLFW_MOUSE_BUTTON_RIGHT, Control_h::viewportX, Control_h::viewportY);
		}
		else {
			if (Control_h::HideMouse)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			Control_h::HideMouse = !Control_h::HideMouse;
		}
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		Control_h::pressedMouseLeft = false;
		MouseClickProc(GLFW_RELEASE, GLFW_MOUSE_BUTTON_LEFT, Control_h::viewportX, Control_h::viewportY);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		Control_h::pressedMouseRight = false;
	}
}

void MouseCtrl::MouseClickProc(int button, int state, int x, int y) {
	double dbx = x, dby = y;
	MouseClickProc(button, state, dbx, dby);
}

void MouseCtrl::MouseClickProc(int button, int state, double x, double y) {
	// function interface with game objects
	if (state == GLFW_MOUSE_BUTTON_LEFT) {
		//printf("Mouse click triggered.\n");
		if (button == GLFW_PRESS) {
			Control_h::GUIMousePressedSetTrue = true;
			if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::PLACING) {
				Control_h::GLOBAL_CONTROL_STATUS = Control_h::WANDERING;
				Camera_h::MAIN_CAMERA_STATUS = Control_h::GLOBAL_CONTROL_STATUS;
				printf("Obj put back.\n");
				return;
			}
			else if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING) {
				if (Control_h::UsePickMatrixFirst) {
					// Reference: Lab6
					// Picking
					GLint viewport[4]; //视口大小，四个值按顺序为x, y, width, height
					glSelectBuffer(BUFSIZE, Control_h::selectBuf);
					glRenderMode(GL_SELECT); //拾取前，需要进入选择模式
					glInitNames();

					glMatrixMode(GL_PROJECTION); //选择投影矩阵。判断是以投影的方式进行的。
					glPushMatrix(); //向状态机压入投影矩阵
					glLoadIdentity(); //重置矩阵，为了构建拾取矩阵

					glGetIntegerv(GL_VIEWPORT, viewport);//返回当前视口的数值，并存入viewport数组中
					gluPickMatrix(x, viewport[3] - y, 1, 1, viewport); //建立拾取矩阵，前两个参数为将窗口坐标cursor转化为OpenGL坐标，第三、四个参数是选择框的大小，最后一个就是视口的位置和大小
					gluPerspective(Control_h::GlobalZoomRate, (float)SCRN_W / SCRN_H, 1.0, 1000.0); //乘以一个固定的投影矩阵。当然也可以变化这个投影矩阵
					glMatrixMode(GL_MODELVIEW);//进入视图变换状态

					Control_h::PIPELINE_INTERRUPTION = true;
					renderScene();
					Control_h::PIPELINE_INTERRUPTION = false;

					int hits;

					// restoring the original projection matrix
					glMatrixMode(GL_PROJECTION);
					glPopMatrix();
					glMatrixMode(GL_MODELVIEW);
					glFlush();

					// returning to normal rendering model
					hits = glRenderMode(GL_RENDER);

					// if there are hits process then
					if (hits != 0) {
						if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING) {
							Control_h::GLOBAL_CONTROL_STATUS = Control_h::PLACING;
							Camera_h::MAIN_CAMERA_STATUS = Control_h::GLOBAL_CONTROL_STATUS;
							ProcessHit(hits, Control_h::selectBuf);
						}
						else if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::PLACING) {
							Control_h::GLOBAL_CONTROL_STATUS = Control_h::WANDERING;
							Camera_h::MAIN_CAMERA_STATUS = Control_h::GLOBAL_CONTROL_STATUS;
						}
					}
					else {
						ProcessHit4GLFW();
					}
					return;
				}
				else {
					ProcessHit4GLFW();
				}
			}
		}
		else if (button == GLFW_RELEASE) {
			Control_h::GUIMousePressedSetTrue = false;
		}
	}
	else if (state == GLFW_MOUSE_BUTTON_RIGHT) {
		if (button == GLFW_PRESS) {
			if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::BACKPACK_GUI) {
				/*TODO: place objs by adding them into a linked list*/
				if (Control_h::Obj2AddId >= 0) {
					if (Control_h::Obj2Add->second->IsLightSource() == false) {
						if (Control_h::Obj2Add->second->IsGeneralPrismCreator() == false) {
							PlayerAddedLayout* p = new PlayerAddedLayout();
							p->PlayerAddInit(Control_h::ptrMouseCamera->getCntr('X'), Control_h::ptrMouseCamera->getCntr('Y'), Control_h::ptrMouseCamera->getCntr('Z'), Control_h::Obj2Add->second);
							p->PushIntoList(Control_h::Obj2AddId);
						}
						else {
							PlayerAddedPrism* p = new PlayerAddedPrism();
							p->PlayerAddInit(Control_h::ptrMouseCamera->getCntr('X'), Control_h::ptrMouseCamera->getCntr('Y'), Control_h::ptrMouseCamera->getCntr('Z'), Control_h::Obj2Add->second);
							p->PushIntoList(Control_h::Obj2AddId);
						}
					}
					else {
						PlayerAddedLight* p = new PlayerAddedLight();
						p->PlayerAddInit(Control_h::ptrMouseCamera->getCntr('X'), Control_h::ptrMouseCamera->getCntr('Y'), Control_h::ptrMouseCamera->getCntr('Z'), Control_h::Obj2Add->second);
						p->PushIntoList(Control_h::Obj2AddId);
						Control_h::AcquireAddLight = p;
					}
				}
			}
		}
		else if (button == GLFW_RELEASE) {
		}
	}
}

// Reference: Lab6
void MouseCtrl::ProcessHit(GLint hits, GLuint buffer[]) {
	unsigned int i, j;
	GLuint names, * ptr, minZ, * ptrNames, numberOfNames = 0;
	ptrNames = NULL;
	//printf("hits = %d\n", hits);
	ptr = (GLuint*)buffer;
	minZ = 0xffffffff;

	for (i = 0; i < hits; i++) {
		names = *ptr;
		ptr++;

		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr + 2;
		}

		ptr += names + 2;
	}

	//printf("The closest hit names are ");
	ptr = ptrNames;
	for (j = 0; j < numberOfNames; j++, ptr++) {
		//printf("%d ", *ptr);
	}
	//printf("\n");

	if (ptrNames && ptrNames[0] == Layout_h::NameBase) {
		//printf("Hit names: %d", ptrNames[0]);
		//printf(", %d\n", ptrNames[0], ptrNames[1]);
		Control_h::ptrCtrlObj = (OperableLayout*)Layout_h::NamedLayout[ptrNames[1]];
		//printf("Now controlling: OBJ%04d\n", ptrNames[1]);
		//printf("Control_h::ptrCtrlObj = %016lx\n", Control_h::ptrCtrlObj);
	}
	else {
		Control_h::GLOBAL_CONTROL_STATUS = Control_h::WANDERING;
		Camera_h::MAIN_CAMERA_STATUS = Control_h::GLOBAL_CONTROL_STATUS;
		ProcessHit4GLFW();
	}
}

void MouseCtrl::ProcessHit4GLFW() {
	if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::PLACING) {
		Control_h::GLOBAL_CONTROL_STATUS = Control_h::WANDERING;
		Camera_h::MAIN_CAMERA_STATUS = Control_h::GLOBAL_CONTROL_STATUS;
		return;
	}
	if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING) {
		Camera* cam = Control_h::ptrMouseCamera;
		PlayerAddedLayout* p = NULL;
		double judge = std::cos(PICK_ANGLE_LIMIT * PI / 180.0);
		glm::vec3 posCam(cam->getPos('X'), cam->getPos('Y'), cam->getPos('Z'));
		glm::vec3 cntrCam(cam->getCntr('X'), cam->getCntr('Y'), cam->getCntr('Z'));
		glm::vec3 frontCam = cntrCam - posCam;
		bool flagHit = false;
		float minDist = cam->getR() * 2.0;
		PlayerAddedLayout* nrHit = NULL;
		for (std::map<int, void*>::iterator it = Layout_h::AddedLayout.begin(); it != Layout_h::AddedLayout.end(); it++) {
			p = (PlayerAddedLayout*)it->second;
			float* ptrTar = p->getTrans();
			glm::vec3 vecTar(ptrTar[0], ptrTar[1], ptrTar[2]);
			glm::vec3 distTar = glm::vec3(vecTar - posCam);
			glm::vec3 frontTar = distTar;
			//printf("tar: %f %f %f cntr: %f %f %f pos: %f %f %f\n", vecTar[0], vecTar[1], vecTar[2], cntrCam[0], cntrCam[1], cntrCam[2], posCam[0], posCam[1], posCam[2]);

			float cosTheta = (frontCam[0] * frontTar[0] + frontCam[1] * frontTar[1] + frontCam[2] * frontTar[2]) / glm::length(frontCam) / glm::length(frontTar);
			if (cosTheta >= judge) {
				//printf("%f %f", cosTheta, judge);
				if (glm::length(distTar) <= minDist) {
					if (p->NotHidden()) {
						minDist = glm::length(distTar);
						nrHit = p;
						flagHit = true;
					}
				}
			}
		}
		if (flagHit) {
			Control_h::ptrCtrlObj = nrHit;
			Control_h::GLOBAL_CONTROL_STATUS = Control_h::PLACING;
			Camera_h::MAIN_CAMERA_STATUS = Control_h::GLOBAL_CONTROL_STATUS;
		}
	}
}

void MouseCtrl::MouseDragProc(int x, int y) {

}

void MouseCtrl::MouseScrollProc4GLFWLoop(GLFWwindow* window) {
	ImGuiIO& curImguiIO = ImGui::GetIO();
	float dm = curImguiIO.MouseWheel - Control_h::prevImguiMouseWheel;
	if (std::abs(dm) > 0.001f) {
		MouseScrollProc(window, 0, dm);
	}
}

void MouseCtrl::MouseScrollProc(int button, int dir, int x, int y) {
	if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING) {
		//Control_h::GlobalZoomRate *= dir > 0 ? 1.1 : (1 / 1.1);//GlobalZoomRate now is fovy
		Control_h::GlobalZoomRate += dir > 0 ? (-1) : (1);
		reshape(ptrWindow, SCRN_W, SCRN_H);
		//glutPostRedisplay();
	}
}

void MouseCtrl::MouseScrollProc(GLFWwindow* window, double xoffset, double yoffset) {
	if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING) {
		Control_h::GlobalZoomRate += (float)(-yoffset);
		Control_h::GlobalZoomRate = max(30, min(120, Control_h::GlobalZoomRate));
		reshape(ptrWindow, SCRN_W, SCRN_H);
	}
}


class KeyboardCtrl {
private:
	Camera* Cam;

public:
	KeyboardCtrl(Camera* cam);
	void SetAsKeyCtrlCamera() { Control_h::ptrKeyCamera = Cam; }
	static void KeyInProc(unsigned char k, int x, int y);
	static void KeyInProc(int k, int x, int y);
	static void KeyProcExt4GLFW(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void KeyProc4GLFWLoop(GLFWwindow* window);
};

KeyboardCtrl::KeyboardCtrl(Camera* cam) {
	Cam = cam;
}

void KeyboardCtrl::KeyInProc(unsigned char key, int x, int y) {
	int intkey = key;
	KeyInProc(intkey, x, y);
}

void KeyboardCtrl::KeyProc4GLFWLoop(GLFWwindow* window) {
	int key = GLFW_KEY_UNKNOWN;
	if (glfwGetKey(ptrWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(ptrWindow, true);
		return;
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { key = 'W'; }
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { key = 'S'; }
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { key = 'A'; }
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { key = 'D'; }
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) { key = 'Z'; }
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) { key = 'X'; }
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) { key = 'C'; }
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { key = 'Q'; }
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { key = 'E'; }
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) { key = 'P'; }
	if (key != GLFW_KEY_UNKNOWN && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS) { key = key | 0x20; }
	if (key == GLFW_KEY_UNKNOWN) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { key = ' '; }
	}
	if (key != GLFW_KEY_UNKNOWN) {
		Control_h::curKeyTime = glfwGetTime();
		KeyInProc(key, 0, 0);
		if (key == 'e' && Control_h::HideMouse && (Control_h::GLOBAL_CONTROL_STATUS == Control_h::BACKPACK_GUI || Control_h::GLOBAL_CONTROL_STATUS == Control_h::EDITING)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			Control_h::HideMouse = false;
		}
	}
}

void KeyboardCtrl::KeyProcExt4GLFW(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}
}

void KeyboardCtrl::KeyInProc(int key, int x, int y) {
	if (Control_h::ptrKeyCamera && (Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING || Control_h::GLOBAL_CONTROL_STATUS == Control_h::BACKPACK_GUI))
		Control_h::ptrKeyCamera->ProcessInput(key);
	else if (Control_h::ptrKeyCamera && Control_h::GLOBAL_CONTROL_STATUS == Control_h::PLACING || Control_h::GLOBAL_CONTROL_STATUS == Control_h::EDITING) {
		//printf("Placing mode keyprocessing.\n");
		float CamX = Control_h::ptrKeyCamera->getPos('X');
		float CamY = Control_h::ptrKeyCamera->getPos('Y');
		float CamZ = Control_h::ptrKeyCamera->getPos('Z');
		float CamCtX = Control_h::ptrKeyCamera->getCntr('X');
		float CamCtY = Control_h::ptrKeyCamera->getCntr('Y');
		float CamCtZ = Control_h::ptrKeyCamera->getCntr('Z');
		float dirX = CamCtX - CamX;
		float dirY = CamCtY - CamY;
		float dirZ = CamCtZ - CamZ;
		//ckpt();
		switch (key)
		{
		case ((int)'a'): {
			Control_h::ptrCtrlObj->AddTrans(dirZ / 30, 0, -dirX / 30);
			break;
		}
		case ((int)'d'): {
			Control_h::ptrCtrlObj->AddTrans(-dirZ / 30, 0, dirX / 30);
			break;
		}
		case ((int)'w'): {
			Control_h::ptrCtrlObj->AddTrans(dirX / 30, 0, dirZ / 30);
			break;
		}
		case ((int)'s'): {
			Control_h::ptrCtrlObj->AddTrans(-dirX / 30, 0, -dirZ / 30);
			break;
		}
		case ((int)' '): {
			Control_h::ptrCtrlObj->AddTrans(0, 0.05, 0);
			break;
		}
		case ((int)'z'): {
			Control_h::ptrCtrlObj->AddTrans(0, -0.05, 0);
			break;
		}
		case ((int)'x'): {
			Control_h::ptrCtrlObj->MultScale(1.05);
			break;
		}
		case ((int)'c'): {
			Control_h::ptrCtrlObj->MultScale(1 / 1.05);
			break;
		}
		case ((int)'Z'): {
			Control_h::ptrCtrlObj->AddRot(0.5, 0, 0);
			break;
		}
		case ((int)'X'): {
			Control_h::ptrCtrlObj->AddRot(0, 0.5, 0);
			break;
		}
		case ((int)'C'): {
			Control_h::ptrCtrlObj->AddRot(0, 0, 0.5);
			break;
		}
		default:
			break;
		}
	}

	switch (key)
	{
	//case 27: {exit(0); break; }
	case ((int)'P'): {
		if (Control_h::curKeyTime - Control_h::prevKeyTime <= KEY_ANTI_JITTER_TIME * 2) { Control_h::prevKeyTime = Control_h::curKeyTime; break; }
		else { Control_h::prevKeyTime = Control_h::curKeyTime; }
		ScreenShotCapturer::getScreenShot();
		break;
	}
	case ((int)'e'): {	/*Backpack/GUI Mode*/
		/*interfaces for GUI*/
		if (Control_h::curKeyTime - Control_h::prevKeyTime <= KEY_ANTI_JITTER_TIME) { Control_h::prevKeyTime = Control_h::curKeyTime; break; }
		else { Control_h::prevKeyTime = Control_h::curKeyTime; }
		if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING) {
			Control_h::GLOBAL_CONTROL_STATUS = Control_h::BACKPACK_GUI;
		}
		else if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::BACKPACK_GUI) {
			Control_h::GLOBAL_CONTROL_STATUS = Control_h::WANDERING;
			Control_h::Obj2AddId = -1;
		}
		else if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::PLACING) {
			Control_h::GLOBAL_CONTROL_STATUS = Control_h::EDITING;
		}
		else if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::EDITING) {
			Control_h::GLOBAL_CONTROL_STATUS = Control_h::PLACING;
		}
		Camera_h::MAIN_CAMERA_STATUS = Control_h::GLOBAL_CONTROL_STATUS;
		break;
	}
	case ((int)'r'): {	/*Settings*/
		/*interfaces for GUI*/
		break;
	}
	default: break;
	}
}

#undef BUFSIZE
#undef max
#undef min
#undef PI
#undef PICK_ANGLE_LIMIT
#endif