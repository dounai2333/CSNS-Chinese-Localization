#include "Main.h"

int main()
{
    SetConsoleTitle("CSN:S汉化程序");
    Misc->CloseConsoleQuickEdit();
    Misc->SetConsoleSize(370, 385);
    Misc->SetConsoleCursor(false);
    //Misc->SetConsoleEncoding(CP_UTF8);

    Misc->ConsoleClear();

    if (!mem->Attach("cstrike-online.exe"))
    {
        cout <<"cstrike-online.exe 未启动...\n"
             <<"请在右下角加载N社反作弊时开启此程序\n\n"
             <<"以下是使用须知和温馨提醒：\n"
             <<"- 请确保带启动项\"-disableauthui\"\n"
             <<"--推荐同时使用-login和-password\n"
             <<"-- -login 你的帐号 -password 你的密码\n\n"
             <<"- 不保证绝对不会出现程序BUG\n"
             <<"- 不保证游戏数次更新后仍可用\n\n"
             <<"- 如提示\"error TS6,1 (GetLastError 0x20)\"\n"
             <<"-- 无视并点击确定即可，提示2次后正常游戏\n\n"
             <<"Made by dounai2333 (QQ1328600509)\n\n";
        system("pause");
        exit(1);
    }

    cout << "cstrike-online.exe (PID: "<< mem->m_dwProcessId << ") attached.\nAttach handle: "<< (int)mem->m_hProcess <<"("<< Misc->DecimalToHex((int)mem->m_hProcess, true) <<")\n\n";

    // if user start program too early, we will hit exception in here.
    Module* hw = mem->GetModule("hw.dll");
    Module* filesystem = mem->GetModule("filesystem_nar.dll");
    cout << "hw\t\t- 0x" << (int*)hw->GetImage() << "\n";
    cout << "filesystem_nar\t- 0x" << (int*)filesystem->GetImage() << "\n";
    
    if (mem->GetModule("mp.dll"))
    {
        system("cls");
        cout << "游戏已完成加载!开启此程序不会生效!\n请确保在游戏运行前就打开本软件!\n\n";
        system("pause");
        exit(-1);
    }

    // string, original content: na_en (it's protected so we can not scan it with "writable")
    // this signature is little dangers, may it will not working anymore if has any update
    DWORD langaddr = hw->GetImage() + find_ptr(hw, "C7 00 ? ? ? ? C7 40 04 ? ? ? ? E8 ? ? ? ? 83 C4 10 6A 00 56 57 83 EC 08 8B C4", 0x2, 0x3C);
    // string, original content: /fixtrike/
    // if filesysyem_nar.dll get any update then we need to check this
    DWORD fixaddr = mem->Read<DWORD>(filesystem->GetImage() + 0xCC6E4);

    string langtext = mem->Read<t>(langaddr).text;
    string fixtext = mem->Read<t>(fixaddr).text;

    if (fixtext == "/cstrike_chn/" && langtext == "chn")
    {
        system("cls");
        cout << "请勿重复操作!待游戏加载完毕后即可加载汉化\n";
        system("pause");
        exit(0);
    }
    else if (fixtext != "/fixtrike/" || langtext != "na_en")
    {
        system("cls");
        cout << "检测到非法内容,可能由于游戏更新基址已过期\n";
        system("pause");
        exit(-1);
    }

    // pause the game process so we can do the memory scan and override without trouble
    DebugActiveProcess(mem->m_dwProcessId);
    DebugSetProcessKillOnExit(false);
    cout << "\n已冻结游戏进程,正在进行操作...\n\n";
    
    mem->Write(fixaddr, "/cstrike_chn/");
    cout << "国服nar重定向已完成\n";

    mem->WriteProtected(langaddr, "chn");
    cout << "选择中国语已完成\n\n";

    DWORD resourceaddr = NULL;
    string memfile_missing_list = CheckMemFile();
    if (memfile_missing_list == "")
    {
        // hex content: /cstrike_chn/resource/background/800_2_c_advice.tga (with extra 00 security verification hex to prevent error result)
        DWORD exitcode = RunMemScanAndGetExitCode("-pid=" + to_string(mem->m_dwProcessId) + " -T=\"h\" -value=\"2F 63 73 74 72 69 6B 65 5F 63 68 6E 2F 72 65 73 6F 75 72 63 65 2F 62 61 63 6B 67 72 6F 75 6E 64 2F 38 30 30 5F 32 5F 63 5F 61 64 76 69 63 65 2E 74 67 61 00 33 00 00 00 33\"");

        if ((string)mem->Read<t>(exitcode).text == "/cstrike_chn/resource/background/800_2_c_advice.tga")
            resourceaddr = exitcode;
        else if (exitcode == 2)
            cout << "内存扫描失败!请检查fixtrike.nar!\n其应被放置在Data目录内且不得改名!\n";
        else
            cout << "内存扫描出现意外错误!\n返回数值:" << exitcode << "(" << Misc->DecimalToHex(exitcode, true) << ")\n";
    }
    else
    {
        cout << "因缺少内存扫描必要文件,已跳过...\n不进行内存扫描可能会导致游戏出现问题!\n缺少文件: "<<memfile_missing_list<<"\n";
    }

    if (resourceaddr != NULL)
    {
        for (int i = 0; i < 2048; i++)
        {
            string filename = mem->Read<t>(resourceaddr + i * 0x40).text;
            if (filename.find("/cstrike_chn") == -1)
                break;

            if (filename == "/cstrike_chn/resource/bad_words.csv")
            {
                cout << "将bad_words.csv重定向至游戏原始内容...\n"; // 脏话屏蔽列表
                mem->Write(resourceaddr + i * 0x40, "/cstrike_na_en/resource/bad_words.csv");
            }
            if (filename == "/cstrike_chn/resource/chat_filter_list.csv")
            {
                cout << "将chat_filter_list.csv重定向...\n"; // 早期的脏话屏蔽列表
                mem->Write(resourceaddr + i * 0x40, "/cstrike_na_en/resource/chat_filter_list.csv");
            }
            if (filename == "/cstrike_chn/resource/item.csv")
            {
                cout << "将item.csv重定向至游戏原始内容...\n"; // 游戏里各种道具的定义
                mem->Write(resourceaddr + i * 0x40, "/cstrike_na_en/resource/item.csv");
            }
            if (filename == "/cstrike_chn/resource/relation_product_ver2.csv")
            {
                cout << "将relation_product_ver2.csv重定向...\n"; // 商城道具的定义
                mem->Write(resourceaddr + i * 0x40, "/cstrike_na_en/resource/relation_product_ver2.csv");
            }
        }
    }

    cout << "\n操作执行完毕,已加载汉化! :)\n按下任意键退出汉化程序解冻游戏进程!\nMade by dounai2333 (QQ1328600509)\n";
    system("pause");
    //Sleep(3000);

    mem->Detach();
    return 0;
}

string CheckMemFile()
{
    string filelist[5] = { "clrcompression.dll", "clrjit.dll", "coreclr.dll", "MemScan.exe", "mscordaccore.dll" };
    string mem_file_missing_list = "";
    for (int i = 0; i < 5; i++)
    {
        if (!Misc->FileExist(filelist[i]))
        {
            if (mem_file_missing_list.length() == 0)
                mem_file_missing_list += filelist[i];
            else
                mem_file_missing_list += ", " + filelist[i];
        }
    }
    return mem_file_missing_list;
}

DWORD RunMemScanAndGetExitCode(string args)
{
    // thanks Bunsei for his Memory Scan program
    LPSTR start_str = strdup(string("MemScan.exe " + args).c_str());
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD exit_code;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcess(NULL, start_str, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exit_code);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return exit_code;
    }

    return NULL;
}