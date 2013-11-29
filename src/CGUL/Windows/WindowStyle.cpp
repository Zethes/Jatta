// C++ General Utility Library (mailto:cgul@zethes.com)
// Copyright (C) 2012-2014, Joshua Brookover and Amber Thrall
// All rights reserved.

/** @file WindowStyle.cpp
 */

#include "WindowStyle.hpp"

/** @brief Default constructor.  Loads the window style with presets.
 *  @details Sets up the window style with the following presets:
 *  @code
 *  title = "CGUL Window";
 *  backgroundColor = Color(255, 255, 255);
 *  width = 640;
 *  height = 480;
 *  resizable = true;
 *  @endcode
 */
_CGUL_EXPORT CGUL::WindowStyle::WindowStyle() :
    title("CGUL Window"),
    backgroundColor(Color(255, 255, 255)),
    width(640),
    height(480),
    resizable(true)
{
}

/** @brief Copy constructor.
 *  @param style The style to copy.
 */
_CGUL_EXPORT CGUL::WindowStyle::WindowStyle(const WindowStyle& style) :
    title(style.title),
    backgroundColor(style.backgroundColor),
    width(style.width),
    height(style.height),
    resizable(style.resizable)
{
}

/** @brief Parameterized constructor.
 *  @param title The title of the window.
 *  @param backgroundColor The background color of the window.
 *  @param width The width of the window.
 *  @param height The height of the window.
 *  @param resizable Whether the window is resizable or not.
 *  @param minimizable Whether the minimize button is enabled or not.
 *  @param maximizable Whether the maximize button is enabled or not.
 *  @param closeable Whether the close button is enabled or not.
 */
_CGUL_EXPORT CGUL::WindowStyle::WindowStyle(const std::string& title, Color backgroundColor, UInt32 width, UInt32 height, Boolean resizable) :
    title(title),
    backgroundColor(backgroundColor),
    width(width),
    height(height),
    resizable(resizable)
{
}