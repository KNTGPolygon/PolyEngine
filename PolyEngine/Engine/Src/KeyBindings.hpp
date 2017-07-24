#pragma once

#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>

namespace Poly
{
	enum class eKey
	{
		UNDEFINED =		0x00,
		MLBUTTON =		VK_LBUTTON,
		MRBUTTON =		VK_RBUTTON,
		CANCEL =		VK_CANCEL,
		MMBUTTON =		VK_MBUTTON,
		MBUTTON1 =		VK_XBUTTON1,
		MBUTTON2 =		VK_XBUTTON2,
		BACKSPACE =		VK_BACK,
		TAB =			VK_TAB,
		CLEAR =			VK_CLEAR,
		ENTER = 		VK_RETURN,
		SHIFT =			VK_SHIFT,
		CTRL =			VK_CONTROL,
		ALT = 			VK_MENU,
		PAUSE =			VK_PAUSE,
		CAPS_LOCK =		VK_CAPITAL,
		IME_KANA =		VK_KANA,
		IME_HANGUL =	VK_HANGUL,
		IME_JUNJA =		VK_JUNJA,
		IME_FINAL =		VK_FINAL,
		IME_HANJE =		VK_HANJA,
		IME_KENJI =		VK_KANJI,
		ESCAPE =		VK_ESCAPE,
		IME_CONVERT =	VK_CONVERT,
		IME_NONCONVERT= VK_NONCONVERT,
		IME_ACCEPT =	VK_ACCEPT,
		IME_MODECHANGE= VK_MODECHANGE,
		SPACE =			VK_SPACE,
		PAGE_UP =		VK_PRIOR,
		PAGE_DOWN =		VK_NEXT,
		END =			VK_END,
		HOME =			VK_HOME,
		LEFT =			VK_LEFT,
		UP =			VK_UP,
		RIGHT =			VK_RIGHT,
		DOWN =			VK_DOWN,
		SELECT =		VK_SELECT,
		PRINT =			VK_PRINT,
		EXECUTE =		VK_EXECUTE,
		SNAPSHOT =		VK_SNAPSHOT,
		INS =			VK_INSERT,
		DEL =			VK_DELETE,
		HELP =			VK_HELP,
		KEY_0 =			0x30,
		KEY_1 =			0x31,
		KEY_2 =			0x32,
		KEY_3 =			0x33,
		KEY_4 =			0x34,
		KEY_5 = 		0x35,
		KEY_6 =			0x36,
		KEY_7 =			0x37,
		KEY_8 =			0x38,
		KEY_9 =			0x39,
		KEY_A =			0x41,
		KEY_B =			0x42,
		KEY_C =			0x43,
		KEY_D =			0x44,
		KEY_E =			0x45,
		KEY_F =			0x46,
		KEY_G =			0x47,
		KEY_H =			0x48,
		KEY_I =			0x49,
		KEY_J =			0x4A,
		KEY_K =			0x4B,
		KEY_L =			0x4C,
		KEY_M =			0x4D,
		KEY_N =			0x4E,
		KEY_O =			0x4F,
		KEY_P =			0x50,
		KEY_Q =			0x51,
		KEY_R =			0x52,
		KEY_S =			0x53,
		KEY_T =			0x54,
		KEY_U =			0x55,
		KEY_V =			0x56,
		KEY_W =			0x57,
		KEY_X =			0x58,
		KEY_Y =			0x59,
		KEY_Z =			0x5A,
		LWIN =			VK_LWIN,
		RWIN =			VK_RWIN,
		APPS =			VK_APPS,
		SLEEP =			VK_SLEEP,
		NUM_0 =			VK_NUMPAD0,
		NUM_1 =			VK_NUMPAD1,
		NUM_2 = 		VK_NUMPAD2,
		NUM_3 =			VK_NUMPAD3,
		NUM_4 = 		VK_NUMPAD4,
		NUM_5 = 		VK_NUMPAD5,
		NUM_6 =			VK_NUMPAD6,
		NUM_7 = 		VK_NUMPAD7,
		NUM_8 = 		VK_NUMPAD8,
		NUM_9 = 		VK_NUMPAD9,
		NUM_MULTIPLY = 	VK_MULTIPLY,
		NUM_ADD =		VK_ADD,
		NUM_SEPARATOR = VK_SEPARATOR,
		NUM_SUBTRACT =	VK_SUBTRACT,
		NUM_DECIMAL = 	VK_DECIMAL,
		NUM_DIVIDE =	VK_DIVIDE,
		F1 =			VK_F1,
		F2 =			VK_F2,
		F3 =			VK_F3,
		F4 =			VK_F4,
		F5 =			VK_F5,
		F6 =			VK_F6,
		F7 =			VK_F7,
		F8 =			VK_F8,
		F9 =			VK_F9,
		F10 =			VK_F10,
		F11 =			VK_F11,
		F12 =			VK_F12,
		F13 =			VK_F13,
		F14 =			VK_F14,
		F15 =			VK_F15,
		F16 =			VK_F16,
		F17 =			VK_F17,
		F18 =			VK_F18,
		F19 =			VK_F19,
		F20 =			VK_F20,
		F21 =			VK_F21,
		F22 =			VK_F22,
		F23 =			VK_F23,
		F24 =			VK_F24,
		NUM_LOCK = 		VK_NUMLOCK,
		SCROLL_LOCK =	VK_SCROLL,
		LSHIFT = 		VK_LSHIFT,
		RSHIFT = 		VK_RSHIFT,
		LCTRL = 		VK_LCONTROL,
		RCTRL =			VK_RCONTROL,
		LMENU = 		VK_LMENU,
		RMENU = 		VK_RMENU,
		BROWSER_BACK =			VK_BROWSER_BACK,
		BROWSER_FORWARD = 		VK_BROWSER_FORWARD,
		BROWSER_REFRESH = 		VK_BROWSER_REFRESH,
		BROWSER_STOP =			VK_BROWSER_STOP,
		BROWSER_SEARCH = 		VK_BROWSER_SEARCH,
		BROWSER_FAVORITES = 	VK_BROWSER_FAVORITES,
		BROWSER_HOME =			VK_BROWSER_HOME,
		VOLUME_MUTE =			VK_VOLUME_MUTE,
		VOLUME_DOWN =			VK_VOLUME_DOWN,
		VOLUME_UP =				VK_VOLUME_UP,
		MEDIA_NEXT =			VK_MEDIA_NEXT_TRACK,
		MEDIA_PREV =			VK_MEDIA_PREV_TRACK,
		MEDIA_STOP =			VK_MEDIA_STOP,
		MEDIA_PLAY_PAUSE = 		VK_MEDIA_PLAY_PAUSE,
		LAUNCH_MAIL =			VK_LAUNCH_MAIL,
		LAUNCH_MEDIA_SELECT =	VK_LAUNCH_MEDIA_SELECT,
		LAUNCH_APP1 =			VK_LAUNCH_APP1,
		LAUNCH_AP2 =			VK_LAUNCH_APP2,
		SEMICOLON_COLON =		VK_OEM_1,					// Windows 2000: for the US standard keyboard, the ';:' key
		PLUS =					VK_OEM_PLUS,				// Windows 2000: for any country/region, the '+' key
		COMMA =					VK_OEM_COMMA,				// Windows 2000: for any country/region, the ',' key
		MINUS =					VK_OEM_MINUS,				// Windows 2000: for any country/region, the '-' key
		PERIOD =				VK_OEM_PERIOD,				// Windows 2000: for any country/region, the '.' key
		SLASH_QUESTION =		VK_OEM_2,					// Windows 2000: for the US standard keyboard, the '/?' key
		TILDE =					VK_OEM_3,					// Windows 2000: for the US standard keyboard, the '`~' key
		OPEN_SQUARE_CURLY =		VK_OEM_4,					// Windows 2000: for the US standard keyboard, the '[{' key
		BACKSLASH_VERTICAL_BAR= VK_OEM_5,					// Windows 2000: for the US standard keyboard, the '\|' key
		CLOSE_SQUARE_CURLY =	VK_OEM_6,					// Windows 2000: for the US standard keyboard, the ']}' key
		QUOTE_SINGLE_DOUBLE =	VK_OEM_7,					// Windows 2000: for the US standard keyboard, the 'single-quote/double-quote' key
		OEM_8 =					VK_OEM_8,
		OEM_102	=				VK_OEM_102,					// Windows 2000: either the '<>' key or the '\|' key on the RT 102-key keyboard
		IME_PROCESS =			VK_PROCESSKEY,
		PACKET =				VK_PACKET,
		ATTN =					VK_ATTN,
		CRSEL =					VK_CRSEL,
		EXSEL =					VK_EXSEL,
		EREOF =					VK_EREOF,
		PLAY =					VK_PLAY,
		ZOOM =					VK_ZOOM,
		PA1 =					VK_PA1,
		OEM_CLEAR =				VK_OEM_CLEAR,
		_COUNT =				0xff
	};
}
#elif defined(__linux__)
namespace Poly
{
	enum class eKey
	{
		UNDEFINED              = 0x00,
		MLBUTTON               = 0x01,
		MRBUTTON               = 0x03,
		CANCEL                 = UNDEFINED,
		MMBUTTON               = 0x02,
		MBUTTON1               = 0x06,
		MBUTTON2               = 0x07,
		BACKSPACE              = 0x16,
		TAB                    = 0x17,
		CLEAR                  = UNDEFINED,
		ENTER                  = 0x24, //Return key, not the NumPad Enter
		SHIFT                  = UNDEFINED, //FIXME(vuko): no such key on X11
		CTRL                   = UNDEFINED, //FIXME(vuko): same here
		ALT                    = UNDEFINED, //FIXME(vuko): this makes me sad :(
		PAUSE                  = 0x7F,
		CAPS_LOCK              = 0x42,
		IME_KANA               = 0x65, //FIXME(vuko): rip the asian linux market
		IME_HANGUL             = 0x82,
		IME_JUNJA              = UNDEFINED,
		IME_FINAL              = UNDEFINED,
		IME_HANJE              = 0x83,
		IME_KENJI              = UNDEFINED,
		ESCAPE                 = 0x09,
		IME_CONVERT            = UNDEFINED, //FIXME(vuko): don't have a CJK keyboard, sorry
		IME_NONCONVERT         = UNDEFINED,
		IME_ACCEPT             = UNDEFINED,
		IME_MODECHANGE         = 0xCB,
		SPACE                  = 0x41,
		PAGE_UP                = 0x70,
		PAGE_DOWN              = 0x75,
		END                    = 0x73,
		HOME                   = 0x6E,
		LEFT                   = 0x71,
		UP                     = 0x6F,
		RIGHT                  = 0x72,
		DOWN                   = 0x74,
		SELECT                 = UNDEFINED, //wtf is this, even
		PRINT                  = UNDEFINED,
		EXECUTE                = UNDEFINED, //???
		SNAPSHOT               = 0x6B, //that's Print Screen/SysRq, yo
		INS                    = 0x76,
		DEL                    = 0x77,
		HELP                   = UNDEFINED, //help!
		KEY_0                  = 0x13,
		KEY_1                  = 0x0A,
		KEY_2                  = 0x0B,
		KEY_3                  = 0x0C,
		KEY_4                  = 0x0D,
		KEY_5                  = 0x0E,
		KEY_6                  = 0x0F,
		KEY_7                  = 0x10,
		KEY_8                  = 0x11,
		KEY_9                  = 0x12,
		KEY_A                  = 0x26,
		KEY_B                  = 0x38,
		KEY_C                  = 0x36,
		KEY_D                  = 0x28,
		KEY_E                  = 0x1A,
		KEY_F                  = 0x29,
		KEY_G                  = 0x2A,
		KEY_H                  = 0x2B,
		KEY_I                  = 0x1F,
		KEY_J                  = 0x2C,
		KEY_K                  = 0x2D,
		KEY_L                  = 0x2E,
		KEY_M                  = 0x3A,
		KEY_N                  = 0x39,
		KEY_O                  = 0x20,
		KEY_P                  = 0x21,
		KEY_Q                  = 0x18,
		KEY_R                  = 0x1B,
		KEY_S                  = 0x27,
		KEY_T                  = 0x1C,
		KEY_U                  = 0x1E,
		KEY_V                  = 0x37,
		KEY_W                  = 0x19,
		KEY_X                  = 0x35,
		KEY_Y                  = 0x1D,
		KEY_Z                  = 0x34,
		LWIN                   = 0x85,
		RWIN                   = 0x86,
		APPS                   = 0x87,
		SLEEP                  = 0x96,
		NUM_0                  = 0x5A,
		NUM_1                  = 0x57,
		NUM_2                  = 0x58,
		NUM_3                  = 0x59,
		NUM_4                  = 0x53,
		NUM_5                  = 0x54,
		NUM_6                  = 0x55,
		NUM_7                  = 0x4F,
		NUM_8                  = 0x50,
		NUM_9                  = 0x51,
		NUM_MULTIPLY           = 0x3F,
		NUM_ADD                = 0x56,
		NUM_SEPARATOR          = UNDEFINED, //?
		NUM_SUBTRACT           = 0x52,
		NUM_DECIMAL            = 0x5B,
		NUM_DIVIDE             = 0x6A,
		F1                     = 0x43,
		F2                     = 0x44,
		F3                     = 0x45,
		F4                     = 0x46,
		F5                     = 0x47,
		F6                     = 0x48,
		F7                     = 0x49,
		F8                     = 0x4A,
		F9                     = 0x4B,
		F10                    = 0x4C,
		F11                    = 0x5F,
		F12                    = 0x60,
		F13                    = 0xBF, //these could be off by one for some keyboards...?
		F14                    = 0xC0,
		F15                    = 0xC1,
		F16                    = 0xC2,
		F17                    = 0xC3,
		F18                    = UNDEFINED, //FIXME(vuko): I'm assuming they go on incrementing by one, but I can't be sure
		F19                    = UNDEFINED,
		F20                    = UNDEFINED,
		F21                    = UNDEFINED,
		F22                    = UNDEFINED,
		F23                    = UNDEFINED,
		F24                    = UNDEFINED,
		NUM_LOCK               = 0x4D,
		SCROLL_LOCK            = 0x4E,
		LSHIFT                 = 0x32,
		RSHIFT                 = 0x3E,
		LCTRL                  = 0x25,
		RCTRL                  = 0x69,
		LMENU                  = UNDEFINED, //wut
		RMENU                  = APPS,
		BROWSER_BACK           = 0xA6,
		BROWSER_FORWARD        = 0xA7,
		BROWSER_REFRESH        = 0xB5,
		BROWSER_STOP           = UNDEFINED, //nope
		BROWSER_SEARCH         = 0xE1,
		BROWSER_FAVORITES      = 0xA4,
		BROWSER_HOME           = 0xB4,
		VOLUME_MUTE            = 0x79,
		VOLUME_DOWN            = 0x7A,
		VOLUME_UP              = 0x7B,
		MEDIA_NEXT             = 0xAB,
		MEDIA_PREV             = 0xAD,
		MEDIA_STOP             = 0xAE,
		MEDIA_PLAY_PAUSE       = 0xAC,
		LAUNCH_MAIL            = UNDEFINED, //:(
		LAUNCH_MEDIA_SELECT    = UNDEFINED,
		LAUNCH_APP1            = UNDEFINED,
		LAUNCH_AP2             = UNDEFINED,
		SEMICOLON_COLON        = 0x2F,
		PLUS                   = 0x15,
		COMMA                  = 0x3B,
		MINUS                  = 0x14,
		PERIOD                 = 0x3C,
		SLASH_QUESTION         = 0x3D,
		TILDE                  = 0x31,
		OPEN_SQUARE_CURLY      = 0x22,
		BACKSLASH_VERTICAL_BAR = 0x33,
		CLOSE_SQUARE_CURLY     = 0x23,
		QUOTE_SINGLE_DOUBLE    = 0x30,
		OEM_8                  = UNDEFINED, //?
		OEM_102                = UNDEFINED,
		IME_PROCESS            = UNDEFINED,
		PACKET                 = UNDEFINED,
		ATTN                   = UNDEFINED,
		CRSEL                  = UNDEFINED,
		EXSEL                  = UNDEFINED,
		EREOF                  = UNDEFINED,
		PLAY                   = UNDEFINED,
		ZOOM                   = UNDEFINED,
		PA1                    = UNDEFINED,
		OEM_CLEAR              = UNDEFINED,
		_COUNT                 = 0xFF
	};
}
#else
	#error [ERROR] Unsupported platform! You are trying to compile for unsupported platform. This won't work.'
#endif
