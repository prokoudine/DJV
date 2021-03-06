//------------------------------------------------------------------------------
// Copyright (c) 2004-2019 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvAV/GLFWSystem.h>

#include <djvAV/OpenGL.h>

#include <djvCore/Context.h>
#include <djvCore/CoreSystem.h>
#include <djvCore/LogSystem.h>
#include <djvCore/OS.h>
#include <djvCore/Vector.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace djv::Core;

namespace djv
{
    namespace AV
    {
        namespace GLFW
        {
            namespace
            {
                //! \todo Should this be configurable?
                const glm::ivec2 windowSize(1280, 720);
            
                //! \todo Should this be configurable?
                std::weak_ptr<LogSystem> logSystemWeak;

                void glfwErrorCallback(int error, const char * description)
                {
                    if (auto logSystem = logSystemWeak.lock())
                    {
                        logSystem->log("djv::CmdLine::GLFW::System", description);
                    }
                }

#if defined(DJV_OPENGL_ES2)
#else // DJV_OPENGL_ES2
                void APIENTRY glDebugOutput(
                    GLenum         source,
                    GLenum         type,
                    GLuint         id,
                    GLenum         severity,
                    GLsizei        length,
                    const GLchar * message,
                    const void *   userParam)
                {
                    switch (severity)
                    {
                    case GL_DEBUG_SEVERITY_HIGH_KHR:
                    case GL_DEBUG_SEVERITY_MEDIUM_KHR:
                        if (auto log = reinterpret_cast<const Context *>(userParam)->getSystemT<LogSystem>())
                        {
                            log->log("djv::CmdLine::GLFW::System", message);
                        }
                        break;
                    default: break;
                    }
                }
#endif // DJV_OPENGL_ES2

            } // namespace

            std::string getErrorMessage(ErrorString error)
            {
                std::stringstream ss;
                switch (error)
                {
                case ErrorString::Init:
                    ss << DJV_TEXT("Cannot initialize GLFW.");
                    break;
                case ErrorString::Window:
                    ss << DJV_TEXT("Cannot create GLFW window.");
                    break;
                case ErrorString::GLAD:
                    ss << DJV_TEXT("Cannot initialize GLAD.");
                    break;
                default: break;
                }
                return ss.str();
            }

            Error::Error(const std::string& what) :
                std::runtime_error(what)
            {}

            struct System::Private
            {
                GLFWwindow* glfwWindow = nullptr;
            };

            void System::_init(const std::shared_ptr<Context>& context)
            {
                ISystem::_init("djv::AV::GLFW::System", context);

                DJV_PRIVATE_PTR();

                addDependency(context->getSystemT<CoreSystem>());

                // Initialize GLFW.
                glfwSetErrorCallback(glfwErrorCallback);
                int glfwMajor    = 0;
                int glfwMinor    = 0;
                int glfwRevision = 0;
                glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
                {
                    std::stringstream ss;
                    ss << "GLFW version: " << glfwMajor << "." << glfwMinor << "." << glfwRevision;
                    _log(ss.str());
                }
                if (!glfwInit())
                {
                    throw Error(getErrorMessage(ErrorString::Init));
                }

                // Create a window.
#if defined(DJV_OPENGL_ES2)
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else // DJV_OPENGL_ES2
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif // DJV_OPENGL_ES2
                glfwWindowHint(GLFW_SAMPLES, 1);
                glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
                glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
                if (OS::getIntEnv("DJV_OPENGL_DEBUG") != 0)
                {
                    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
                }
                p.glfwWindow = glfwCreateWindow(
                    windowSize.x,
                    windowSize.y,
                    context->getName().c_str(), NULL, NULL);
                if (!p.glfwWindow)
                {
                    throw Error(AV::GLFW::getErrorMessage(ErrorString::Window));
                }
                {
                    int glMajor = glfwGetWindowAttrib(_p->glfwWindow, GLFW_CONTEXT_VERSION_MAJOR);
                    int glMinor = glfwGetWindowAttrib(_p->glfwWindow, GLFW_CONTEXT_VERSION_MINOR);
                    int glRevision = glfwGetWindowAttrib(_p->glfwWindow, GLFW_CONTEXT_REVISION);
                    std::stringstream ss;
                    ss << "OpenGL version: " << glMajor << "." << glMinor << "." << glRevision;
                    _log(ss.str());
                }
                glfwSetWindowUserPointer(p.glfwWindow, context.get());
                glfwMakeContextCurrent(p.glfwWindow);
#if defined(DJV_OPENGL_ES2)
                if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
#else
                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
#endif
                {
                    throw Error(AV::GLFW::getErrorMessage(ErrorString::GLAD));
                }
#if defined(DJV_OPENGL_ES2)
#else // DJV_OPENGL_ES2
                GLint flags = 0;
                glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
                if (flags & static_cast<GLint>(GL_CONTEXT_FLAG_DEBUG_BIT))
                {
                    glEnable(GL_DEBUG_OUTPUT);
                    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                    glDebugMessageCallback(glDebugOutput, context.get());
                    glDebugMessageControl(
                        static_cast<GLenum>(GL_DONT_CARE),
                        static_cast<GLenum>(GL_DONT_CARE),
                        static_cast<GLenum>(GL_DONT_CARE),
                        0,
                        nullptr,
                        GL_TRUE);
                }
#endif // DJV_OPENGL_ES2
                glfwSwapInterval(1);
            }

            System::System() :
                _p(new Private)
            {}

            System::~System()
            {
                DJV_PRIVATE_PTR();
                if (p.glfwWindow)
                {
                    glfwDestroyWindow(p.glfwWindow);
                    p.glfwWindow = nullptr;
                }
                glfwTerminate();
            }

            std::shared_ptr<System> System::create(const std::shared_ptr<Context>& context)
            {
                auto out = std::shared_ptr<System>(new System);
                out->_init(context);
                return out;
            }

            GLFWwindow* System::getGLFWWindow() const
            {
                return _p->glfwWindow;
            }

        } // namespace GLFW
    } // namespace AV
} // namespace djv

