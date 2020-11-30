#include "Main.h"

using namespace std;
using std::cout;
using std::string;

struct t
{
    char text[128];
};

int main()
{
    SetConsoleTitle("CSO Chinese Localization");
    Misc->CloseConsoleQuickEdit();
    Misc->SetConsoleSize(370, 350);
    Misc->SetConsoleCursor(false);
    //Misc->SetConsoleEncoding(CP_UTF8);

    Misc->ConsoleClear();

    if (!mem->Attach("cstrike-online.exe"))
    {
        cout << "cstrike-online.exe 未启动...\n请在右下角加载N社反作弊时开启此程序\n\n以下是使用须知和温馨提醒：\n- 请确保带启动项\"-disableauthui\"\n--推荐同时使用-login和-password\n-- -login 你的帐号 -password 你的密码\n- 不保证不会出现程序BUG\n- 不保证游戏更新后仍可用\n- 如提示\"error TS6,1 (GetLastError 0x20)\"\n-- 无视并点击确定即可，提示2次后正常游戏\n\nMade by dounai2333 (QQ1328600509)\n\n";
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
    DWORD langaddr = hw->GetImage() + 0x870DE8;
    // string, original content: /fixtrike/ (we have the + 1 so actually it's fixtrike/ )
    DWORD fixaddr = mem->Read<DWORD>(filesystem->GetImage() + 0xCC6E4) + 1;

    string langtext = mem->Read<t>(langaddr).text;
    string fixtext = mem->Read<t>(fixaddr).text;

    if (fixtext == "cstrike_chn/" || langtext == "chn")
    {
        system("cls");
        cout << "请勿重复操作!待游戏加载完毕后即可加载汉化\n";
        system("pause");
        exit(0);
    }
    else if (fixtext != "fixtrike/" || langtext != "na_en")
    {
        system("cls");
        cout << "检测到非法内容,可能由于游戏更新基址已过期\n";
        system("pause");
        exit(-1);
    }

    // pause the game process so we can do the memory scan without trouble
    DebugActiveProcess(mem->m_dwProcessId);
    DebugSetProcessKillOnExit(false);

    cout << "\n已冻结游戏进程,正在进行操作...\n\n";
    
    // content: cstrike_chn/ (with 00 ends)
    char cstrike_chn[13] = { 0x63, 0x73, 0x74, 0x72, 0x69, 0x6B, 0x65, 0x5F, 0x63, 0x68, 0x6E, 0x2F, 0x00 };
    for (int i = 0; i < 13; i++)
        mem->Write<byte>(fixaddr + i, cstrike_chn[i]);
    cout << "国服nar重定向已完成\n";

    // content: chn (with 00 ends)
    char chn[4] = { 0x63, 0x68, 0x6E, 0x00 };
    for (int i=0;i<4;i++)
        mem->WriteProtected<byte>(langaddr + i, chn[i]);
    cout << "选择中国语已完成\n\n";

    DWORD resourceaddr = NULL;
    string filelist[5] = { "clrcompression.dll", "clrjit.dll", "coreclr.dll", "MemScan.exe", "mscordaccore.dll" };
    bool mem_file_check_passed = true;
    string mem_file_missing_list = "";
    for (int i = 0; i < 5; i++)
    {
        if (!Misc->FileExist(filelist[i]))
        {
            mem_file_check_passed = false;
            //cout << "缺少关键文件" << filelist[i] << "!\n";
            (mem_file_missing_list.length() == 0) ? mem_file_missing_list += filelist[i] : mem_file_missing_list += ", " + filelist[i];
        }
    }

    if (mem_file_check_passed)
    {
        // content: /cstrike_chn/resource/background/800_2_c_advice.tga (with extra 00 security verification hex to prevent error result)
        // thanks Bunsei's Memory Scan program
        string process = "MemScan.exe";
        string args = " -pid=" + to_string(mem->m_dwProcessId) + " -T=\"h\" -value=\"2F 63 73 74 72 69 6B 65 5F 63 68 6E 2F 72 65 73 6F 75 72 63 65 2F 62 61 63 6B 67 72 6F 75 6E 64 2F 38 30 30 5F 32 5F 63 5F 61 64 76 69 63 65 2E 74 67 61 00 33 00 00 00 33\"";

        // took from CSDN
        LPSTR start_str = strdup(string(process + args).c_str());
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

            if ((string)mem->Read<t>(exit_code).text == "/cstrike_chn/resource/background/800_2_c_advice.tga")
                resourceaddr = exit_code;
            else if (exit_code == 2)
                cout << "内存扫描失败!请检查fixtrike.nar!\n其应被放置在Data目录内且不得改名!\n";
            else
                cout << "内存扫描出现意外错误!\n返回数值:" << exit_code << "(" << Misc->DecimalToHex(exit_code, true) << ")\n";
        }
    }
    else
    {
        cout << "因缺少内存扫描必要文件,已跳过...\n不进行内存扫描可能会导致游戏出现问题!\n缺少文件: "<<mem_file_missing_list<<"\n";
    }

    if (resourceaddr != NULL)
    {
        // content: /cstrike_na_en/resource/bad_words.csv (with 00 ends)
        char bad_words[38] = { 0x2F, 0x63, 0x73, 0x74, 0x72, 0x69, 0x6B, 0x65, 0x5F, 0x6E, 0x61, 0x5F, 0x65, 0x6E, 0x2F, 0x72, 0x65, 0x73, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x2F, 0x62, 0x61, 0x64, 0x5F, 0x77, 0x6F, 0x72, 0x64, 0x73, 0x2E, 0x63, 0x73, 0x76, 0x00 };
        // content: /cstrike_na_en/resource/chat_filter_list.csv (with 00 ends)
        char chat_filter_list[45] = { 0x2F, 0x63, 0x73, 0x74, 0x72, 0x69, 0x6B, 0x65, 0x5F, 0x6E, 0x61, 0x5F, 0x65, 0x6E, 0x2F, 0x72, 0x65, 0x73, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x2F, 0x63, 0x68, 0x61, 0x74, 0x5F, 0x66, 0x69, 0x6C, 0x74, 0x65, 0x72, 0x5F, 0x6C, 0x69, 0x73, 0x74, 0x2E, 0x63, 0x73, 0x76, 0x00 };
        // content: /cstrike_na_en/resource/item.csv (with 00 ends)
        char item[34] = { 0x2F, 0x63, 0x73, 0x74, 0x72, 0x69, 0x6B, 0x65, 0x5F, 0x6E, 0x61, 0x5F, 0x65, 0x6E, 0x2F, 0x72, 0x65, 0x73, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x2F, 0x69, 0x74, 0x65, 0x6D, 0x2E, 0x63, 0x73, 0x76 };
        for (int i = 0; i < 2048; i++)
        {
            string filename = mem->Read<t>(resourceaddr + i * 0x40).text;
            if (filename.find("/cstrike_chn") == -1)
                break;

            if (filename == "/cstrike_chn/resource/bad_words.csv")
            {
                cout << "将bad_words.csv重定向至游戏原始内容...\n";
                for (int j = 0; j < 38; j++)
                    mem->Write<byte>(resourceaddr + i * 0x40 + j, bad_words[j]);
            }
            if (filename == "/cstrike_chn/resource/chat_filter_list.csv")
            {
                cout << "将chat_filter_list.csv重定向...\n"; // 太长了要换行破坏美观
                for (int j = 0; j < 45; j++)
                    mem->Write<byte>(resourceaddr + i * 0x40 + j, chat_filter_list[j]);
            }
            if (filename == "/cstrike_chn/resource/item.csv")
            {
                cout << "将item.csv重定向至游戏原始内容...\n";
                for (int j = 0; j < 34; j++)
                    mem->Write<byte>(resourceaddr + i * 0x40 + j, item[j]);
            }
        }
    }

    cout << "\n操作执行完毕,已加载汉化! :)\n按下任意键退出汉化程序解冻游戏进程!\n";
    system("pause");
    //Sleep(3000);

    mem->Detach();
    return 0;
}