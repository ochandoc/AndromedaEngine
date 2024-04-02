#pragma once

#include "GL/glew.h"

#define WAIT_GPU_LOAD()                                                                     \
{                                                                                           \
	GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);                              \
	bool loaded = false;                                                                      \
	while (!loaded)                                                                           \
	{                                                                                         \
		GLenum result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000));  \
		if (result != GL_TIMEOUT_EXPIRED) loaded = true;                                        \
	}                                                                                         \
	glDeleteSync(sync);                                                                       \
} 

class OpenGLSync
{
private:
    OpenGLSync() {}
public:
    ~OpenGLSync() {}

    static std::mutex& GetMutex() 
    { 
        static std::mutex Mutex;
        return Mutex;
    }
};