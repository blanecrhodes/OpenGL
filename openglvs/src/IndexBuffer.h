#pragma once

struct IndexBuffer {
	u32 ID;
	u32 indicesCount;
};

IndexBuffer MakeIndexBuffer(u32* data, u32 elementCount, u32 drawFlags) {
    IndexBuffer result;
    glCreateBuffers(1, &result.ID);
    glNamedBufferStorage(result.ID, elementCount, data, drawFlags);
    return result;
}

void DeleteIndexBuffer(IndexBuffer ibo) {
    glDeleteBuffers(1, &ibo.ID);
} 