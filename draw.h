#pragma once
#ifndef MY_OPENGL_DRAW_H
#define MY_OPENGL_DRAW_H
#define am 4
extern const char* cgui= "GUI";
extern GLuint BALLSDL=1, CUBESDL=1,ELLIPSESDL=1,CONESDL=1;
extern float X = 0.5, Y = 0.5, Z= 0.5;
extern float move[2] = { 0 },turn=0;
extern int s_row=99, s_col=99;
extern int i = 0, j = 0,addflag=-1;
extern int num[am] = { 0 };
extern float p1[] = { 0,0,0 }, p2[] = { 0,0,0 }, initx[am][100] = { 0 }, inity[am][100] = { 0 }, angle[am][100] = { 0 };
extern bool deleted[am][100] = { 0 };
extern float posx[am][100] = {0}, posy[am][100] = {0}, ambientR[am][100] = { 0 }, ambientG[am][100] = { 0 }, ambientB[am][100] = { 0 }, diffuseR[am][100] = { 0 }, diffuseG[am][100] = { 0 }, diffuseB[am][100] = { 0 }, specularR[am][100] = { 0 }, specularG[am][100] = { 0 }, specularB[am][100] = { 0 };
void createList()
{
    GLfloat mat[] = {0.2,0,0, 1.f };
    if (i == s_row && j == s_col && cgui != "GUI")
    {
       
        mat[0] = 1;
        mat[1] = 1;
        mat[2] = 1;
        
    }
    {
        BALLSDL = glGenLists(1);
        GLUquadricObj* sphere;
        glNewList(BALLSDL, GL_COMPILE);
        sphere = gluNewQuadric();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat);
        gluSphere(sphere, 0.2, 10, 10);
        gluDeleteQuadric(sphere);
        glEndList();
    }//
    {
        CUBESDL = glGenLists(1);
        glNewList(CUBESDL, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat);
        glBegin(GL_QUADS);
        glutSolidCube(0.3);
        glEnd();

        glEndList();
    
    }//
    {
        ELLIPSESDL = glGenLists(1);
        glNewList(ELLIPSESDL, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat);
        GLUquadricObj* ellipsoid = gluNewQuadric();
        gluQuadricDrawStyle(ellipsoid, GLU_FILL);
        glScalef(1.0f, 0.6f, 0.6f);
        gluSphere(ellipsoid, 0.2f, 20, 20);
        gluDeleteQuadric(ellipsoid);

        glEndList();

    }//
    {
        CONESDL = glGenLists(1);
        glNewList(CONESDL, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat);
        GLUquadricObj* cone = gluNewQuadric();
        gluQuadricDrawStyle(cone, GLU_FILL);
        gluCylinder(cone, 0.0f, 0.2f, 0.2f, 20, 1);
        gluDeleteQuadric(cone);
        glEndList();
    }//
}

void drawBalls()
{

    for ( i = 0; i <am; i++)
    {
        glPushName(i);
        for ( j = 0; j <num[i]; j++) {


            glPushMatrix();
            glPushName(j);
    
            glTranslatef(initx[i][j], 0, inity[i][j]);
            glTranslatef(posx[i][j], 0,posy[i][j]);
            glRotatef(angle[i][j], 0, 1, 0);
            if (!deleted[i][j])
            {
                if (i == s_row && j == s_col && cgui!="GUI")
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else
                
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                switch (i)
                {
                case 0:glCallList(BALLSDL); break;
                case 1:glCallList(CUBESDL); break;
                case 2:glCallList(ELLIPSESDL); break;
                case 3:glCallList(CONESDL); break;

                }
            }
            glPopMatrix();
            glPopName();

        }
        glPopName();
    }
}

#endif