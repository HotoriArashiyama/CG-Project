#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <assert.h>

#pragma comment(lib, "glew32.lib")
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/*Complemented Sources*/
#include "utils.h"
#include "Camera.h"
#include "Control.h"
#include "Render.h"
#include "Layout.h"
#include "LoadTexture.h"
#include "txts.h"

using namespace std;

/*Global Variables*/
int SCRN_W = WINDOW_WIDTH, SCRN_H = WINDOW_HEIGHT;
Camera MainCam(0, 0, 2.5);
KeyboardCtrl KeyIn(&MainCam);
MouseCtrl MouseIn(&MainCam);
unsigned int TexsDesc[10];
string SkyBoxSrcs[6];
string TerrainSrcs;
SkyBox GlobalBackGround(0, -0.5, 2.5, 20, 10, 20, NULL);
Terrain GlobalTerrain(0, -3, 2.5, 100, 1, 100, NULL);
SolidTeapot Teapot1;
ObjHandler ObjLoader;
glm::mat4 BeginningViewMatrix;
glm::mat4 projection;
Shader NrmlObjShader;
Model GediksGuitarBackpack;

static void init()
{
	glEnable(GL_DEPTH_TEST);    //turn on depth
	projection = glm::perspective(glm::radians((float)Control_h::GlobalZoomRate), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 1000.0f);

	//bind camera, keyboard, mouse
	MainCam.SetAsMainCam();
	KeyIn.SetAsKeyCtrlCamera();
	MouseIn.SetAsMouseCtrlCamera();
	BeginningViewMatrix = MainCam.glmGetViewMatrix();

	//init SkyBox
	SkyBoxSrcs[0] = "./texture/skybox/posX.bmp";
	SkyBoxSrcs[1] = "./texture/skybox/negX.bmp";
	SkyBoxSrcs[2] = "./texture/skybox/posY.bmp";
	SkyBoxSrcs[3] = "./texture/skybox/negY.bmp";
	SkyBoxSrcs[4] = "./texture/skybox/posZ.bmp";
	SkyBoxSrcs[5] = "./texture/skybox/negZ.bmp";
	glGenTextures(6, TexsDesc);
	TexLoadCubeBmp(TexsDesc, SkyBoxSrcs);
	GlobalBackGround.SetTex(TexsDesc);
	MainCam.SetSkyBoxFollow(&GlobalBackGround, 0, -0.5, 0);

	//init Terrain
	TerrainSrcs = "./texture/terrain/snow_ground.bmp";
	glGenTextures(1, &TexsDesc[6]);
	TexLoad(TerrainSrcs.c_str(), &TexsDesc[6], GL_REPEAT, GL_NEAREST, GL_LINEAR);
	GlobalTerrain.SetTex(&TexsDesc[6]);
	MainCam.SetTerrainFollow(&GlobalTerrain, 0, -3, 0, false);

	//init Teapot
	Teapot1.SetName();

	//init shaders
	NrmlObjShader.Initialize(txts_h::vtxShaderNrmlObj, txts_h::fragShaderNrmlObj);

	//init other models from file
	ObjLoader.ObjLoad("./objects/backpack/backpack.obj", &GediksGuitarBackpack);
	GediksGuitarBackpack.setShdr(NrmlObjShader.getHdl());
	
}

static void reshape(int w, int h)
{
	glViewport(0, 0, w, h);    //截图;1、2为视口的左下角;3、4为视口的宽度和高度
	glMatrixMode(GL_PROJECTION);    //将当前矩阵指定为投影矩阵
	glLoadIdentity();
	gluPerspective(Control_h::GlobalZoomRate, (float)w / h, 1.0, 1000.0); //1、视野在Y-Z平面的角度[0,180];2、投影平面宽度与高度的比率;3、近截剪面到视点的距离;4、远截剪面到视点的距离
	projection = glm::perspective(glm::radians((float)Control_h::GlobalZoomRate), (float)w / (float)h, 1.0f, 1000.0f);
	SCRN_W = w, SCRN_H = h;
	glMatrixMode(GL_MODELVIEW);     //对模型视景矩阵堆栈应用随后的矩阵操作.
}

static void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	MainCam.glutUpdateLook();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 5,5,5,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

	GlobalBackGround.DrawSky();
	//glScalef(Control_h::GlobalZoomRate, Control_h::GlobalZoomRate, Control_h::GlobalZoomRate);
	// // may cause unexpected bug with skybox, now zooming is implemented by adjusting fovy
	GlobalTerrain.DrawTerrain();

	glColor3f(1.0, 0.0, 0.0);
	Teapot1.Draw();

	//loaded obj from file
	glUseProgram(NrmlObjShader.getHdl());
	//	view/projection transformations
	glm::mat4 view = MainCam.glmGetViewMatrix();
	NrmlObjShader.glmSetMat4("projection", projection);
	NrmlObjShader.glmSetMat4("view", view);
	//	render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-MainCam.getPos('X'), -MainCam.getPos('Y'), -MainCam.getPos('Z'))); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	NrmlObjShader.glmSetMat4("model", model);
	NrmlObjShader.glmSetVec3("desiredPos", glm::vec3(0.0, 2.5, 0.0));
	GediksGuitarBackpack.Draw();
	glUseProgram(0);
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	// glut init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("CGFinalProject");

	// glew init
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	assert(GLEW_OK == err);

	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMouseFunc(MouseCtrl::MouseClickProc);
	glutMotionFunc(MouseCtrl::MouseDragProc);
	glutMouseWheelFunc(MouseCtrl::MouseScrollProc);
	glutPassiveMotionFunc(Camera::SightMouseFollow);
	glutKeyboardFunc(KeyboardCtrl::KeyInProc);

	glutMainLoop();
	return 0;
}