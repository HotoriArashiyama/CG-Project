#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <cmath>

/*TexLoad has been moved into LoadTexture.h*/

int ckpt() {
	static int cnt = 0;
	printf("CHECKPOINT%03d triggered.\n", cnt++);
	return 0;
}

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

class utilFloat
{
public:
	utilFloat(float val) : Val(val) {};
	~utilFloat() {};

	bool operator== (const utilFloat& f1) const {
		return std::abs(Val - f1.Val) < 0.00001f;
	}
	bool operator!= (const utilFloat& f1) const {
		return std::abs(Val - f1.Val) >= 0.00001f;
	}

private:
	float Val;
};

#endif
