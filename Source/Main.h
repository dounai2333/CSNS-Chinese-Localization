#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <fstream>
#include <tlhelp32.h>
#include <psapi.h>
#include <thread>
#include <functional>
#include <future>
#include <array>
#include <Wincrypt.h>
#include <cctype>
#include <stdint.h>
#include <mutex>
#include <unordered_map>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <dwmapi.h>
#include <TCHAR.H>
#include <codecvt>
#include <chrono>
#include <shlobj.h>

#include "Miscs\Miscs.h"
#include "Miscs\ArgParser.h"

#include "Memory\module.h"
#include "Memory\mem.h"

using namespace std;
using std::cout;
using std::string;

void PackerMuteMultiFile(DWORD address, string file, DWORD index, int blocktype = 0);
string CheckMemFile();
// Type: i8 ui8 i16 i32 i64 ui16 ui32 ui64 fs fd s h c b
// most type is untested (because I don't need it), so take care when use them!
DWORD RunMemScanAndGetExitCode(DWORD ProcessID, string Type, string Value, string Encoding = "utf-8");
void RunMemScanAndGetAllAddress(DWORD ProcessID, string Type, string Value, DWORD (&output)[128], string Encoding = "utf-8");
void Exit(int exitcode);

static auto find_ptr = [](Module* mod, const char* sig, DWORD sig_add = NULL, DWORD off_add = NULL, bool sub_base = true, bool doubleread = true) -> DWORD
{
    auto off = mem->FindPattern(mod, sig);
    auto sb = sub_base ? mod->GetImage() : 0;
    off = doubleread ? mem->Read<DWORD>(off + sig_add) : off;

    return (!off ? 0 : off + off_add - sb);
};

struct str
{
    char text[CHAR_MAX];
};

struct bigstr
{
    wchar_t text[CHAR_MAX];
};