/* Jatta - General Utility Library
 * Copyright (C) 2012-2013, Joshua Brookover and Amber Thrall
 * All rights reserved.
 */

#pragma once
#include <Jatta/Config.h>
#include "OpenGL.h"
#if defined(JATTA_BUILD) && defined(JATTA_MACOS)
#   import "MacOS/OpenGLView.h"
#endif
#include "../Windows/Window.h"
#include "../External/Defines.h"

/* OpenGL is an experimental namespace in Jatta! */
namespace Jatta
{
    namespace OpenGL
    {
        class Context
        {
            Window* window;

#           ifdef JATTA_WINDOWS
            _JATTA_PIXELFORMATDESCRIPTOR pfd;
            _JATTA_HDC deviceContext;
            UInt32 pixelFormat;
            _JATTA_HGLRC renderContext;
#           endif

#           ifdef JATTA_LINUX
            _JATTA_DISPLAY display;
            _JATTA_GLXCONTEXT context;
#           endif

#           if defined(JATTA_BUILD) && defined(JATTA_MACOS)
            OpenGLView* view;
#           elif defined(JATTA_MACOS)
            void* view;
#           endif
        public:
            _JATTA_EXPORT Context* GetCurrent();

            _JATTA_EXPORT Context();
            _JATTA_EXPORT ~Context();

            _JATTA_EXPORT void Create(Window* window);
            _JATTA_EXPORT void MakeCurrent();
            _JATTA_EXPORT void Destroy();
            _JATTA_EXPORT bool IsValid() const;

            _JATTA_EXPORT void Enable(Enum capability);
            _JATTA_EXPORT void Disable(Enum capability);

            _JATTA_EXPORT void DepthMask(bool enabled);
            _JATTA_EXPORT void StencilMask(bool enabled);
            _JATTA_EXPORT void StencilMask(UInt32 mask);

            _JATTA_EXPORT void StencilFunc(Enum func, SInt32 reference, UInt32 mask = ~0);
            _JATTA_EXPORT void StencilOp(Enum sfail, Enum dpfail, Enum dppass);

            _JATTA_EXPORT void Viewport(SInt32 x, SInt32 y, UInt32 width, UInt32 height);

            _JATTA_EXPORT void ClearColor(const Color& color);
            _JATTA_EXPORT void Clear(Enum mask);

            _JATTA_EXPORT void SwapBuffers();

            // TODO: finish importing opengl context functions
        };
    }
}
#include "../External/Undefines.h"
