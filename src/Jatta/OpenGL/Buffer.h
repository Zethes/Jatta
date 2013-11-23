/* Jatta - General Utility Library
 * Copyright (C) 2012-2013, Joshua Brookover and Amber Thrall
 * All rights reserved.
 */

#pragma once
#include <Jatta/Config.h>
#include "OpenGL.h"
#include "../External/Defines.h"

/* OpenGL is an experimental namespace in Jatta! */
namespace Jatta
{
    namespace OpenGL
    {
        class Buffer
        {
            UInt32 buffer;
            Enum type;
        public:
            DEPRECATED(_JATTA_EXPORT Buffer(), "Use Raw GL wrappers instead.");

            _JATTA_EXPORT void Create(Enum type);
            _JATTA_EXPORT void Delete();
            _JATTA_EXPORT UInt32 GetID();

            _JATTA_EXPORT void Bind();
            _JATTA_EXPORT void Unbind();

            _JATTA_EXPORT void Data(Size size, void* data, Enum usage);

            // TODO: finish importing opengl buffer functions
        };
    }
}
#include "../External/Undefines.h"
