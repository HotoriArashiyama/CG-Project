#pragma once

#ifndef MY_OPENGL_PICK_H
#define MY_OPENGL_PICK_H
#define BUFSIZE 512
extern int mousepressed = 0, leftmouse = 0, rightmouse = 0;
extern bool ongui = 1; 
extern GLuint selectBuf[BUFSIZE]; // 设置一个选择缓冲区
extern double currentX = 0, currentY = 0;
extern float eye[] = { 0, 0, 0 };
extern float poz[] = { 0, 0, 0 };
extern double du = 90, OriX = -1, OriY = -1, duh = 0;
extern float r = 1.5;
extern float c = 3.1415926535 / 180.0;
void processHits(GLint hits, GLuint buffer[])
{
    if (!ongui)
    {
        signed int i;
        unsigned int j;
        GLuint names, * ptr, minZ, * ptrNames, numberOfNames;
        ptrNames = NULL;
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

        printf("The closest hit names are ");
        ptr = ptrNames;
        for (j = 0; j < numberOfNames; j++, ptr++) {
            printf("%d ", *ptr);
        }
        printf("\n");

        assert(numberOfNames == 2);
        s_row = ptrNames[0];
        s_col = ptrNames[1];
        switch (s_row)
        {
        case 0:cgui = "ball"; break;
        case 1:cgui = "cube"; break;
        case 2:cgui = "ellipse"; break;
        case 3:cgui = "cone"; break;
        }
    }
}



void mousepress(GLFWwindow* window, int button, int action, int mods)
{
    if (!ongui)
    {
        cgui = "GUI";
        if (addflag != -1&&(duh*poz[1]<0))
        {
            num[addflag]++;
            initx[addflag][num[addflag] - 1] = p1[0] - p1[1] / (p2[1] - p1[1]) * (p2[0] - p1[0]), inity[addflag][num[addflag] - 1] = p1[2] - p1[1] / (p2[1] - p1[1]) * (p2[2] - p1[2]);
            printf("%lf %lf %lf\n", p1[0], p1[1],p1[2]);
            addflag = -1;
        }

    }
    //Picking
    GLint viewport[4]; //视口大小，四个值按顺序为x, y, width, height
    glSelectBuffer(BUFSIZE, selectBuf);
    glRenderMode(GL_SELECT); //拾取前，需要进入选择模式
    glInitNames();

    glMatrixMode(GL_PROJECTION); //选择投影矩阵。判断是以投影的方式进行的。
    glPushMatrix(); //向状态机压入投影矩阵
    glLoadIdentity(); //重置矩阵，为了构建拾取矩阵

    glGetIntegerv(GL_VIEWPORT, viewport);//返回当前视口的数值，并存入viewport数组中
    gluPickMatrix(currentX, viewport[3] - currentY, 1, 1, viewport); //建立拾取矩阵，前两个参数为将窗口坐标cursor转化为OpenGL坐标，第三、四个参数是选择框的大小，最后一个就是视口的位置和大小

    gluPerspective(75, 800.0 / 600.0, 0.1, 1000.0); //乘以一个固定的投影矩阵。当然也可以变化这个投影矩阵
    glMatrixMode(GL_MODELVIEW);//进入视图变换状态

    drawBalls();
    int hits;

    // restoring the original projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();

    // returning to normal rendering model
    hits = glRenderMode(GL_RENDER);

    // if there are hits process them
    if (hits != 0)
    {
        processHits(hits, selectBuf);
    }

    switch (action)
    {
    case GLFW_PRESS: mousepressed = 1;
        break;
    case GLFW_RELEASE: mousepressed = 0;
        break;
    }
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT: leftmouse = 1;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT: rightmouse = 1;
        break;
    }

}



#endif