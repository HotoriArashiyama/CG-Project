#pragma once

#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma comment(lib, "glew32.lib")
#include <gl/glew.h>
#include <gl/glut.h>

#include "Layout.h"
#include "ScreenShot.h"

#define BUFSIZE 512

namespace Control_h {
	enum SystemStatus { WANDERING = 0, PLACING, INTERACTING, BACKPACK };
	static Camera* ptrMouseCamera;
	static OperableLayout* ptrCtrlObj = NULL;
	static Camera* ptrKeyCamera = NULL;
	static GLfloat GlobalZoomRate = 1;
	static int GLOBAL_CONTROL_STATUS = WANDERING;
	GLuint selectBuf[BUFSIZE]; // ����һ��ѡ�񻺳���
}

class MouseCtrl {
private:
	Camera* Cam;

	static void ProcessHit(GLint hits, GLuint buffer[]);

public:
	MouseCtrl(Camera* cam);
	void SetAsMouseCtrlCamera() { Control_h::ptrMouseCamera = Cam; }
	static void MouseClickProc(int button, int state, int x, int y);
	static void MouseDragProc(int x, int y);
	static void MouseScrollProc(int button, int dir, int x, int y);
};

MouseCtrl::MouseCtrl(Camera *cam) {
	Cam = cam;
}

extern void renderScene(void);

void MouseCtrl::MouseClickProc(int button, int state, int x, int y) {
	// function interface with game objects
	if (state == GLUT_LEFT_BUTTON) {
		//printf("Mouse click triggered.\n");
		if (button == GLUT_DOWN) {
			if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::PLACING) {
				Control_h::GLOBAL_CONTROL_STATUS = Control_h::WANDERING;
				printf("Obj put back.\n");
				return;
			}
			// Reference: Lab6
			// Picking
			GLint viewport[4]; //�ӿڴ�С���ĸ�ֵ��˳��Ϊx, y, width, height
			glSelectBuffer(BUFSIZE, Control_h::selectBuf);
			glRenderMode(GL_SELECT); //ʰȡǰ����Ҫ����ѡ��ģʽ
			glInitNames();

			glMatrixMode(GL_PROJECTION); //ѡ��ͶӰ�����ж�����ͶӰ�ķ�ʽ���еġ�
			glPushMatrix(); //��״̬��ѹ��ͶӰ����
			glLoadIdentity(); //���þ���Ϊ�˹���ʰȡ����

			glGetIntegerv(GL_VIEWPORT, viewport);//���ص�ǰ�ӿڵ���ֵ��������viewport������
			gluPickMatrix(x, viewport[3] - y, 1, 1, viewport); //����ʰȡ����ǰ��������Ϊ����������cursorת��ΪOpenGL���꣬�������ĸ�������ѡ���Ĵ�С�����һ�������ӿڵ�λ�úʹ�С
			gluPerspective(60, 1, 0.1, 1000); //����һ���̶���ͶӰ���󡣵�ȻҲ���Ա仯���ͶӰ����
			glMatrixMode(GL_MODELVIEW);//������ͼ�任״̬

			renderScene();

			int hits;

			// restoring the original projection matrix
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glFlush();

			// returning to normal rendering model
			hits = glRenderMode(GL_RENDER);

			// if there are hits process them
			if (hits != 0) {
				if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING) {
					Control_h::GLOBAL_CONTROL_STATUS = Control_h::PLACING;
					ProcessHit(hits, Control_h::selectBuf);
				}
				else if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::PLACING) {
					Control_h::GLOBAL_CONTROL_STATUS = Control_h::WANDERING;
				}
			}
		}
		else if (button == GLUT_UP) {
		}
	}
	else if (state == GLUT_RIGHT_BUTTON) {
		/*can't be triggered when tested*/
		if (button == GLUT_DOWN) {
		}
		else if (button == GLUT_UP) {
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

	//assert(numberOfNames == 2);
	if (ptrNames && ptrNames[0] == Layout_h::NameBase) {
		Control_h::ptrCtrlObj = (OperableLayout*)Layout_h::NamedLayout[ptrNames[1]];
		printf("Now controlling: OBJ%04d\n", ptrNames[1]);
		//printf("Control_h::ptrCtrlObj = %016lx\n", Control_h::ptrCtrlObj);
	}
	else {
		Control_h::GLOBAL_CONTROL_STATUS = Control_h::WANDERING;
	}
}

void MouseCtrl::MouseDragProc(int x, int y) {

}

void MouseCtrl::MouseScrollProc(int button, int dir, int x, int y) {
	if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING) {
		Control_h::GlobalZoomRate *= dir > 0 ? 1.1 : (1 / 1.1);
		glutPostRedisplay();
	}
}


class KeyboardCtrl {
private:
	Camera* Cam;

public:
	KeyboardCtrl(Camera* cam);
	void SetAsKeyCtrlCamera() { Control_h::ptrKeyCamera = Cam; }
	static void KeyInProc(unsigned char k, int x, int y);
};

KeyboardCtrl::KeyboardCtrl(Camera* cam) {
	Cam = cam;
}

void KeyboardCtrl::KeyInProc(unsigned char key, int x, int y) {
	if(Control_h::ptrKeyCamera && Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING)
		Control_h::ptrKeyCamera->ProcessInput(key);
	else if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::PLACING) {
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
		case 'a': {
			Control_h::ptrCtrlObj->AddTrans(dirZ / 30, 0, -dirX / 30);
			break;
		}
		case 'd': {
			Control_h::ptrCtrlObj->AddTrans(-dirZ / 30, 0, dirX / 30);
			break;
		}
		case 'w': {
			Control_h::ptrCtrlObj->AddTrans(dirX / 30, 0, dirZ / 30);
			break;
		}
		case 's': {
			Control_h::ptrCtrlObj->AddTrans(-dirX / 30, 0, -dirZ / 30);
			break;
		}
		case ' ': {
			Control_h::ptrCtrlObj->AddTrans(0, 0.05, 0);
			break;
		}
		case 'z': {
			Control_h::ptrCtrlObj->AddTrans(0, -0.05, 0);
			break;
		}
		case 'x': {
			Control_h::ptrCtrlObj->MultScale(1.05);
			break;
		}
		case 'c': {
			Control_h::ptrCtrlObj->MultScale(1 / 1.05);
			break;
		}
		case 'Z': {
			Control_h::ptrCtrlObj->AddRot(0.5, 0, 0);
			break;
		}
		case 'X': {
			Control_h::ptrCtrlObj->AddRot(0, 0.5, 0);
			break;
		}
		case 'C': {
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
	case 'P': {
		ScreenShotCapturer::getScreenShot();
		break;
	}
	case 'b': {	/*Backpack Mode*/
		/*interfaces for GUI*/
		break;
	}
	case 'r': {	/*Settings*/
		/*interfaces for GUI*/
		break;
	}
	default: break;
	}
}

#undef BUFSIZE
#endif