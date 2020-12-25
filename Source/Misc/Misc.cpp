#include "..\main.h"

MiscFunc* Misc;

void MiscFunc::SetConsoleCursor( bool cursor )
{
	CONSOLE_CURSOR_INFO curCursorInfo;
	curCursorInfo.bVisible = cursor;
	curCursorInfo.dwSize = 1;
	SetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &curCursorInfo );
}

void MiscFunc::CloseConsoleQuickEdit()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	mode &= ~ENABLE_INSERT_MODE;
	SetConsoleMode(hStdin, mode);
}

void MiscFunc::SetConsoleColor( concol textColor, concol bgColor )
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), ( textColor + ( bgColor * 16 ) ) );
}

void MiscFunc::ConsoleClear()
{
	system("cls");
	// why we need this?
	/*
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hStdOut;
	DWORD count;
	DWORD cellCount;
	COORD homeCoords = { 0, 0 };

	hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	if( hStdOut == INVALID_HANDLE_VALUE ) return;

	if( !GetConsoleScreenBufferInfo( hStdOut, &csbi ) ) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	if( !FillConsoleOutputCharacter( hStdOut, ( TCHAR ) ' ', cellCount, homeCoords, &count ) )
		return;

	if( !FillConsoleOutputAttribute( hStdOut, csbi.wAttributes, cellCount, homeCoords, &count ) )
		return;
	SetConsoleCursorPosition( hStdOut, homeCoords );
	*/
}

int MiscFunc::ReadKey()
{
	HANDLE hConsole;
	INPUT_RECORD inrec;
	DWORD count, mode;

	hConsole = GetStdHandle( STD_INPUT_HANDLE );
	if( hConsole == INVALID_HANDLE_VALUE || !GetConsoleMode( hConsole, &mode ) || !SetConsoleMode( hConsole, 0 ) )
		return 0;

	FlushConsoleInputBuffer( hConsole );

	do
	{
		ReadConsoleInput( hConsole, &inrec, 1, &count );
	}
	while( ( inrec.EventType != KEY_EVENT ) || inrec.Event.KeyEvent.bKeyDown );

	SetConsoleMode( hConsole, mode );

	return inrec.Event.KeyEvent.wVirtualKeyCode;
}

void MiscFunc::SetConsoleSize( int w, int h )
{
	HWND hConsole = GetConsoleWindow();	RECT r;

	GetWindowRect( hConsole, &r );

	MoveWindow( hConsole, r.left, r.top, w, h, TRUE );
}

void MiscFunc::SetConsoleEncoding(UINT code)
{
	SetConsoleOutputCP(code);
}

double MiscFunc::GetWindowsMajorVersion()
{
	double ret = 0.0;
	NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
	OSVERSIONINFOEXW osInfo;

	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

	if (NULL != RtlGetVersion)
	{
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		RtlGetVersion(&osInfo);
		ret = (double)osInfo.dwMajorVersion;
	}
	return ret;
}

double MiscFunc::GetWindowsMinorVersion()
{
	double ret = 0.0;
	NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
	OSVERSIONINFOEXW osInfo;

	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

	if (NULL != RtlGetVersion)
	{
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		RtlGetVersion(&osInfo);
		ret = (double)osInfo.dwMinorVersion;
	}
	return ret;
}

bool MiscFunc::FileExist(std::string file)
{
	struct stat buffer;
	return (stat(file.c_str(), &buffer) == 0);
}

std::string MiscFunc::DecimalToHex(int input, bool withhexmark)
{
	std::stringstream ss;
	ss << std::hex << input;
	std::string res(ss.str());
	res = ToUpper(res);
	return withhexmark ? "0x" + res : res;
}

int MiscFunc::HexToDecimal(std::string input)
{
	std::stringstream ss;
	int result;
	ss << input;
	ss >> std::hex >> result;
	return result;
}

std::string MiscFunc::ToUpper(std::string& text)
{
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);
	return text;
}

std::string MiscFunc::ToLower(std::string& text)
{
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
	return text;
}

int MiscFunc::GetKeyFromString( std::string str )
{
	char data = str.c_str()[0];

	if ((data >= '0' && data <= '9') || (data >= 'A' && data <= 'Z'))
		return (int)data;

	if (str == "mouse1") return VK_LBUTTON;
	else if (str == "mouse2") return VK_RBUTTON;
	else if (str == "mouse3") return VK_MBUTTON;
	else if (str == "mouse4") return VK_XBUTTON1;
	else if (str == "mouse5") return VK_XBUTTON2;
	else if (str == "backspace") return VK_BACK;
	else if (str == "tab") return VK_TAB;
	else if (str == "enter") return VK_RETURN;
	else if (str == "shift") return VK_SHIFT;
	else if (str == "ctrl") return VK_CONTROL;
	else if (str == "alt") return VK_MENU;
	else if (str == "capslock") return VK_CAPITAL;
	else if (str == "escape") return VK_ESCAPE;
	else if (str == "space") return VK_SPACE;
	else if (str == "pgup") return VK_PRIOR;
	else if (str == "pgdn") return VK_NEXT;
	else if (str == "end") return VK_END;
	else if (str == "home") return VK_HOME;
	else if (str == "leftarrow") return VK_LEFT;
	else if (str == "rightarrow") return VK_RIGHT;
	else if (str == "uparrow") return VK_UP;
	else if (str == "downarrow") return VK_DOWN;
	else if (str == "ins") return VK_INSERT;
	else if (str == "del") return VK_DELETE;
	else if (str == "numpad_0") return VK_NUMPAD0;
	else if (str == "numpad_1") return VK_NUMPAD1;
	else if (str == "numpad_2") return VK_NUMPAD2;
	else if (str == "numpad_3") return VK_NUMPAD3;
	else if (str == "numpad_4") return VK_NUMPAD4;
	else if (str == "numpad_5") return VK_NUMPAD5;
	else if (str == "numpad_6") return VK_NUMPAD6;
	else if (str == "numpad_7") return VK_NUMPAD7;
	else if (str == "numpad_8") return VK_NUMPAD8;
	else if (str == "numpad_9") return VK_NUMPAD9;
	else if (str == "kp_multiply") return VK_MULTIPLY;
	else if (str == "kp_plus") return VK_ADD;
	else if (str == "kp_minus") return VK_SUBTRACT;
	else if (str == "kp_slash") return VK_DIVIDE;
	else if (str == "f1") return VK_F1;
	else if (str == "f2") return VK_F2;
	else if (str == "f3") return VK_F3;
	else if (str == "f4") return VK_F4;
	else if (str == "f5") return VK_F5;
	else if (str == "f6") return VK_F6;
	else if (str == "f7") return VK_F7;
	else if (str == "f8") return VK_F8;
	else if (str == "f9") return VK_F9;
	else if (str == "f10") return VK_F10;
	else if (str == "f11") return VK_F11;
	else if (str == "f12") return VK_F12;
	else if (str == ";") return VK_OEM_1;
	else if (str == "+") return VK_OEM_PLUS;
	else if (str == "-") return VK_OEM_MINUS;
	else if (str == ",") return VK_OEM_COMMA;
	else if (str == ".") return VK_OEM_PERIOD;
	else if (str == "/") return VK_OEM_2;
	else if (str == "~") return VK_OEM_3;
	else if (str == "[") return VK_OEM_4;
	else if (str == std::to_string(char(0x5C))) return VK_OEM_5;
	else if (str == "]") return VK_OEM_6;
	else if (str == std::to_string(char(0x22))) return VK_OEM_7;
	else return -1;

}

std::string MiscFunc::GetStringFromKey(int key)
{
	char c[2] = { 0 };

	if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z'))
	{
		c[0] = (char)key;
		std::string sweg(c);
		std::transform(sweg.begin(), sweg.end(), sweg.begin(), ::tolower);
		return sweg;
	}

	switch (key)
	{
	case VK_LBUTTON: return std::string("mouse1");
	case VK_RBUTTON: return std::string("mouse2");
	case VK_MBUTTON: return std::string("mouse3");
	case VK_XBUTTON1: return std::string("mouse4");
	case VK_XBUTTON2: return std::string("mouse5");
	case VK_BACK: return std::string("backspace");
	case VK_TAB: return std::string("tab");
	case VK_RETURN: return std::string("enter");
	case VK_SHIFT: return std::string("shift");
	case VK_CONTROL: return std::string("ctrl");
	case VK_MENU: return std::string("alt");
	case VK_CAPITAL: return std::string("capslock");
	case VK_ESCAPE: return std::string("escape");
	case VK_SPACE: return std::string("space");
	case VK_PRIOR: return std::string("pgup");
	case VK_NEXT: return std::string("pgdn");
	case VK_END: return std::string("end");
	case VK_HOME: return std::string("home");
	case VK_LEFT: return std::string("leftarrow");
	case VK_UP: return std::string("uparrow");
	case VK_DOWN: return std::string("downarrow");
	case VK_RIGHT: return std::string("rightarrow");
	case VK_INSERT: return std::string("ins");
	case VK_DELETE: return std::string("del");
	case VK_NUMPAD0: return std::string("numpad_0");
	case VK_NUMPAD1: return std::string("numpad_1");
	case VK_NUMPAD2: return std::string("numpad_2");
	case VK_NUMPAD3: return std::string("numpad_3");
	case VK_NUMPAD4: return std::string("numpad_4");
	case VK_NUMPAD5: return std::string("numpad_5");
	case VK_NUMPAD6: return std::string("numpad_6");
	case VK_NUMPAD7: return std::string("numpad_7");
	case VK_NUMPAD8: return std::string("numpad_8");
	case VK_NUMPAD9: return std::string("numpad_9");
	case VK_MULTIPLY: return std::string("kp_multiply");
	case VK_ADD: return std::string("kp_plus");
	case VK_SUBTRACT: return std::string("kp_minus");
	case VK_DIVIDE: return std::string("kp_slash");
	case VK_F1: return std::string("f1");
	case VK_F2: return std::string("f2");
	case VK_F3: return std::string("f3");
	case VK_F4: return std::string("f4");
	case VK_F5: return std::string("f5");
	case VK_F6: return std::string("f6");
	case VK_F7: return std::string("f7");
	case VK_F8: return std::string("f8");
	case VK_F9: return std::string("f9");
	case VK_F10: return std::string("f10");
	case VK_F11: return std::string("f11");
	case VK_F12: return std::string("f12");
	case VK_OEM_1: return std::string(";");
	case VK_OEM_PLUS: return std::string("+");
	case VK_OEM_MINUS: return std::string("-");
	case VK_OEM_COMMA: return std::string(",");
	case VK_OEM_PERIOD: return std::string(".");
	case VK_OEM_2: return std::string("/");
	case VK_OEM_3: return std::string("~");
	case VK_OEM_4: return std::string("[");
	case VK_OEM_5: return std::to_string(char(0x5C));
	case VK_OEM_6: return std::string("]");
	case VK_OEM_7: return std::to_string(char(0x22));
	default: return std::string("unknown key");
	}
}