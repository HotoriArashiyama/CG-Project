#pragma once
#ifndef __SCREENSHOT_H__
#define __SCREENSHOT_H__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#pragma comment(lib, "glew32.lib")
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <stb_image/stb_image.h>
#pragma warning(disable : 4996)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

#include "utils.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

namespace ScreenShot_h {
	static unsigned char data[WINDOW_HEIGHT * WINDOW_WIDTH * 4 + 5];
	static char path[128] = "./ScreenShots/screenshot ";
	static char tarr[64];
}

class ScreenShotCapturer
{
public:
	ScreenShotCapturer() {};
	~ScreenShotCapturer() {};
	static void getScreenShot();

private:
	static FILE* fileHdl;
};

void ScreenShotCapturer::getScreenShot() {
	time_t t;
	time(&t);
	struct tm tinfo;
	localtime_s(&tinfo, &t);
	asctime_s(ScreenShot_h::tarr, &tinfo);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, ScreenShot_h::data);
	//std::cout << ScreenShot_h::tarr << std::endl;
	for (int i = 25, j = 0; i < 124; i++) {
		if (ScreenShot_h::tarr[j] >= 0x20) {
			if (ScreenShot_h::tarr[j] != ':')
				ScreenShot_h::path[i] = ScreenShot_h::tarr[j];
			else
				i--;
			j++;
			continue;
		}
		ScreenShot_h::path[i] = '.';
		ScreenShot_h::path[i + 1] = 'p';
		ScreenShot_h::path[i + 2] = 'n';
		ScreenShot_h::path[i + 3] = 'g';
		ScreenShot_h::path[i + 4] = 0x0;
		break;
	}
	//std::cout << ScreenShot_h::path << std::endl;
	if (!stbi_write_png(ScreenShot_h::path, WINDOW_WIDTH, WINDOW_HEIGHT, 3, ScreenShot_h::data, WINDOW_WIDTH * 3))
		printf("ScreenShot failed!\n");
	else
		printf("%s has been saved.\n", ScreenShot_h::path);
}

#undef PIC_WIDTH
#undef PIC_HEIGHT
#endif
