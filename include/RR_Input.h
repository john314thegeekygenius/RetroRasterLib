/*
	RetroRasterLib - a 2D graphics engine
	Written by Jonathan Clevenger
	January 3, 2023
	Copyright (c) 2023 TimeLabs LLC

	Use of this libaray is allowed
	as long as the name and author is 
	refrenced to in the application
	using it, such as:
    
				Made using RetroRasterLib
	www.github.com/john314thegeekygenius/RetroRasterLib

	The following comment must appear in
	all released source code using the library:

	=================================================================
	Uses RetroRasterLib written by Jonathan Clevenger
	Find it here: www.github.com/john314thegeekygenius/RetroRasterLib
	=================================================================

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
	
*/
#pragma once

#include <RR_Headers.h>

const SDL_KeyCode SDL_KeyMapper[256] = {
	SDLK_UNKNOWN, SDLK_RETURN, SDLK_ESCAPE, SDLK_BACKSPACE, SDLK_TAB, SDLK_SPACE, SDLK_EXCLAIM, SDLK_QUOTEDBL, SDLK_HASH, SDLK_PERCENT, SDLK_DOLLAR, SDLK_AMPERSAND, SDLK_QUOTE, SDLK_LEFTPAREN, SDLK_RIGHTPAREN, SDLK_ASTERISK, SDLK_PLUS, 
	SDLK_COMMA, SDLK_MINUS, SDLK_PERIOD, SDLK_SLASH, SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_COLON, SDLK_SEMICOLON, SDLK_LESS, 
	SDLK_EQUALS, SDLK_GREATER, SDLK_QUESTION, SDLK_AT, SDLK_LEFTBRACKET, SDLK_BACKSLASH, SDLK_RIGHTBRACKET, SDLK_CARET, SDLK_UNDERSCORE, SDLK_BACKQUOTE, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f, SDLK_g, 
	SDLK_h, SDLK_i, SDLK_j, SDLK_k, SDLK_l, SDLK_m, SDLK_n, SDLK_o, SDLK_p, SDLK_q, SDLK_r, SDLK_s, SDLK_t, SDLK_u, SDLK_v, SDLK_w, SDLK_x, 
	SDLK_y, SDLK_z, SDLK_CAPSLOCK, SDLK_F1, SDLK_F2, SDLK_F3, SDLK_F4, SDLK_F5, SDLK_F6, SDLK_F7, SDLK_F8, SDLK_F9, SDLK_F10, SDLK_F11, SDLK_F12, SDLK_PRINTSCREEN, SDLK_SCROLLLOCK, 
	SDLK_PAUSE, SDLK_INSERT, SDLK_HOME, SDLK_PAGEUP, SDLK_DELETE, SDLK_END, SDLK_PAGEDOWN, SDLK_RIGHT, SDLK_LEFT, SDLK_DOWN, SDLK_UP, SDLK_NUMLOCKCLEAR, SDLK_KP_DIVIDE, SDLK_KP_MULTIPLY, SDLK_KP_MINUS, SDLK_KP_PLUS, SDLK_KP_ENTER, 
	SDLK_KP_1, SDLK_KP_2, SDLK_KP_3, SDLK_KP_4, SDLK_KP_5, SDLK_KP_6, SDLK_KP_7, SDLK_KP_8, SDLK_KP_9, SDLK_KP_0, SDLK_KP_PERIOD, SDLK_APPLICATION, SDLK_POWER, SDLK_KP_EQUALS, SDLK_F13, SDLK_F14, SDLK_F15, 
	SDLK_F16, SDLK_F17, SDLK_F18, SDLK_F19, SDLK_F20, SDLK_F21, SDLK_F22, SDLK_F23, SDLK_F24, SDLK_EXECUTE, SDLK_HELP, SDLK_MENU, SDLK_SELECT, SDLK_STOP, SDLK_AGAIN, SDLK_UNDO, SDLK_CUT, 
	SDLK_COPY, SDLK_PASTE, SDLK_FIND, SDLK_MUTE, SDLK_VOLUMEUP, SDLK_VOLUMEDOWN, SDLK_KP_COMMA, SDLK_KP_EQUALSAS400, SDLK_ALTERASE, SDLK_SYSREQ, SDLK_CANCEL, SDLK_CLEAR, SDLK_PRIOR, SDLK_RETURN2, SDLK_SEPARATOR, SDLK_OUT, 
	SDLK_OPER, SDLK_CLEARAGAIN, SDLK_CRSEL, SDLK_EXSEL, SDLK_KP_00, SDLK_KP_000, SDLK_THOUSANDSSEPARATOR, SDLK_DECIMALSEPARATOR,  SDLK_CURRENCYUNIT, SDLK_CURRENCYSUBUNIT,  SDLK_KP_LEFTPAREN, SDLK_KP_RIGHTPAREN, SDLK_KP_LEFTBRACE, SDLK_KP_RIGHTBRACE, 
	SDLK_KP_TAB, SDLK_KP_BACKSPACE, SDLK_KP_A, SDLK_KP_B, SDLK_KP_C, SDLK_KP_D, SDLK_KP_E, SDLK_KP_F, SDLK_KP_XOR, SDLK_KP_POWER, SDLK_KP_PERCENT, SDLK_KP_LESS, SDLK_KP_GREATER, SDLK_KP_AMPERSAND, SDLK_KP_DBLAMPERSAND,  SDLK_KP_VERTICALBAR, 
	 SDLK_KP_DBLVERTICALBAR,  SDLK_KP_COLON, SDLK_KP_HASH, SDLK_KP_SPACE, SDLK_KP_AT, SDLK_KP_EXCLAM, SDLK_KP_MEMSTORE, SDLK_KP_MEMRECALL, SDLK_KP_MEMCLEAR, SDLK_KP_MEMADD, SDLK_KP_MEMSUBTRACT,  SDLK_KP_MEMMULTIPLY,  SDLK_KP_MEMDIVIDE, 
	SDLK_KP_PLUSMINUS, SDLK_KP_CLEAR, SDLK_KP_CLEARENTRY, SDLK_KP_BINARY, SDLK_KP_OCTAL, SDLK_KP_DECIMAL, SDLK_KP_HEXADECIMAL,  SDLK_LCTRL, SDLK_LSHIFT, SDLK_LALT, SDLK_LGUI, SDLK_RCTRL, SDLK_RSHIFT, SDLK_RALT, SDLK_RGUI, SDLK_MODE, 
	SDLK_AUDIONEXT, SDLK_AUDIOPREV, SDLK_AUDIOSTOP, SDLK_AUDIOPLAY, SDLK_AUDIOMUTE, SDLK_MEDIASELECT, SDLK_WWW, SDLK_MAIL, SDLK_CALCULATOR, SDLK_COMPUTER, SDLK_AC_SEARCH, SDLK_AC_HOME, SDLK_AC_BACK, SDLK_AC_FORWARD, SDLK_AC_STOP, SDLK_AC_REFRESH, SDLK_AC_BOOKMARKS, 
};

enum RR_KeyCode {
	RRK_NONE,
	RRK_RETURN,
	RRK_ESCAPE,
	RRK_BACKSPACE,
	RRK_TAB,
	RRK_SPACE,
	RRK_EXCLAIM,
	RRK_QUOTEDBL,
	RRK_HASH,
	RRK_PERCENT,
	RRK_DOLLAR,
	RRK_AMPERSAND,
	RRK_QUOTE,
	RRK_LEFTPAREN,
	RRK_RIGHTPAREN,
	RRK_ASTERICK,
	RRK_PLUS,
	RRK_COMMA,
	RRK_MINUS,
	RRK_PERIOD,
	RRK_SLASH,
	RRK_0,
	RRK_1,
	RRK_2,
	RRK_3,
	RRK_4,
	RRK_5,
	RRK_6,
	RRK_7,
	RRK_8,
	RRK_9,
	RRK_COLON,
	RRK_SEMICOLON,
	RRK_LESS_THAN,
	RRK_EQUALS,
	RRK_GREATER_THAN,
	RRK_QUESTION_MARK,
	RRK_AT,
	RRK_LEFT_BRACKET,
	RRK_BACKSLASH,
	RRK_RIGHT_BRACKET,
	RRK_CARET,
	RRK_UNDERSCORE,
	RRK_BACKQUOTE,
	RRK_A,
	RRK_B,
	RRK_C,
	RRK_D,
	RRK_E,
	RRK_F,
	RRK_G,
	RRK_H,
	RRK_I,
	RRK_J,
	RRK_K,
	RRK_L,
	RRK_M,
	RRK_N,
	RRK_O,
	RRK_P,
	RRK_Q,
	RRK_R,
	RRK_S,
	RRK_T,
	RRK_U,
	RRK_V,
	RRK_W,
	RRK_X,
	RRK_Y,
	RRK_Z,
	RRK_CAPSLOCK,
	RRK_F1,
	RRK_F2,
	RRK_F3,
	RRK_F4,
	RRK_F5,
	RRK_F6,
	RRK_F7,
	RRK_F8,
	RRK_F9,
	RRK_F10,
	RRK_F11,
	RRK_F12,
	RRK_PRINT_SCREEN,
	RRK_SCROLL_LOCK,
	RRK_PAUSE,
	RRK_INSERT,
	RRK_HOME,
	RRK_PAGE_UP,
	RRK_DELETE,
	RRK_END,
	RRK_PAGE_DOWN,
	RRK_RIGHT,
	RRK_LEFT,
	RRK_DOWN,
	RRK_UP,
	RRK_NUMLOCK_CLEAR,
	RRK_KP_DIVIDE,
	RRK_KP_MULTIPLY,
	RRK_KP_MINUS,
	RRK_KP_PLUS,
	RRK_KP_ENTER,
	RRK_KP_1,
	RRK_KP_2,
	RRK_KP_3,
	RRK_KP_4,
	RRK_KP_5,
	RRK_KP_6,
	RRK_KP_7,
	RRK_KP_8,
	RRK_KP_9,
	RRK_KP_0,
	RRK_KP_PERIOD,
	RRK_APPLICATION,
	RRK_POWER,
	RRK_KP_EQUALS,
	RRK_F13,
	RRK_F14,
	RRK_F15,
	RRK_F16,
	RRK_F17,
	RRK_F18,
	RRK_F19,
	RRK_F20,
	RRK_F21,
	RRK_F22,
	RRK_F23,
	RRK_F24,
	RRK_EXECUTE,
	RRK_HELP,
	RRK_MENU,
	RRK_SELECT,
	RRK_STOP,
	RRK_AGAIN,
	RRK_UNDO,
	RRK_CUT,
	RRK_COPY,
	RRK_PASTE,
	RRK_FIND,
	RRK_MUTE,
	RRK_VOLUME_UP,
	RRK_VOLUME_DOWN,
	RRK_KP_COMMA,
	RRK_KP_EQUALS_AS_400,
	RRK_ALT_ERASE,
	RRK_SYSREQ,
	RRK_CANCEL,
	RRK_CLEAR,
	RRK_PRIOR,
	RRK_RETURN2,
	RRK_SEPERATOR,
	RRK_OUT,
	RRK_OPER,
	RRK_CLEAR_AGAIN,
	RRK_CRSEL,
	RRK_EXSEL,
	RRK_KP_00,
	RRK_KP_000,
	RRK_THOUSAND_SEPARATOR,
	RRK_DECEMAL_SEPARATOR,
	RRK_CURRENCY_UNIT,
	RRK_CURRENCY_SUB_UNIT,
	RRK_KP_LEFT_PAREN,
	RRK_KP_RIGHT_PAREN,
	RRK_KP_LEFT_BRACE,
	RRK_KP_RIGHT_BRACE,
	RRK_KP_TAB,
	RRK_KP_BACKSPACE,
	RRK_KP_A,
	RRK_KP_B,
	RRK_KP_C,
	RRK_KP_D,
	RRK_KP_E,
	RRK_KP_F,
	RRK_KP_XOR,
	RRK_KP_POWER,
	RRK_KP_PERCENT,
	RRK_KP_LESS_THAN,
	RRK_KP_GREATER_THAN,
	RRK_KP_AMPERSAND,
	RRK_KP_DBL_AMPERSAND,
	RRK_KP_VERTICAL_BAR,
	RRK_KP_DBL_VERTICAL_BAR,
	RRK_KP_COLON,
	RRK_KP_HASH,
	RRK_KP_SPACE,
	RRK_KP_AT,
	RRK_KP_EXLAM,
	RRK_KP_MEM_STORE,
	RRK_KP_MEM_RECALL,
	RRK_KP_MEM_CLEAR,
	RRK_KP_MEM_ADD,
	RRK_KP_MEM_SUBTRACT,
	RRK_KP_MEM_MULTIPLY,
	RRK_KP_MEM_DIVIDE,
	RRK_KP_PLUS_MINUS,
	RRK_KP_CLEAR,
	RRK_KP_CLEAR_ENTRY,
	RRK_KP_BINARY,
	RRK_KP_OCTAL,
	RRK_KP_DECIMAL,
	RRK_KP_HEXADECIMAL,
	RRK_LCTRL,
	RRK_LSHIFT,
	RRK_LALT,
	RRK_LGUI,
	RRK_RCTRL,
	RRK_RSHIFT,
	RRK_RALT,
	RRK_RGUI,
	RRK_MODE,
	RRK_AUDIO_NEXT,
	RRK_AUDIO_PREVIOUS,
	RRK_AUDIO_STOP,
	RRK_AUDIO_PLAY,
	RRK_AUDIO_MUTE,
	RRK_MEDIA_SELECT,
	RRK_WWW,
	RRK_MAIL,
	RRK_CALCULATOR,
	RRK_COMPUTER,
	RRK_AC_SEARCH,
	RRK_AC_HOME,
	RRK_AC_BACK,
	RRK_AC_FORWARD,
	RRK_AC_STOP,
	RRK_AC_REFRESH,
	RRK_AC_BOOKMARKS
};

bool RR_GetKey(RR_Window &window, RR_KeyCode key);

typedef struct RR_MButton_t {
	uint8_t cur_state; // what state is the button currently (released / pressed)
	uint8_t last_state; // what state was the button last in?
	uint8_t clicks; // how many times was the mouse button clicked
	int x, y; // Where on the screen was the mouse pressed?
}RR_MButton;

#define RR_MAX_MOUSE_BUTTONS 32

typedef struct RR_Mouse_t {
	int x; // Mouse x position /w scale correction
	int y; // Mouse y position /w scale correction
	int real_x; // Mouse x position on window
	int real_y; // Mouse y position on window
	int h_move; // Relative motion horizontally
	int v_move; // Relative motion vertically
	RR_MButton buttons[RR_MAX_MOUSE_BUTTONS]; // Button states
	int wheel_x; // distance scrolled horizontally -left +right
	int wheel_y; // distance scrolled vertically -down +up
	int wheel_dir; // direction wheel is scrolled
	int window_id; // window id
}RR_Mouse;

RR_Mouse RR_GetMouse(RR_Window &window);
int RR_GetMouseX(RR_Window &window);
int RR_GetMouseY(RR_Window &window);
int RR_GetMouseRealX(RR_Window &window);
int RR_GetMouseRealY(RR_Window &window);
int RR_GetMouseWheelX(RR_Window &window);
int RR_GetMouseWheelY(RR_Window &window);
RR_MButton RR_GetMouseButton(RR_Window &window, int id);

#define RR_MAX_CONTROLER_BUTTONS 32

typedef struct RR_Controler_t {
	// Joystick axies
	float x[2], y[2];
	int real_x[2], real_y[2];
	int num_axis;
	// Triggers
	float trigger_value[2];
	int real_trigger_value[2];
	// Hat
	int hat_x, hat_y;
	// General Buttons
	bool buttons[RR_MAX_CONTROLER_BUTTONS];
	int num_buttons;
	// Rumble
	bool can_rumble;
	// Is the controler still available?
	bool available;
	// SDL controler ID
	int controler_id;
}RR_Controler;


// Adds a controler
void RR_AddControler(int joystick_id);

// Removes a controler
void RR_RemoveControler(int joystick_id);

// Return the number of controlers connected
int RR_GetControlerCount();

// Get a specific controler
RR_Controler RR_GetControler(int controler_id);

// Get the controler name
std::string RR_GetControlerName(int controler_id);

// Rumble the controler
void RR_RumbleControler(RR_Controler &controler, float l_rumble, float r_rumble, int len_ms);

