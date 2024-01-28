#pragma once

#ifndef MY_OPENGL_PICK_H
#define MY_OPENGL_PICK_H
#define BUFSIZE 512
extern int mousepressed = 0, leftmouse = 0, rightmouse = 0;
extern bool ongui = 1; 
extern GLuint selectBuf[BUFSIZE]; // ����һ��ѡ�񻺳���
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
    GLint viewport[4]; //�ӿڴ�С���ĸ�ֵ��˳��Ϊx, y, width, height
    glSelectBuffer(BUFSIZE, selectBuf);
    glRenderMode(GL_SELECT); //ʰȡǰ����Ҫ����ѡ��ģʽ
    glInitNames();

    glMatrixMode(GL_PROJECTION); //ѡ��ͶӰ�����ж�����ͶӰ�ķ�ʽ���еġ�
    glPushMatrix(); //��״̬��ѹ��ͶӰ����
    glLoadIdentity(); //���þ���Ϊ�˹���ʰȡ����

    glGetIntegerv(GL_VIEWPORT, viewport);//���ص�ǰ�ӿڵ���ֵ��������viewport������
    gluPickMatrix(currentX, viewport[3] - currentY, 1, 1, viewport); //����ʰȡ����ǰ��������Ϊ����������cursorת��ΪOpenGL���꣬�������ĸ�������ѡ���Ĵ�С�����һ�������ӿڵ�λ�úʹ�С

    gluPerspective(75, 800.0 / 600.0, 0.1, 1000.0); //����һ���̶���ͶӰ���󡣵�ȻҲ���Ա仯���ͶӰ����
    glMatrixMode(GL_MODELVIEW);//������ͼ�任״̬

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