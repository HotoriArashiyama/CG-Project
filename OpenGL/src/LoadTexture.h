#pragma once
#ifndef __LOADTEXTURE_H__
#define __LOADTEXTURE_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>

// Reference: lab5
#define BITMAP_ID 0x4D42
#define imageweight 128
#define imageheight 128
GLubyte image[imageheight][imageweight][3];

// Reference: lab5
static unsigned char* LoadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapInfoHeader)
{
	FILE* filePtr;	// 文件指针
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
	unsigned char* bitmapImage;		// bitmap图像数据
	int	imageIdx = 0;		// 图像位置索引
	unsigned char	tempRGB;	// 交换变量

	// 以“二进制+读”模式打开文件filename 
	fopen_s(&filePtr, filename, "rb");
	if (filePtr == NULL) return NULL;
	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// 关闭bitmap图像文件
	fclose(filePtr);
	return bitmapImage;
}

// Reference: lab5
static void TexLoadBmp(int i, const char* filename, unsigned int *Texs = NULL)
{
	assert(Texs != NULL);

	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap信息头
	unsigned char* bitmapData;                                       // 纹理数据

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, Texs[i]);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap层次(通常为0，表示最上层) 
		GL_RGB,	//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,	//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapData);	//bitmap数据指针  
}

static void TexLoad(const char* filename, unsigned int* TexHdl = NULL, GLint wrap = GL_CLAMP_TO_EDGE, GLint MinFil = GL_LINEAR, GLint MagFil = GL_LINEAR, bool flip = false)
{
	assert(TexHdl != NULL);

	int w, h, c;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(filename, &w, &h, &c, 0);
	assert(data != NULL);

	GLenum format = c == 1 ? (GL_RED) : (c == 3 ? GL_RGB : GL_RGBA);

	glBindTexture(GL_TEXTURE_2D, *TexHdl);
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFil);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFil);

	stbi_image_free(data);
}

static void TexLoadCubeBmp(unsigned int* TexDesc, std::string* s) {
	assert((TexDesc != NULL));

	unsigned char* bmpData;
	BITMAPINFOHEADER bmpHdr;
	for (int i = 0; i < 6; i++) {
		bmpData = LoadBitmapFile(s[i].c_str(), &bmpHdr);
		glBindTexture(GL_TEXTURE_2D, TexDesc[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmpHdr.biWidth, bmpHdr.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bmpData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

#endif