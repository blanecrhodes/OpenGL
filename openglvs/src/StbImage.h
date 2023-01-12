#pragma once

#include "crheader.h"

struct StbImage {
	s32 width;
	s32 height;
	s32 nrChannels;
	void* data;
};

StbImage LoadImage(char* filePath) {
    StbImage result;
    stbi_set_flip_vertically_on_load(true);
    result.data = stbi_load(filePath, &result.width, &result.height, &result.nrChannels, 0);
	if (!result.data)
		printf("Couldn't load texture");
	return result;
}
