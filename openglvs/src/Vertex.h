#pragma once

#include "crheader.h"

struct Vertex {
    Vec3 position;
    Vec3 colors;
    Vec2 texCoords;
};

void AddPosition(Vertex* v, f32 x, f32 y, f32 z) {
    v->position = { x, y, z };
}

void AddPosition(Vertex* v, f32 x, f32 y) {
    v->position = { x, y, 0 };
}

void AddColorRGB(Vertex* v, f32 r, f32 g, f32 b) {
    v->colors = { r, g, b };
}
void AddTextureCoords(Vertex* v, f32 x, f32 y) {
    v->texCoords = { x, y };
}

