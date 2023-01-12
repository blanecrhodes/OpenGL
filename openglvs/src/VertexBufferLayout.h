#pragma once

#include <GL/glew.h>
#include <vector>
#include "crheader.h"

struct VertexBufferElement {
	u32 type;
	u32 count;
	u32 normalized;
};

struct VertexBufferLayout {
	std::vector<VertexBufferElement> elements;
};

void LayoutPushFloat(VertexBufferLayout* layout, u32 count) {
	layout->elements.push_back({ GL_FLOAT, count, false });
}

void LayoutPushU32(VertexBufferLayout* layout, u32 count) {
	layout->elements.push_back({ GL_UNSIGNED_INT, count, false });
}

void LayoutPushByte(VertexBufferLayout* layout, u32 count) {
	layout->elements.push_back({ GL_UNSIGNED_BYTE, count, false });
}
