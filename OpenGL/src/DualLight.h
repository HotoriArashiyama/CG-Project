#pragma once

#ifndef __DUALLIGHT_H__
#define __DUALLIGHT_H__

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <assert.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "txts.h"

#define NR_POINT_LIGHTS 4

/*This file is mainly a integration of light part of jkx's code.*/

class DualLight
{
public:
    DualLight() {};
	DualLight(const char* vSrc, const char* fSrc);
	~DualLight();
    void Initialize(const char* vSrc, const char* fSrc) { Shdr.Initialize(vSrc, fSrc); };
    void Enlighten();
    void use() { glUseProgram(Shdr.getHdl()); }
    void glutEnlighten();
    void AddLight(PlayerAddedLight* lt) { LightList.push_back(lt); }
    Shader& getShdr() { return Shdr; }

    std::vector<PlayerAddedLight*> LightList;

private:
    Shader Shdr;
};

DualLight::DualLight(const char* vSrc, const char* fSrc)
{
    Shdr.Initialize(vSrc, fSrc);
    LightList.push_back(NULL);
}

DualLight::~DualLight()
{
}

void DualLight::glutEnlighten() {
    float pos[4] = { 0, 0, 0, 1.0f };
    float amb[4] = { 0, 0, 0, 1.0f };
    float dif[4] = { 0, 0, 0, 1.0f };
    float spe[4] = { 0, 0, 0, 1.0f };
    for (int i = 1, cnt = 1; i < LightList.size() && cnt < 8; i++) {
        if (LightList[i]->NotHidden() == false || LightList[i]->IsLit() == false) continue;
        glm::vec3 vecpos = LightList[i]->getTrans();
        glm::vec3 vecamb = LightList[i]->getltAmbi();
        glm::vec3 vecdif = LightList[i]->getltDiff();
        glm::vec3 vecspe = LightList[i]->getltSpec();
        pos[0] = vecpos[0], pos[1] = vecpos[1], pos[2] = vecpos[2];
        amb[0] = vecamb[0], amb[1] = vecamb[1], amb[2] = vecamb[2];
        dif[0] = vecdif[0], dif[1] = vecdif[1], dif[2] = vecdif[2];
        spe[0] = vecspe[0], spe[1] = vecspe[1], spe[2] = vecspe[2];
        glLightfv(GL_LIGHT0 + cnt, GL_POSITION, pos);
        glLightfv(GL_LIGHT0 + cnt, GL_AMBIENT, amb);
        glLightfv(GL_LIGHT0 + cnt, GL_DIFFUSE, dif);
        glLightfv(GL_LIGHT0 + cnt, GL_SPECULAR, spe);
        glEnable(GL_LIGHT0 + cnt);
        cnt++;
    }
}

void DualLight::Enlighten() {
    // 平行光源
    Shdr.glmSetVec3("dirLight.direction", glm::vec3(0.0f, -1.0f, -0.5f));
    Shdr.glmSetVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
    Shdr.glmSetVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    Shdr.glmSetVec3("dirLight.specular", glm::vec3(0.9f, 0.9f, 0.9f));
    // 点光源
    int cnt = 0;
    for (int i = 0; i < LightList.size() && cnt < NR_POINT_LIGHTS; i++) {
        if (LightList[i]->NotHidden() == false) continue;
        Shdr.glmSetVec3(std::string("pointLights[") + (char)('0' + cnt) + std::string("].position"), LightList[i]->getTrans());
        Shdr.glmSetVec3(std::string("pointLights[") + (char)('0' + cnt) + std::string("].ambient"), LightList[i]->getltAmbi());
        Shdr.glmSetVec3(std::string("pointLights[") + (char)('0' + cnt) + std::string("].diffuse"), LightList[i]->getltDiff());
        Shdr.glmSetVec3(std::string("pointLights[") + (char)('0' + cnt) + std::string("].specular"), LightList[i]->getltSpec());
        Shdr.glmSetFloat(std::string("pointLights[") + (char)('0' + cnt) + std::string("].constant"), LightList[i]->getCons());
        Shdr.glmSetFloat(std::string("pointLights[") + (char)('0' + cnt) + std::string("].linear"), LightList[i]->getLinr());
        Shdr.glmSetFloat(std::string("pointLights[") + (char)('0' + cnt) + std::string("].quadratic"), LightList[i]->getLinr());
        cnt++;
    }
    // 环境光
    Shdr.glmSetVec3("ambientLight", glm::vec3(0.1f, 0.1f, 0.1f));
    // 点光源个数
    Shdr.glmSetInt("pointLightsCount", cnt);
    // 材质光照属性
    Shdr.glmSetFloat("material.shininess", 32.0f);
}

namespace DualLight_h {
    unsigned int VBO, cubeVAO;
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    void InitLightCube() {
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(DualLight_h::vertices), DualLight_h::vertices, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);


        // 点光源的正方体模型的Cube
        unsigned int lightCubeVAO;

        glGenVertexArrays(1, &lightCubeVAO);
        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
}


#undef NR_POINT_LIGHTS
#endif
