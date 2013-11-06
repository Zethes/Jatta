/* Jatta - General Utility Library
 * Copyright (C) 2012-2013, Joshua Brookover and Amber Thrall
 * All rights reserved.
 */

#include "Mipmap.h"

_JATTA_EXPORT bool Jatta::Mipmap::IsValid()
{
    return (width != 0 && height != 0);
}

_JATTA_EXPORT Jatta::Mipmap::Mipmap()
{
    data = NULL;
    format = ImageFormats::NONE;
    width = 0;
    height = 0;
}

_JATTA_EXPORT Jatta::Mipmap::Mipmap(ImageFormat format, UInt32 width, UInt32 height, void* data)
{
    this->format = format;
    this->width = width;
    this->height = height;
    this->data = data;
}

_JATTA_EXPORT Jatta::Mipmap::~Mipmap()
{
    //Free();
}

_JATTA_EXPORT void Jatta::Mipmap::Free()
{
    free(data);
    data = NULL;
    width = 0;
    height = 0;
    format = ImageFormats::NONE;
}

_JATTA_EXPORT const void* Jatta::Mipmap::GetData()
{
    return data;
}
_JATTA_EXPORT Jatta::ImageFormat Jatta::Mipmap::GetFormat() const
{
    return format;
}