#pragma once
#ifndef MY_OPENGL_gui_H
#define MY_OPENGL_gui_H

void gui()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create a window called "Hello, world!" and append into it.
    ImGui::Begin("UI");
    if (ImGui::IsWindowHovered()) // 检测鼠标是否在窗口上
    {
        ongui = 1;
    }
    else
    {
        if(!mousepressed)
        ongui = 0;
    }
    ImGui::Text("%s",cgui);               // Display some text 

    if (cgui == "GUI")
    {
        ImVec2 buttonPos1(5, 50); // 按钮的位置
        ImVec2 buttonPos2(55, 50); 
        ImVec2 buttonPos3(105, 50);
        ImVec2 buttonPos4(175, 50);

        ImGui::SetCursorPos(buttonPos1);
        if (ImGui::Button("ball+")) // 添加一个按钮
        {
            addflag = 0;
        }
        ImGui::SetCursorPos(buttonPos2);
        if (ImGui::Button("cube+")) 
        {
            addflag = 1;
        }
        ImGui::SetCursorPos(buttonPos3);
        if (ImGui::Button("ellipse+")) 
        {
            addflag = 2;
        }

        ImGui::SetCursorPos(buttonPos4);
        if (ImGui::Button("cone+")) 
        {
            addflag = 3;
        }


        ImGui::SliderFloat("X", &X, 0.0f, 1.0f);
        ImGui::SliderFloat("Y", &Y, 0.0f, 1.0f);
        ImGui::SliderFloat("Z", &Z, 0.0f, 1.0f);
    }
    else
    {
        if (ImGui::Button("delete")&& cgui != "GUI") 
        {
            deleted[s_row][s_col] = 1;
            cgui = "GUI";
        }
        ImGui::SliderFloat("R ambient", &ambientR[s_row][s_col], 0.0f, 1.0f);
        ImGui::SliderFloat("G ambient", &ambientG[s_row][s_col], 0.0f, 1.0f);
        ImGui::SliderFloat("B ambient", &ambientB[s_row][s_col], 0.0f, 1.0f);

        ImGui::SliderFloat("R diffuse", &diffuseR[s_row][s_col], 0.0f, 1.0f);
        ImGui::SliderFloat("G diffuse", &diffuseG[s_row][s_col], 0.0f, 1.0f);
        ImGui::SliderFloat("B diffuse", &diffuseB[s_row][s_col], 0.0f, 1.0f);

        ImGui::SliderFloat("R specular", &specularR[s_row][s_col], 0.0f, 1.0f);
        ImGui::SliderFloat("G specular", &specularG[s_row][s_col], 0.0f, 1.0f);
        ImGui::SliderFloat("B specular", &specularB[s_row][s_col], 0.0f, 1.0f);
    }
    
    ImGui::SetWindowSize(ImVec2(400, -1));
    ImGui::End();
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

#endif