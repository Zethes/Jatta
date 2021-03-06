// C++ General Utility Library (mailto:cgul@zethes.com)
// Copyright (C) 2012-2014, Joshua Brookover and Amber Thrall
// All rights reserved.

/** @file MaterialProperty.hpp
 */

#pragma once
#include <CGUL/Config.hpp>
#include "../Utility/String.hpp"
#include "../Assimp/Assimp.hpp"
#include "../External/Defines.hpp"

namespace CGUL
{
    namespace Assimp
    {
        class MaterialProperty
        {
            _CGUL_AIMATERIALPROPERTY matProp;
        public:
            _CGUL_EXPORT MaterialProperty(_CGUL_AIMATERIALPROPERTY matProp);

            _CGUL_EXPORT String GetKey();
            _CGUL_EXPORT UInt32 GetSemantic();
            _CGUL_EXPORT UInt32 GetIndex();

            _CGUL_EXPORT UInt32 GetDataLength();
            _CGUL_EXPORT UInt32 GetType();
            _CGUL_EXPORT char* GetData();
        };
    }
}

#include "../External/Undefines.hpp"
