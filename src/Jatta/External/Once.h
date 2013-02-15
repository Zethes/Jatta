/* Jatta - General Utility Library
 * Copyright (C) 2012-2013, Joshua Brookover and Amber Thrall
 * All rights reserved.
 */

#pragma once

// Operating System Detection
#if !defined(WINDOWS) & !defined(LINUX) & !defined(MACOS)
#   if defined(_WIN32)
#       define WINDOWS
#   elif defined(__APPLE__)
#       define MACOS
#   elif defined(__GNUC__)
#       define LINUX
#   else
#       error Failed to automatically detected operating system.  Must define WINDOWS, LINUX, or MACOS on the command line.
#   endif
#endif

#ifdef WINDOWS
namespace Jatta
{
    struct _JATTA_PIXELFORMATDESCRIPTOR
    {
        char padding[40];
    };
}
#endif

// TODO: clean this up (make a more logical way of determining if std::move exists)
// Clang doesn't support this...
#ifdef MACOS
namespace std
{
    template <typename T> const T& move(const T& value)
    {
        return value;
    }
    template <typename T> T& move(T& value)
    {
        return value;
    }
}
#endif

// TODO: fix static assert on clang builds
// Jatta Type Defines
#define TYPE(oldType, newType, size) typedef oldType newType; static_assert(sizeof(oldType) == size, #oldType " must be a size of " #size ". Please change Jatta.h to fix this error.");
/** @brief General utility library.
 */
namespace Jatta
{
    /** @brief A 32 bit (4 bytes) floating point number.
     */
    TYPE(float, Float32, 4);

    /** @brief A 64 bit (8 bytes) floating point number.
     */
    TYPE(double, Float64, 8);

    /** @brief An 8 bit (1 byte) signed integer number.
     */
    TYPE(char, SInt8, 1);

    /** @brief An 8 bit (1 byte) unsigned integer number.
     */
    TYPE(unsigned char, UInt8, 1);

    /** @brief A 16 bit (2 bytes) signed integer number.
     */
    TYPE(short, SInt16, 2);

    /** @brief A 16 bit (2 bytes) unsigned integer number.
     */
    TYPE(unsigned short, UInt16, 2);

    /** @brief A 32 bit (4 bytes) signed integer number.
     */
    TYPE(int, SInt32, 4);

    /** @brief A 32 bit (4 bytes) unsigned integer number.
     */
    TYPE(unsigned int, UInt32, 4);

    /** @brief A 64 bit (8 bytes) signed integer number.
     */
    TYPE(long long, SInt64, 8);

    /** @brief A 64 bit (8 bytes) unsigned integer number.
     */
    TYPE(unsigned long long, UInt64, 8);

    /** @brief A 8 bit (1 byte) boolean value (true or false).
     */
    TYPE(bool, Boolean, 1);

    /** @brief Alias for UInt8, an unsigned byte.
     */
    TYPE(UInt8, Byte, 1);

    /** @brief Alias for SInt8, a signed byte.
     */
    TYPE(SInt8, SignedByte, 1);

    /** @brief A system-dependent sized integer capable of holding addresses in memory.
     */
    TYPE(unsigned long, Size, sizeof(void*));
}
#undef TYPE

// TODO: clean up the standard template library list
// Standard Template Library Includes
#include <algorithm>
//#include <array>
#include <cctype>
//#include <forward_list>
#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

// DLL Exports
#ifdef MSVC
#   ifdef JATTA_BUILD
#       define _JATTA_EXPORT __declspec(dllexport) 
#   else
#       define _JATTA_EXPORT __declspec(dllimport)
#   endif
#else
#   define _JATTA_EXPORT
#endif
