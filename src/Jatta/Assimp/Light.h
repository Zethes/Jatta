/* Jatta - General Utility Library
 * Copyright (C) 2012-2013, Joshua Brookover and Amber Thrall
 * All rights reserved.
 */

#pragma once
#include <Jatta/Config.h>
#include "../Utility/String.h"
#include "../Math/Float3.h"
#include "../Images/Color.h"
#include "../External/Defines.h"

/* Assimp is an experimental namespace in Jatta! */
namespace Jatta
{
    namespace Assimp
    {
        enum LightTypes
        {
            UNDEFINED = 0x0,
            DIRECTIONAL = 0x1,
            POINT = 0x2,
            SPOT = 0x3
        };

        class Light
        {
            const _JATTA_AILIGHT light;
        public:
            _JATTA_EXPORT Light(_JATTA_AILIGHT light);

            _JATTA_EXPORT String GetName();

            _JATTA_EXPORT UInt32 GetType();

            _JATTA_EXPORT Float3 GetPosition();
            _JATTA_EXPORT Float3 GetDirection();
            _JATTA_EXPORT Float32 GetAngleInnerCone();
            _JATTA_EXPORT Float32 GetAngleOuterCone();

            _JATTA_EXPORT Float32 GetAttenuationConstant();
            _JATTA_EXPORT Float32 GetAttenuationLinear();
            _JATTA_EXPORT Float32 GetAttenuationQuadratic();

            _JATTA_EXPORT Color GetColorDiffuse();
            _JATTA_EXPORT Color GetColorSpecular();
            _JATTA_EXPORT Color GetColorAmbient();
        };
    }
}

#include "../External/Undefines.h"