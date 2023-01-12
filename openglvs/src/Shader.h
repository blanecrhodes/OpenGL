#pragma once
#include "glm/ext.hpp"

struct Shader{
    u32 ID;
};

static bool CheckShaderCompile(u32 shader, u32 type){
    s32 success;
    s32 logSize;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    char* infoLog = (char*)_malloca(logSize * sizeof(char)); //_malloca is windows only!
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(shader, logSize, 0, infoLog);
        switch(type){
            case GL_VERTEX_SHADER:{
                DebugPrint("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
            }
            break;
            case GL_FRAGMENT_SHADER:{
                DebugPrint("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
            }
            break;
        }
        DebugPrint(infoLog);
        DebugPrint("\n");
    }
    _freea(infoLog);
    return success;
}

bool CheckProgramLink(u32 program){
    s32 success;
    s32 logSize;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    char* infoLog = (char*)_malloca(logSize * sizeof(char));
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if(!success){
        glGetProgramInfoLog(program, logSize, 0, infoLog);
        DebugPrint("ERROR::SHADER::PROGRAM::LINK_FAILED\n");
        DebugPrint(infoLog);
        DebugPrint("\n");
    }
    _freea(infoLog);
    return success;
}

Shader MakeShader(char* vertexPath, char* fragmentPath){
    Shader result;
    ReadFileResult vertexShaderData = LoadFile(vertexPath);
    ReadFileResult fragmentShaderData = LoadFile(fragmentPath);

    const char* vertSrc = (const char*)vertexShaderData.contents;
    const char* fragSrc = (const char*)fragmentShaderData.contents;

    u32 vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertSrc, 0);
    glCompileShader(vertex);

#ifdef CRUMBLE_DEBUG 
    Assert(CheckShaderCompile(vertex, GL_VERTEX_SHADER));
#else
    CheckShaderCompile(vertex, GL_VERTEX_SHADER);
#endif

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragSrc, 0);
    glCompileShader(fragment);

#ifdef CRUMBLE_DEBUG 
    Assert(CheckShaderCompile(fragment, GL_FRAGMENT_SHADER));
#else
    CheckShaderCompile(fragment, GL_FRAGMENT_SHADER);
#endif

    result.ID = glCreateProgram();
    glAttachShader(result.ID, vertex);
    glAttachShader(result.ID, fragment);
    glLinkProgram(result.ID);

#ifdef CRUMBLE_DEBUG 
    Assert(CheckProgramLink(result.ID));
#else
    CheckProgramLink(result.ID);
#endif
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return result;
}

void UseShader(Shader shader) {
    glUseProgram(shader.ID);
}

void UseShader(u32 id) {
    glUseProgram(id);
}
    
void SetUniformBool(Shader shader, char* name, bool value) {
    s32 location = glGetUniformLocation(shader.ID, name);
    glUniform1i(location, (s32)value);
}

void SetUniformS32(Shader shader, char* name, s32 value) {
    s32 location = glGetUniformLocation(shader.ID, name);
    glUniform1i(location, value);
}

void SetUniformF32(Shader shader, char* name, f32 value) {
    s32 location = glGetUniformLocation(shader.ID, name);
    glUniform1f(location, value);
}

void SetUniformVec4f(Shader shader, char* name, Vec4 value) {
    s32 location = glGetUniformLocation(shader.ID, name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void SetUniformMat4f(Shader shader, char* name, glm::mat4 mat){
    u32 loc = glGetUniformLocation(shader.ID, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

