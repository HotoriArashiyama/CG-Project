
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl2.h"
#include "gl/glut.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include "draw.h"
#include "pick.h"
#include "gui.h"
#include "camera.h"
GLfloat white[] = { 1,1,1, 1.f };
GLuint selectBuf[BUFSIZE];
int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1600, 1200, "GLFW Project", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    createList();
    glfwSetCursorPosCallback(window, onMouseMove);
    glfwSetKeyCallback(window, key);
    glfwSetMouseButtonCallback(window, mousepress);

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* Render here */
        glClear(GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(75, 800.0 / 600.0, 0.1, 1000.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        camera();

        glPushMatrix(); // 设置一个新的坐标系
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
        glScalef(100, 0.1, 100);
        glTranslatef(0, -2, 0);
        glutSolidCube(0.1);
        glPopMatrix(); // 回到前一个坐标系

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light_pos[] = { 5,5,5,1 };

        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, white);
        glEnable(GL_LIGHT0);

        glColor3f(1.0, 0.0, 0.0);

        drawBalls();

        gui();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}