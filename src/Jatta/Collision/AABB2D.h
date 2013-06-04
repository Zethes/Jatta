/* Jatta - General Utility Library
 * Copyright (C) 2012-2013, Joshua Brookover and Amber Thrall
 * All rights reserved.
 */

#pragma once
#include <Jatta/Config.h>
#include "../Math/Vector2.h"
#include "../External/Defines.h"

namespace Jatta
{
    namespace Collision
    {
        /** @brief 2D Axis Aligned Bounding Box
         */
        struct AABB2D
        {
            // TODO: this class
            Vector2 position;
            Vector2 extents;
            
            _JATTA_EXPORT AABB2D();
        };
    }
}

#include "../External/Undefines.h"
