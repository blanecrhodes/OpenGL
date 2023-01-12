#pragma once

struct VertexBuffer {
	u32 ID;
    u32 offset;
    u32 stride;
};

VertexBuffer MakeVertexBuffer(void* data, u32 dataSize, u32 stride, u32 drawFlags, u32 offset = 0){
    VertexBuffer result;
    glCreateBuffers(1, &result.ID);
    glNamedBufferStorage(result.ID, dataSize, data, drawFlags);
    result.stride = stride;
    result.offset = offset;
    return result;
}

void DeleteVertexBuffer(VertexBuffer vbo) {
    glDeleteBuffers(1, &vbo.ID);
}
