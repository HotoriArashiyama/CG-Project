#pragma once
#ifndef __GUI_H__
#define __GUI_H__

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <assert.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw_gl3.h" //can't be included twice

#include "Layout.h"
#include "Control.h"
#include "utils.h"

namespace GUI_h{
    const char* cgui = "GUI";
    bool ongui = false;
    int addflag = -1;
}

class GUI
{
public:
	GUI();
	~GUI();
	void Draw();
    void Add(std::string name, OperableLayout* obj);
    std::vector<std::pair<std::string, OperableLayout*>> ObjList;
    std::vector<std::pair<ImVec2, int>> Btns;

private:

};

GUI::GUI()
{
}

GUI::~GUI()
{
}

void GUI::Add(std::string name, OperableLayout* obj) {
    ObjList.push_back(std::pair<std::string, OperableLayout*>(name, obj));
    ImVec2 n;
    float nlen = (name.length()) * 10;
    if (Btns.size() == 0) {
        n = ImVec2(5, 50);
        Btns.push_back(std::pair<ImVec2, int>(n, nlen));
        return;
    }
    ImVec2 t = Btns.back().first;
    float tlen = Btns.back().second * 10;
    if (t[0] + tlen >= 300) {
        n = ImVec2(5, t[1] + 25);
    }
    else {
        n = ImVec2(t[0] + tlen, t[1]);
    }
    Btns.push_back(std::pair<ImVec2, int>(n, nlen));
}

void GUI::Draw() {

    ImGui::Begin("UI");
    if (ImGui::IsWindowHovered() || (!Control_h::GUIMousePressedSetTrue)) // 检测鼠标是否在窗口上
    {
        GUI_h::ongui = true;
    }
    //ImGui::Text("%s", GUI_h::cgui);
    if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::WANDERING) {
        ImGui::Text("GUI: Press e to furnish the scene.");
    }
    else if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::PLACING) {
        ImGui::Text("GUI: Selecting %s. Press e to edit.", ObjList[Layout_h::NameInGUI[Control_h::ptrCtrlObj->getName()]].first.c_str());
    }

    else if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::BACKPACK_GUI)
    {
        ImGui::Text("GUI: Furnishing. Press e to exit.");
        for (int i = 0; i < ObjList.size(); i++) {
            std::pair<std::string, OperableLayout*> p = ObjList[i];
            std::pair<ImVec2, int> b = Btns[i];

            ImGui::SetCursorPos(b.first);
            if (ImGui::Button(p.first.c_str())) // 添加一个按钮
            {
                Control_h::Obj2AddId = i;
                Control_h::Obj2Add = ObjList.begin() + i;
            }
        }
        //// now use vetor to store addable objs, this part of code is abandoned
        //ImVec2 buttonPos1(5, 50); // 按钮的位置
        //ImVec2 buttonPos2(55, 50);
        //ImVec2 buttonPos3(105, 50);
        //ImVec2 buttonPos4(175, 50);
        //ImGui::SetCursorPos(buttonPos1);
        //if (ImGui::Button("ball+")) // 添加一个按钮
        //{
        //    addflag = 0;
        //}
        //ImGui::SetCursorPos(buttonPos2);
        //if (ImGui::Button("cube+"))
        //{
        //    addflag = 1;
        //}
        //ImGui::SetCursorPos(buttonPos3);
        //if (ImGui::Button("ellipse+"))
        //{
        //    addflag = 2;
        //}
        //ImGui::SetCursorPos(buttonPos4);
        //if (ImGui::Button("cone+"))
        //{
        //    addflag = 3;
        //}
        //ImGui::SliderFloat("X", &X, 0.0f, 1.0f);
        //ImGui::SliderFloat("Y", &Y, 0.0f, 1.0f);
        //ImGui::SliderFloat("Z", &Z, 0.0f, 1.0f);
    }
    else if (Control_h::GLOBAL_CONTROL_STATUS == Control_h::EDITING)
    {
        std::map<int, void*>::iterator it = Layout_h::AddedLayout.find(Control_h::ptrCtrlObj->getName());
        OperableLayout* p = NULL;
        if (it != Layout_h::AddedLayout.end())
            p = (OperableLayout*)(it->second);
        ImGui::Text(p ? "GUI: Editing %s. Press e to exit.\nX: %f, Y: %f, Z: %f" : "GUI: Editing %s. Press e to exit.\n",
            ObjList[Layout_h::NameInGUI[Control_h::ptrCtrlObj->getName()]].first.c_str(),
            p ? p->getPos('X') : 0, p ? p->getPos('Y') : 0, p ? p->getPos('Z') : 0);
        /*TODO: in placing mode select to operate the obj*/
        if (ImGui::Button("delete"))
        {
            Control_h::ptrCtrlObj->SetDisplayHide();
        }
        Control_h::ptrCtrlObj->OperationShowedGUI();
        //ImGui::SliderFloat("R ambient", &ambientR[s_row][s_col], 0.0f, 1.0f);
        //ImGui::SliderFloat("G ambient", &ambientG[s_row][s_col], 0.0f, 1.0f);
        //ImGui::SliderFloat("B ambient", &ambientB[s_row][s_col], 0.0f, 1.0f);
        //ImGui::SliderFloat("R diffuse", &diffuseR[s_row][s_col], 0.0f, 1.0f);
        //ImGui::SliderFloat("G diffuse", &diffuseG[s_row][s_col], 0.0f, 1.0f);
        //ImGui::SliderFloat("B diffuse", &diffuseB[s_row][s_col], 0.0f, 1.0f);
        //ImGui::SliderFloat("R specular", &specularR[s_row][s_col], 0.0f, 1.0f);
        //ImGui::SliderFloat("G specular", &specularG[s_row][s_col], 0.0f, 1.0f);
        //ImGui::SliderFloat("B specular", &specularB[s_row][s_col], 0.0f, 1.0f);
    }

    ImGui::End();
}

#endif // !__GUI_H__
