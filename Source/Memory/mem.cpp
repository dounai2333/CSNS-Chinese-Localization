#include "..\main.h"

Mem* mem = new Mem();

Mem::Mem()
{

}

Mem::~Mem()
{
	Detach();
}

bool Mem::Attach( const std::string& ExeName )
{
	Detach();

	if(ExeName.empty() )
	{
		return false;
	}

	m_dwProcessId = GetProcessIdByName( ExeName );
	if( !m_dwProcessId )
	{
		return false;
	}

	m_hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, m_dwProcessId );

	if( !m_hProcess )
	{
		return false;
	}

	return DumpModList();
}

void Mem::Detach()
{
	if( m_hProcess )
	{
		CloseHandle( m_hProcess );
	}

	m_hProcess = NULL;
	m_dwProcessId = NULL;

	SAFE_DELETE_VECTOR( m_pModList );
}

bool Mem::Read( DWORD_PTR dwAddress, LPVOID lpBuffer, DWORD_PTR dwSize )
{
	SIZE_T Out = NULL;

	return ( ReadProcessMemory( m_hProcess, ( LPCVOID )dwAddress, lpBuffer, dwSize, &Out ) == TRUE );
}

bool Mem::Write( DWORD_PTR dwAddress, LPCVOID lpBuffer, DWORD_PTR dwSize )
{
	SIZE_T Out = NULL;

	return ( WriteProcessMemory( m_hProcess, ( LPVOID )dwAddress, lpBuffer, dwSize, &Out ) == TRUE );
}

Module* Mem::GetModule( const std::string& ImageName )
{
	if( m_pModList.empty() )
	{
		return nullptr;
	}

	for( auto& pMod : m_pModList )
	{
		if( ImageName.compare( pMod->GetName() ) == 0 )
		{
			return pMod;
		}
	}

	return nullptr;
}

void Mem::SetWindow( HWND window )
{
	m_hWindow = window;
}

HWND Mem::GetWindow()
{
	return m_hWindow;
}

HMODULE Mem::LoadRemote( const std::string& ImageName )
{
	if( m_pModList.empty() )
	{
		return NULL;
	}

	for( auto& pMod : m_pModList )
	{
		if( ImageName.compare( pMod->GetName() ) == 0 )
		{
			return LoadLibrary( pMod->GetPath().c_str() );
		}
	}

	return NULL;
}

bool DataCompare( const BYTE* pData, const BYTE* pMask, const char* pszMask )
{
	for( ; *pszMask; ++pszMask, ++pData, ++pMask )
	{
		if( *pszMask == 'x' && *pData != *pMask )
		{
			return false;
		}
	}

	return ( *pszMask == NULL );
}

DWORD_PTR Mem::Scan( DWORD_PTR dwStart, DWORD_PTR dwSize, const char* pSignature, const char* pMask )
{
	BYTE* pRemote = new BYTE[ dwSize ];	// Forgot to delete this after we're finshed with it, resulting in a memory leak

	if( !Read( dwStart, pRemote, dwSize ) )
	{
		delete[] pRemote;
		return NULL;
	}

	for( DWORD_PTR dwIndex = 0; dwIndex < dwSize; dwIndex++ ) {
		if( DataCompare( ( const BYTE* )( pRemote + dwIndex ), ( const BYTE* )pSignature, pMask ) )
		{
			delete[] pRemote;
			return ( dwStart + dwIndex );
		}
	}
	delete[] pRemote;

	return NULL;
}

DWORD_PTR Mem::FindPattern(Module* mod, const char* pattern)
{
	if (!mod->GetImage())
		return 0;

	static auto compare_bytes = [](const byte* bytes, const char* pattern) -> bool
	{
		for (; *pattern; *pattern != ' ' ? ++bytes : bytes, ++pattern) {
			if (*pattern == ' ' || *pattern == '?')
				continue;
			if (*bytes != getByte(pattern))
				return false;
			++pattern;
		}
		return true;
	};

	auto get_text_section = [&](uintptr_t& start, size_t& size)
	{
		auto header = (byte*)malloc(0x1000);
		Read(mod->GetImage(), header, 0x1000);

		auto pDosHdr = (const IMAGE_DOS_HEADER*)(header);
		if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
		{
			::free(header);
			return false;
		}
		const IMAGE_NT_HEADERS *pImageHdr = (const IMAGE_NT_HEADERS*)((uint8_t*)pDosHdr + pDosHdr->e_lfanew);

		if (pImageHdr->Signature != IMAGE_NT_SIGNATURE)
		{
			::free(header);
			return false;
		}

		auto pSection = (const IMAGE_SECTION_HEADER*)((uint8_t*)pImageHdr + sizeof(IMAGE_NT_HEADERS));
		for (int i = 0; i < pImageHdr->FileHeader.NumberOfSections; ++i, pSection++)
		{
			if (_stricmp((LPCSTR)pSection->Name, ".text") == 0)
			{
				start = (uintptr_t)(mod->GetImage() + pSection->VirtualAddress);
				size = pSection->Misc.VirtualSize;
				::free(header);
				return true;
			}
		}

		::free(header);
		return false;
	};

	uintptr_t base; size_t size; 
	if (!get_text_section(base, size))
	{
		base = mod->GetImage();
		size = mod->GetSize();
	}

	auto pb = (byte*)malloc(size);
	auto max = size;
	Read(base, pb, size);

	uintptr_t offset = 0;
	for (auto off = 0UL; off < max; ++off) {
		if (compare_bytes(pb + off, pattern)) {
			offset = base + off;
			break;
		}
	}

	return offset;
}

bool Mem::DumpModList()
{
	m_pModList.clear();
	
	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, m_dwProcessId );

	if( hSnapshot == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	MODULEENTRY32 Entry = { NULL };

	Entry.dwSize = sizeof( MODULEENTRY32 );

	if( !Module32First( hSnapshot, &Entry ) )
	{
		CloseHandle( hSnapshot );
		return false;
	}

	Module* pMod = nullptr;

	do
	{
		char szPath[ MAX_PATH ] = { NULL };

		GetModuleFileNameEx( m_hProcess, Entry.hModule, szPath, MAX_PATH );

		pMod = new Module( ( DWORD_PTR )Entry.hModule, ( DWORD_PTR )Entry.modBaseSize, Entry.szModule, szPath );
		m_pModList.push_back( pMod );
	} while( Module32Next( hSnapshot, &Entry ) );

	CloseHandle( hSnapshot );

	return !m_pModList.empty();
}

BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == lParam)
	{
		mem->WindowHandle = hwnd;
		return FALSE;
	}
	return TRUE;
}

DWORD Mem::GetProcessIdByName( const std::string& name )
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

	if( hSnapshot == INVALID_HANDLE_VALUE )
	{
		return NULL;
	}

	PROCESSENTRY32 Entry = { NULL };

	Entry.dwSize = sizeof( PROCESSENTRY32 );

	if( !Process32First( hSnapshot, &Entry ) )
	{
		CloseHandle( hSnapshot );
		return NULL;
	}

	bool finded = false;
	do
	{
		if( name.compare( Entry.szExeFile ) == 0 )
		{
			finded = true;
			break;
		}
	} while( Process32Next( hSnapshot, &Entry ) );
	
	CloseHandle( hSnapshot );
	return finded ? Entry.th32ProcessID : NULL;
}

DWORD Mem::GetProcessIdByWindow(const std::string& name)
{
	HWND Window = FindWindow(NULL,name.c_str());
	if (Window == NULL)
	{
		return NULL;
	}
	DWORD processID;
	DWORD actualProcId = GetWindowThreadProcessId(Window, &processID);
	if (processID == NULL)
	{
		return NULL;
	}
	return processID;
}

std::string Mem::GetProcessNameByWindow(const std::string& name)
{
	HWND Window = FindWindow(NULL, name.c_str());
	if (Window == NULL)
	{
		return "Unknown";
	}
	DWORD processID;
	DWORD actualProcId = GetWindowThreadProcessId(Window, &processID);
	if (processID == NULL)
	{
		return "Unknown";
	}
	HANDLE h_Process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	char path[_MAX_PATH + 1] = "";
	char fname[_MAX_FNAME];
	GetModuleFileNameEx(h_Process, NULL, path, MAX_PATH + 1);
	_splitpath(path, NULL, NULL, fname, NULL);
	if (fname == NULL)
	{
		return "Unknown";
	}
	std::string nn = ((std::string)fname + ".exe").c_str();
	return nn;
}

std::string Mem::GetProcessNameById(DWORD name)
{
	DWORD processID = name;
	HANDLE h_Process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (h_Process == NULL)
	{
		return "Unknown";
	}
	char path[_MAX_PATH + 1] = "";
	char fname[_MAX_FNAME];
	GetModuleFileNameEx(h_Process, NULL, path, MAX_PATH + 1);
	_splitpath(path, NULL, NULL, fname, NULL);
	if (fname == NULL)
	{
		return "Unknown";
	}
	CloseHandle(h_Process);
	std::string nn = ((std::string)fname + ".exe").c_str();
	return nn;
}

DWORD Mem::GetThreadById(DWORD name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 te32;

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	te32.dwSize = sizeof(THREADENTRY32);

	if (!Thread32First(hSnapshot, &te32))
	{
		CloseHandle(hSnapshot);
		return NULL;
	}

	bool finded = false;
	do
	{
		if (te32.th32OwnerProcessID == name)
		{
			finded = true;
			break;
		}
	} while (Thread32Next(hSnapshot, &te32));

	CloseHandle(hSnapshot);
	return finded ? te32.th32ThreadID : NULL;
}

std::string Mem::GetWindowById(DWORD name)
{
	EnumWindows(EnumWindowsProcMy, name);
	HWND Window = WindowHandle;
	if (Window == NULL)
	{
		return "Unknown";
	}
	char windowTitle[256];
	GetWindowText(Window, windowTitle, sizeof(windowTitle));
	if ((std::string)windowTitle == "")
	{
		return "Unknown / Multi-window";
	}
	return windowTitle;
}

std::string Mem::GetWindowByName(const std::string& name)
{
	DWORD processID = GetProcessIdByName(name);
	EnumWindows(EnumWindowsProcMy, processID);
	HWND Window = WindowHandle;
	if (Window == NULL)
	{
		return "Unknown";
	}
	char windowTitle[256];
	GetWindowText(Window, windowTitle, sizeof(windowTitle));
	if ((std::string)windowTitle == "")
	{
		return "Unknown / Multi-window";
	}
	return windowTitle;
}