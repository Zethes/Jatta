/* Jatta - General Utility Library
 * Copyright (C) 2012-2013, Joshua Brookover and Amber Thrall
 * All rights reserved.
 */

#pragma once
#include <Jatta/Config.h>
#include "Input.h"
#include "WindowStyle.h"
#include "../Math/Float2.h"
#include "../Math/Float4.h"
#if defined(JATTA_BUILD) && defined(MACOS)
#   include "MacOS/WindowDelegate.h"
#endif
#include "../External/Defines.h"

namespace Jatta
{
    class Window
    {
#       ifdef WINDOWS
        _JATTA_HWND handle;

        char className[20];

        _JATTA_HGDIOBJ backgroundBrush;

        static long __stdcall WindowProcedure(_JATTA_HWND handle, unsigned int message, unsigned int wParam, long int lParam);
#       endif

#       ifdef LINUX
        static std::map<_JATTA_WINDOW, Window*> windowMap;
        static _JATTA_DISPLAY display;
        _JATTA_WINDOW handle;

        static Boolean initialized;
#       endif

#       if defined(JATTA_BUILD) && defined(MACOS)
        WindowDelegate* handle;
#       elif defined(MACOS)
        void* handle;
#       endif

        Input input;

        _JATTA_EXPORT Window(const Window& copy);
#       ifdef CPP_HAS_MOVE_CONSTRUCTOR
        _JATTA_EXPORT Window(Window&& move);
#       endif
    public:
        _JATTA_EXPORT static void Update();

        _JATTA_EXPORT Window();
        _JATTA_EXPORT ~Window();

#       ifdef WINDOWS
        _JATTA_EXPORT _JATTA_HWND _GetHandle() const;
#       endif

#       ifdef LINUX
        _JATTA_EXPORT _JATTA_DISPLAY _GetDisplay() const;
        _JATTA_EXPORT _JATTA_WINDOW _GetHandle() const;
#       endif

#       if defined(JATTA_BUILD) && defined(MACOS)
        WindowDelegate* _GetHandle() const;
#       endif

        _JATTA_EXPORT Input* GetInput();

        _JATTA_EXPORT void Create(const WindowStyle& style);
        _JATTA_EXPORT void Close();

        _JATTA_EXPORT void UpdateInput();

        _JATTA_EXPORT void SetStyle(const WindowStyle& style);
        _JATTA_EXPORT WindowStyle GetStyle() const;

        _JATTA_EXPORT void SetTitle(const String& title);
        _JATTA_EXPORT String GetTitle() const;

        _JATTA_EXPORT void SetBackgroundColor(const Color& color);
        _JATTA_EXPORT Color GetBackgroundColor() const;

        _JATTA_EXPORT void SetWidth(UInt32 width);
        _JATTA_EXPORT UInt32 GetWidth() const;
        _JATTA_EXPORT void SetHeight(UInt32 height);
        _JATTA_EXPORT UInt32 GetHeight() const;
        _JATTA_EXPORT void SetSize(const Float2& size) const; // TODO: use something other than Float2
        _JATTA_EXPORT Float2 GetSize() const;

        _JATTA_EXPORT void SetResizable(Boolean resizable);
        _JATTA_EXPORT Boolean GetResizable() const;

        _JATTA_EXPORT Float4 GetFrameSize() const;

        _JATTA_EXPORT Boolean IsOpen() const;
        _JATTA_EXPORT Boolean IsFocused() const;
    };
}

#include "../External/Undefines.h"
