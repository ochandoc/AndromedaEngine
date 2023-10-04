#pragma once

namespace And{
namespace Key{
  
  enum KeyCode{
    // From glfw3.h
    Space               = 1,
    Apostrophe          = 2,  /* ' */
    Comma               = 3,  /* , */
    Minus               = 4,  /* - */
    Period              = 5,  /* . */
    Slash               = 6,  /* / */

    D0                  = 7,  /* 0 */
    D1                  = 8,  /* 1 */
    D2                  = 9,  /* 2 */
    D3                  = 10, /* 3 */
    D4                  = 11, /* 4 */
    D5                  = 12, /* 5 */
    D6                  = 13, /* 6 */
    D7                  = 14, /* 7 */
    D8                  = 15, /* 8 */
    D9                  = 16, /* 9 */

    Semicolon           = 17, /* ; */
    Equal               = 18, /* = */

    A                   = 19,
    B                   = 20,
    C                   = 21,
    D                   = 22,
    E                   = 23,
    F                   = 24,
    G                   = 25,
    H                   = 26,
    I                   = 27,
    J                   = 28,
    K                   = 29,
    L                   = 30,
    M                   = 31,
    N                   = 32,
    O                   = 33,
    P                   = 34,
    Q                   = 35,
    R                   = 36,
    S                   = 37,
    T                   = 38,
    U                   = 39,
    V                   = 40,
    W                   = 41,
    X                   = 42,
    Y                   = 43,
    Z                   = 44,

    LeftBracket         = 45,  /* [ */
    Backslash           = 46,  /* \ */
    RightBracket        = 47,  /* ] */
    GraveAccent         = 48,  /* ` */

    World1              = 49,  /* non-US #1 */
    World2              = 50,  /* non-US #2 */

    /* Function keys */
    Escape              = 51,
    Enter               = 52,
    Tab                 = 53,
    Backspace           = 54,
    Insert              = 55,
    Delete              = 56,
    Right               = 57,
    Left                = 58,
    Down                = 59,
    Up                  = 60,
    PageUp              = 61,
    PageDown            = 62,
    Home                = 63,
    End                 = 64,
    CapsLock            = 65,
    ScrollLock          = 66,
    NumLock             = 67,
    PrintScreen         = 68,
    Pause               = 69,
    F1                  = 70,
    F2                  = 71,
    F3                  = 72,
    F4                  = 73,
    F5                  = 74,
    F6                  = 75,
    F7                  = 76,
    F8                  = 77,
    F9                  = 78,
    F10                 = 79,
    F11                 = 80,
    F12                 = 81,
    F13                 = 82,
    F14                 = 83,
    F15                 = 84,
    F16                 = 85,
    F17                 = 86,
    F18                 = 87,
    F19                 = 88,
    F20                 = 89,
    F21                 = 90,
    F22                 = 91,
    F23                 = 92,
    F24                 = 93,
    F25                 = 94,

    /* Keypad */
    KP0                 = 95,
    KP1                 = 96,
    KP2                 = 97,
    KP3                 = 98,
    KP4                 = 99,
    KP5                 = 100,
    KP6                 = 101,
    KP7                 = 102,
    KP8                 = 103,
    KP9                 = 104,
    KPDecimal           = 105,
    KPDivide            = 106,
    KPMultiply          = 107,
    KPSubtract          = 108,
    KPAdd               = 109,
    KPEnter             = 110,
    KPEqual             = 111,

    LeftShift           = 112,
    LeftControl         = 113,
    LeftAlt             = 114,
    LeftSuper           = 115,
    RightShift          = 116,
    RightControl        = 117,
    RightAlt            = 118,
    RightSuper          = 119,
    Menu                = 120
  };
	
  enum MouseCode{
    MouseLeft           = 121,
    MouseRight          = 122
  };

};
};