#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

struct Attribute {
	u32 index;
	s32 componentCount;
	u32 dataType;
	u32 stride;
	//u32 vertexArrayID????
};

enum BindingIndex {
	BINDING_0 = 1 << 0,
	BINDING_1 = 1 << 1,
	BINDING_2 = 1 << 2,
	BINDING_3 = 1 << 3,
	BINDING_4 = 1 << 4,
	BINDING_5 = 1 << 5,
	BINDING_6 = 1 << 6,
	BINDING_7 = 1 << 7,
	BINDING_8 = 1 << 8,
	BINDING_9 = 1 << 9,
	BINDING_10 = 1 << 10,
	BINDING_11 = 1 << 11,
	BINDING_12 = 1 << 12,
	BINDING_13 = 1 << 13,
	BINDING_14 = 1 << 14,
	BINDING_15 = 1 << 15
};

//should VA own the attributes or should they be on their own and just tied to a VA with an ID field
//or the VA holds and ID to an attribute and in both cases the attributes are just in an external array
struct VertexArray {
	u32 ID;
	u16 bindings;
};

u32 GetSizeOfType(u32 type) {
	switch (type) {
	case GL_FLOAT: return 4;
	case GL_UNSIGNED_INT: return 4;
	case GL_UNSIGNED_BYTE: return 1;
	}
	Assert(false);
	return 0;
}

bool IsBindingUsed(VertexArray va, BindingIndex index) {
	bool result = va.bindings & index;
	return result;
}

void AttachVboToVao(VertexArray va, VertexBuffer vbo, u32 binding) {
	glVertexArrayVertexBuffer(va.ID, binding, vbo.ID, 0, vbo.stride);
}

void AttachIboToVao(VertexArray va, IndexBuffer ibo) {
	glVertexArrayElementBuffer(va.ID, ibo.ID);
}

void AttachLayoutToVao(VertexArray va, VertexBufferLayout* layout, u32 bindingIndex) {
	u32 offset = 0;
	for (u32 i = 0; i < layout->elements.size(); i++) {
		VertexBufferElement currElement = layout->elements.at(i);
		glEnableVertexArrayAttrib(va.ID, i);
		glVertexArrayAttribFormat(va.ID, i, currElement.count, currElement.type,
			currElement.normalized, offset);
		glVertexArrayAttribBinding(va.ID, i, bindingIndex);
		offset += GetSizeOfType(currElement.type) * currElement.count;
	}
}

VertexArray MakeVertexArray() {
	VertexArray result;
	glCreateVertexArrays(1, &result.ID);
	return result;
}

VertexArray MakeVertexArray(VertexBuffer vbo, IndexBuffer ibo, u32 vboBinding) {
	VertexArray result;
	glCreateVertexArrays(1, &result.ID);
	AttachVboToVao(result, vbo, vboBinding);
	AttachIboToVao(result, ibo);
	return result;
}

