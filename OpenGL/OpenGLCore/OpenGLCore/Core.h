#pragma once

#include <Core/Log/Log.h>

#include <glad/glad.h>

const char* getErrorString(GLenum errorCode);
#ifdef NDEBUG
#define GL_CALL(glFunc) glFunc;
#else
#define GL_CALL(glFunc) glFunc; { GLenum e = glGetError(); if (e != GL_NO_ERROR) { LOG_ERROR("GL_CALL failed {0}", getErrorString(e)); } }
#endif