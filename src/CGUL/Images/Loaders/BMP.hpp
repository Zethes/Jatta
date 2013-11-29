// C++ General Utility Library (mailto:cgul@zethes.com)
// Copyright (C) 2012-2014, Joshua Brookover and Amber Thrall
// All rights reserved.

/** @file BMP.hpp
 */

#pragma once
#include <CGUL/Config.hpp>
#include "../../Utility/String.hpp"
#include "../Image.hpp"
#include "../../External/Defines.hpp"

namespace CGUL
{
    /** @brief Namespace containing every builtin image loader.
    */
    namespace ImageLoaders
    {
        namespace BMPCompressionMethods
        {
            enum
            {
                RGB = 0,
                RLE8,
                RLE4,
                BITFIELDS,
                JPEG,
                PNG,
                ALPHABITFIELDS
            };
        }

        struct BITMAPHEADER
        {
        public:
            short MagicNumber;
            UInt32 FileSize;
            short Reserved1;
            short Reserved2;
            UInt32 Offset;
        };

        struct BMPXYZ
        {
            long X;
            long Y;
            long Z;
        };

        struct BMPXYZTRIPLE
        {
            BMPXYZ Red;
            BMPXYZ Green;
            BMPXYZ Blue;
        };

        struct BITMAPDIBHEADER
        {
        public:
            SInt32 Length;
            SInt32 Width;
            SInt32 Height;
            short Planes;
            short BitCount;
            UInt32 Compression;
            UInt32 ImageSize;
            SInt32 XPelsPerMeter;
            SInt32 YPelsPerMeter;
            UInt32 Used;
            UInt32 Important;
            //BITMAPCOREHEADER2 SPECIAL
            short ResUnit;
            short Reserved2;
            short Orientation;
            short Halftoning;
            UInt32 HalftoneSize1;
            UInt32 HalftoneSize2;
            UInt32 ColorSpace;
            UInt32 AppData;
            //END OF BITMAPCOREHEADER2
            UInt32 RedMask;
            UInt32 GreenMask;
            UInt32 BlueMask;
            UInt32 AlphaMask;
            UInt32 CSType;
            BMPXYZTRIPLE Endpoints;
            UInt32 GammaRed;
            UInt32 GammaGreen;
            UInt32 GammaBlue;
            UInt32 Intent;
            UInt32 ProfileData;
            UInt32 ProfileSize;
            UInt32 Reserved;
        };

        /** @brief Class inherited from ImageLoader to support BMP file formats.
         */
        class BMP
        {
            _CGUL_EXPORT static BITMAPHEADER ReadHeader(Byte* data, UInt32 size);
            _CGUL_EXPORT static BITMAPDIBHEADER ReadDIBHeader(Byte* data, UInt32 size);
        public:
            _CGUL_EXPORT static bool CanLoad(const String& filename);
            _CGUL_EXPORT static CGUL::Image* Load(const String& filename);
            _CGUL_EXPORT static void Save(const String& filename, CGUL::Image* image);

            _CGUL_EXPORT static BITMAPHEADER GetHeader(const String& filename);
            _CGUL_EXPORT static BITMAPDIBHEADER GetDIBHeader(const String& filename);
        };
    }
}

#include "../../External/Undefines.hpp"