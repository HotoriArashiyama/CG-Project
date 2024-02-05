#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>
#include <assert.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

/*Complemented Sources*/
#include "utils.h"
#include "Camera.h"
#include "Control.h"
#include "Render.h"
#include "Layout.h"
#include "DualLight.h"
#include "GUI.h"
#include "LoadTexture.h"
#include "txts.h"

using namespace std;

/*Global Variables*/
GLFWwindow* ptrWindow;
int SCRN_W = WINDOW_WIDTH, SCRN_H = WINDOW_HEIGHT;
Camera MainCam(0, 0, 2.5);
KeyboardCtrl KeyIn(&MainCam);
MouseCtrl MouseIn(&MainCam);
unsigned int TexsDesc[10];

// GUI
GUI GlobalGUI;
// Layouts Not Using Shaders
string SkyBoxSrcs[6];
string TerrainSrcs;
SkyBox GlobalBackGround(0, -0.5, 2.5, 20, 10, 20, NULL);
Terrain GlobalTerrain(0, -3, 2.5, 100, 1, 100, NULL);
SolidTeapot TeapotContainer;
SolidCube CubeContainer;
SolidSphere SphereContainer;
SolidTorus TorusContainer;
SolidCone ConeContainer;
SolidCylinder CylinderContainer;
GeneralPrismCreator PrismContainer;
ObjHandler ObjLoader;
glm::mat4 BeginningViewMatrix;
glm::mat4 projection;
glm::mat4 view;
// Shaders
Shader NrmlObjShader;
Shader LightCubeShader;
DualLight Lights;
// Objs
Model GediksGuitarBackpack;
Model CampFire;
Model CampFirePutOut;
Model Lantern;
Model Bench;
Model Cottage1;
Model Cottage3;
Model Cottage4;
Model RoundWood;
Model Logs5;
Model Snowman1;
Model TreeObj;
Model Mountain;
OperableLayout GediksGuitarBackPackContainer;
OperableLayout CampFireContainer;
OperableLayout LanternContainer;
OperableLayout BenchContainer;
OperableLayout Cottage1Container;
OperableLayout Cottage3Container;
OperableLayout Cottage4Container;
OperableLayout RoundWoodContainer;
OperableLayout Logs5Container;
OperableLayout Snowman1Container;
OperableLayout TreeObjContainer;
OperableLayout MountainContainer;
bool IsDebugging = false;

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
	SkyBoxSrcs[0] = "./res/textures/skybox/posX.bmp";
	SkyBoxSrcs[1] = "./res/textures/skybox/negX.bmp";
	SkyBoxSrcs[2] = "./res/textures/skybox/posY.bmp";
	SkyBoxSrcs[3] = "./res/textures/skybox/negY.bmp";
	SkyBoxSrcs[4] = "./res/textures/skybox/posZ.bmp";
	SkyBoxSrcs[5] = "./res/textures/skybox/negZ.bmp";
	glGenTextures(6, TexsDesc);
	TexLoadCubeBmp(TexsDesc, SkyBoxSrcs);
	GlobalBackGround.SetTex(TexsDesc);
	MainCam.SetSkyBoxFollow(&GlobalBackGround, 0, -0.5, 0);

	//init Terrain
	TerrainSrcs = "./res/textures/terrain/snow_ground.bmp";
	glGenTextures(1, &TexsDesc[6]);
	TexLoad(TerrainSrcs.c_str(), &TexsDesc[6], GL_REPEAT, GL_NEAREST, GL_LINEAR);
	GlobalTerrain.SetTex(&TexsDesc[6]);
	MainCam.SetTerrainFollow(&GlobalTerrain, 0, -3, 0, false);

	//init other simple tex
	{
		unsigned int tsmpl;
		glGenTextures(1, &tsmpl);
		TexLoad("./res/textures/basic/bark02.png", &tsmpl, GL_REPEAT, GL_NEAREST, GL_LINEAR);
		Layout_h::Tex4NonShdrObj.push_back(tsmpl);
		glGenTextures(1, &tsmpl);
		TexLoad("./res/textures/basic/wall.jpg", &tsmpl, GL_REPEAT, GL_NEAREST, GL_LINEAR);
		Layout_h::Tex4NonShdrObj.push_back(tsmpl);
		glGenTextures(1, &tsmpl);
		TexLoad("./res/textures/basic/wood.jpg", &tsmpl, GL_REPEAT, GL_NEAREST, GL_LINEAR);
		Layout_h::Tex4NonShdrObj.push_back(tsmpl);
	}

	//init containers of object from glut tools
	TeapotContainer.SetName();
	CubeContainer.SetName();
	SphereContainer.SetName();
	TorusContainer.SetName();
	ConeContainer.SetName();
	CylinderContainer.SetName();
	PrismContainer.SetName();

	//init light instance
	DualLight_h::InitLightCube();

	//init shaders
	NrmlObjShader.Initialize(txts_h::vtxShaderNrmlObj, txts_h::fragShaderNrmlObj);
	LightCubeShader.Initialize(txts_h::vtxShaderLightCube, txts_h::fragShaderLightCube);
	////multi-light shader, abandoned for its awful appearance
	//Lights.Initialize(txts_h::vtxShaderDualLight, txts_h::fragShaderDualLight);

	//init other models from file
	ObjLoader.ObjLoad("./res/objects/backpack/backpack.obj", &GediksGuitarBackpack);
	GediksGuitarBackpack.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/campfire/Campfire.obj", &CampFire);
	CampFire.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/campfire/Campfireputout.obj", &CampFirePutOut);
	CampFirePutOut.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/lantern/lantern.obj", &Lantern);
	Lantern.setShdr(NrmlObjShader.getHdl());
	//ObjLoader.ObjLoad("./res/objects/Bench/Bench_LowRes.obj", &Bench);
	//Bench.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/cottage1/Snow_covered_CottageOBJ.obj", &Cottage1);
	Cottage1.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/cottage5/SM_Villa_Weber.obj", &Cottage3, false);
	Cottage3.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/cottage4/003_obj.obj", &Cottage4);
	Cottage4.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/roundwood/Wood.obj", &RoundWood);
	RoundWood.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/logs/log_cgt_5.obj", &Logs5);
	Logs5.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/snowman1/snowman_finish.obj", &Snowman1);
	Snowman1.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/PineTree/PineTree.obj", &TreeObj, false);
	TreeObj.setShdr(NrmlObjShader.getHdl());
	ObjLoader.ObjLoad("./res/objects/mountain/Snow Covered Mountain Range in Northern Montana.obj", &Mountain, false);
	Mountain.setShdr(NrmlObjShader.getHdl());

	//init containers of models from file
	GediksGuitarBackPackContainer.SetObj(&GediksGuitarBackpack);
	GediksGuitarBackPackContainer.SetName();
	CampFireContainer.SetObj(&CampFire);
	CampFireContainer.SetName();
	CampFireContainer.SetSpecialOptions(Layout_h::LIGHTSOURCE);
	CampFireContainer.SetCompulsoryScale(0.01);
	CampFireContainer.SetSpareTex(&CampFirePutOut);
	LanternContainer.SetObj(&Lantern);
	LanternContainer.SetSpecialOptions(Layout_h::LIGHTSOURCE);
	LanternContainer.SetCompulsoryScale(2.50);
	//BenchContainer.SetObj(&Bench);
	//BenchContainer.SetName();
	//BenchContainer.SetCompulsoryScale(0.02);
	Cottage1Container.SetObj(&Cottage1);
	Cottage1Container.SetName();
	Cottage1Container.SetCompulsoryScale(0.1);
	Cottage3Container.SetObj(&Cottage3);
	Cottage3Container.SetName();
	Cottage3Container.SetCompulsoryScale(0.01);
	Cottage4Container.SetObj(&Cottage4);
	Cottage4Container.SetName();
	RoundWoodContainer.SetObj(&RoundWood);
	RoundWoodContainer.SetName();
	RoundWoodContainer.SetCompulsoryScale(0.5);
	Logs5Container.SetObj(&Logs5);
	Logs5Container.SetName();
	Logs5Container.SetCompulsoryScale(1.5);
	Snowman1Container.SetObj(&Snowman1);
	Snowman1Container.SetName();
	TreeObjContainer.SetObj(&TreeObj);
	TreeObjContainer.SetName();
	TreeObjContainer.SetCompulsoryScale(0.01);
	MountainContainer.SetObj(&Mountain);
	MountainContainer.SetName();
	MountainContainer.SetCompulsoryScale(0.001);
	PrismContainer.SetSpecialOptions(Layout_h::GENERALPRISMGENERATOR);

	//add obj containers into the gui layout list
	GlobalGUI.Add("Solid Teapot", &TeapotContainer);
	GlobalGUI.Add("Solid Cube", &CubeContainer);
	GlobalGUI.Add("Solid Sphere", &SphereContainer);
	GlobalGUI.Add("Solid Torus", &TorusContainer);
	GlobalGUI.Add("Solid Cone", &ConeContainer);
	GlobalGUI.Add("Solid Cylinder", &CylinderContainer);
	GlobalGUI.Add("Editable Prism", &PrismContainer);
	GlobalGUI.Add("Berk Gedik's guitar backpack", &GediksGuitarBackPackContainer);
	GlobalGUI.Add("Campfire", &CampFireContainer);
	GlobalGUI.Add("Lantern", &LanternContainer);
	//GlobalGUI.Add("Bench", &BenchContainer);
	GlobalGUI.Add("Cottage1", &Cottage1Container);
	GlobalGUI.Add("Cottage2", &Cottage4Container);
	GlobalGUI.Add("Cottage3", &Cottage3Container);
	GlobalGUI.Add("Tree", &TreeObjContainer);
	GlobalGUI.Add("Logs1", &RoundWoodContainer);
	GlobalGUI.Add("Logs2", &Logs5Container);
	GlobalGUI.Add("Snowman Twins", &Snowman1Container);
	GlobalGUI.Add("Mountain (CAUTION: REAL LARGE)", &MountainContainer);
	
}

static void reshape(GLFWwindow *window, int w, int h)
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
	if (!Control_h::PIPELINE_INTERRUPTION) {
		glfwPollEvents();
		GLClearError();
	}
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	view = MainCam.glmGetViewMatrix();
	projection = glm::perspective(glm::radians((float)Control_h::GlobalZoomRate), (float)SCRN_W / (float)SCRN_H, 1.0f, 1000.0f);

	MainCam.glutUpdateLook();

	if (!Control_h::PIPELINE_INTERRUPTION) {
		/*Keyboard Event*/
		KeyboardCtrl::KeyProc4GLFWLoop(ptrWindow);
		/*MouseWheel Event*/
		MouseCtrl::MouseScrollProc4GLFWLoop(ptrWindow);
		/*MouseButton Event*/
		MouseCtrl::MouseClickProc4GLFWLoop(ptrWindow);
	}

	/*Draw Light*/ {
		if (Control_h::AcquireAddLight) {
			Lights.AddLight(Control_h::AcquireAddLight);
			Control_h::AcquireAddLight = NULL;
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_pos[] = { 0,5,15,1 };

		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, white);
		glEnable(GL_LIGHT0);
		Lights.glutEnlighten();

		//A light cube, now replaced by lamp obj.
		if (IsDebugging) {
			glUseProgram(LightCubeShader.getHdl()); {
				LightCubeShader.glmSetMat4("projection", projection);
				LightCubeShader.glmSetMat4("view", view);
				/*draw objs with shader*/ {
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(0, 0, 2.5)); // translate it down so it's at the center of the scene
					model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
					LightCubeShader.glmSetMat4("model", model);
					LightCubeShader.glmSetVec4("lightColor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
					glBindVertexArray(DualLight_h::cubeVAO);
					glDrawArrays(GL_TRIANGLES, 0, 36);
					glBindVertexArray(0);
				}
			}
			glUseProgram(0);
		}
	}

	/*Draw SkyBox*/ {
		GlobalBackGround.DrawSky();
	}

	/*Draw Terrain*/ {
		GlobalTerrain.DrawTerrain();
	}

	/*Draw Other Layouts (No Shader)*/ {
		if (IsDebugging) {
			glColor3f(1.0, 0.0, 0.0);
			TeapotContainer.Draw();
		}
	}

	/*Draw Loaded Objs from File (Using Shader)*/
	glUseProgram(NrmlObjShader.getHdl()); {
		NrmlObjShader.glmSetMat4("projection", projection);
		NrmlObjShader.glmSetMat4("view", view);
		/*draw objs with shader*/ {
			/*Gedik's guitar backpack*/
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-MainCam.getPos('X'), -MainCam.getPos('Y'), -MainCam.getPos('Z'))); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			NrmlObjShader.glmSetMat4("model", model);
			if (IsDebugging) GediksGuitarBackpack.Draw();
		}
	}
	glUseProgram(0);

	/*Draw Objs added by Player*/
	glUseProgram(NrmlObjShader.getHdl()); {
		NrmlObjShader.glmSetMat4("projection", projection);
		NrmlObjShader.glmSetMat4("view", view);
		PlayerAddedLayout::DrawPlayerAddedLayouts(NrmlObjShader.getHdl());
	}
	glUseProgram(0);
	////multi-light shader, abandoned for its awful appearance
	//if(IsDebugging) {
	//	Lights.use();
	//	Lights.Enlighten();
	//	Lights.getShdr().glmSetVec3("viewPos", glm::vec3(MainCam.getPos('X'), MainCam.getPos('Y'), MainCam.getPos('Z')));
	//	Lights.getShdr().glmSetMat4("projection", projection);
	//	Lights.getShdr().glmSetMat4("view", view);
	//	PlayerAddedLayout::DrawPlayerAddedLayouts(Lights.getShdr().getHdl(), true);
	//	glUseProgram(0);
	//}

	if (!Control_h::PIPELINE_INTERRUPTION) {
		ImGui_ImplGlfwGL3_NewFrame();

		GlobalGUI.Draw();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(ptrWindow);
	}

	//// Because of the compatibility of lib imgui, glut window is now abandoned.
	//glutSwapBuffers();
}

static void glfw_KeyboardProc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//This interface will be replaced by imgui.
	int k = key;
	if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
		if (!(mods & GLFW_MOD_SHIFT)) {
			k = k | 0x20;
		}
	}
	KeyboardCtrl::KeyInProc(k, 0, 0);
	KeyboardCtrl::KeyProcExt4GLFW(window, key, scancode, action, mods);
}

static void glfw_MouseMoveProc(GLFWwindow* window, double xposIn, double yposIn) {
	//equivalent to glutMotionFunc, glutPassiveMotionFunc
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		Camera::SightMouseFollow(xposIn, yposIn);
		Control_h::viewportX = xposIn, Control_h::viewportY = yposIn;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		MainCam.SetPrevXY(xposIn, yposIn);
	}
	else {
		return;
	}
}

static void glfw_MouseClickProc(GLFWwindow* window, int button, int action, int mods) {
	//equivalent to glutMouseFunc
	//This interface will be replaced by imgui.
}

static void glfw_MouseScrollProc(GLFWwindow* window, double xoffset, double yoffset) {
	//This interface will be replaced by imgui.
	MouseCtrl::MouseScrollProc(window, xoffset, yoffset);
}

static void errorCallback(int error, const char* description) {
	printf("[ERROR %d]: %s\n", error, description);
}

static GLFWwindow* initGlfw() {
	GLFWwindow* p;
	assert(glfwInit() && "GlfwInit Error");

	p = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CGFinalProject", NULL, NULL);

	if (!p) {
		glfwTerminate();
		assert(p && "Glfw Window Failed");
	}
	printf("GLFW Window Created\n");

	glfwMakeContextCurrent(p);
	glfwSetFramebufferSizeCallback(p, reshape);
	glfwSetCursorPosCallback(p, glfw_MouseMoveProc);//equivalent to glutMotionFunc, glutPassiveMotionFunc
	//These three callback funcs will be replaced by imgui_impl_glfw_gl3. Input keyboard and mouse button/wheel have to be handled in main loop.
	glfwSetKeyCallback(p, glfw_KeyboardProc);
	glfwSetMouseButtonCallback(p, glfw_MouseClickProc);//equivalent to glutMouseFunc
	glfwSetScrollCallback(p, glfw_MouseScrollProc);//equivalent to glutMouseWheelFunc

	glfwSwapInterval(1);

	return p;
}

int main(int argc, char* argv[])
{
	// glfw init
	glfwSetErrorCallback(errorCallback);
	ptrWindow = initGlfw();

	// glew init
	//glewExperimental = GL_TRUE;
	assert(glewInit() == GLEW_OK && "GLEW Init Failed");

	// imgui init
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(ptrWindow, true);
	ImGui::StyleColorsDark();

	init();

	reshape(ptrWindow, WINDOW_WIDTH, WINDOW_HEIGHT);
	ImGuiIO& startImguiIO = ImGui::GetIO();
	Control_h::prevImguiMouseWheel = startImguiIO.MouseWheel;
	while (!glfwWindowShouldClose(ptrWindow)) {
		renderScene();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	//// Because of the compatibility of lib imgui, glut window is now abandoned.
	//// glut init
	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//glutInitWindowPosition(100, 100);
	//glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutCreateWindow("CGFinalProject");

	//// Because of the compatibility of lib imgui, glut window is now abandoned.
	//glutReshapeFunc(reshape);
	//glutDisplayFunc(renderScene);
	//glutIdleFunc(renderScene);
	//glutMouseFunc(MouseCtrl::MouseClickProc);
	//glutMotionFunc(MouseCtrl::MouseDragProc);
	//glutMouseWheelFunc(MouseCtrl::MouseScrollProc);
	//glutPassiveMotionFunc(Camera::SightMouseFollow);
	//glutKeyboardFunc(KeyboardCtrl::KeyInProc);

	//glutMainLoop();

	return 0;
}