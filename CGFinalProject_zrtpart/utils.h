#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

/*TexLoad has been moved into LoadTexture.h*/

int ckpt() {
	static int cnt = 0;
	printf("CHECKPOINT%03d triggered.\n", cnt++);
	return 0;
}

#endif
