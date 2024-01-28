#pragma once
#ifndef MY_OPENGL_CAMERA_H
#define MY_OPENGL_CAMERA_H

void onMouseMove(GLFWwindow* window, double x, double y)
{
    // Handle mouse movement event
    if (mousepressed && !ongui)
    {
        if (fabs(x - OriX) < 20 && fabs(y - OriY) < 20)
        {
            du += 0.4 * (x - OriX);
            duh += 0.5 * (y - OriY);
            if (duh > 60)
                duh = 60;
            if (duh < -60)
                duh = -60;
        }
        OriX = x;
        OriY = y;
    }

    currentX = x;
    currentY = y;
}

void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case 32: {
        eye[1] = 0.1;
        break;
    }
    case 341: {
        eye[1] = -0.1;
        break;
    }
    case 65: {
        eye[0] = -0.1;
        break;
    }
    case 68: {
        eye[0] = 0.1;
        break;
    }
    case 83: {
        eye[2] = 0.1;
        break;
    }
    case 87: {
        eye[2] = -0.1;
        break;
    }
    case 262: {
        move[0] = 0.1;
        break;
    }
    case 263: {
        move[0] = -0.1;
        break;
    }
    case 264: {
        move[1] = 0.1;
        break;
    }
    case 265: {
        move[1] = -0.1;
        break;
    }
    case 256: {
        cgui = "GUI";
        addflag = -1;
        break;
    }
    case 44: {
        turn = -1;
        break;
    }
    case 46: {
        turn = 1;
        break;
    }
    }

    // Update camera position and orientation
    if (cgui != "GUI")
    {
        if (move[0] != 0 || move[1] != 0)
        {
            posx[s_row][s_col] += move[0] * sin(c * du) - move[1] * cos(c * du);
            posy[s_row][s_col] += move[0] * cos(c * du) + move[1] * sin(c * du);
        }
        if (turn != 0)
        {
            angle[s_row][s_col] += turn;
        }
    }
    move[0] = 0;
    move[1] = 0;
}

void camera(void)
{
    // Update camera position and orientation
    poz[0] += eye[0] * sin(c * du) - eye[2] * cos(c * du);
    poz[1] += eye[1];
    poz[2] += eye[0] * cos(c * du) + eye[2] * sin(c * du);
    gluLookAt(
        poz[0],
        poz[1],
        poz[2] + 5.5,
        poz[0] + r * cos(duh * c) * cos(c * du),
        poz[1] + r * sin(c * duh),
        poz[2] + 5.5 - r * cos(duh * c) * sin(c * du),
        0, 1.0, 0.0);

    // Update camera position for drawing lines
    p1[0] = poz[0];
    p1[1] = poz[1];
    p1[2] = poz[2] + 5.5;
    p2[0] = poz[0] + r * cos(duh * c) * cos(c * du);
    p2[1] = poz[1] + r * sin(c * duh);
    p2[2] = poz[2] + 5.5 - r * cos(duh * c) * sin(c * du);

    eye[0] = 0;
    eye[1] = 0;
    eye[2] = 0;
}

#endif // MY_OPENGL_CAMERA_H