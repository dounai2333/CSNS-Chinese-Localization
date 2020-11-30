#include "Main.h"

using namespace std;
using std::cout;
using std::string;
using std::thread;

struct t
{
    char text[128];
};

Module* hw;
Module* filesystem;

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
        cout << "cstrike-online.exe 未启动...\n请在右下角加载N社反作弊时开启此程序\n\n以下是使用须知和温馨提醒：\n- 请确保带启动项\"-disableauthui\"\n--推荐同时使用-login和-password\n-- -login 你的帐号 -password 你的密码\n- 不保证不会出现程序BUG\n- 不保证游戏更新后仍可用\n- 如提示\"error TS6,1 (GetLastError 0x20)\"\n-- 无视并点击确定即可，提示2次后正常游戏\nMade by dounai2333 (QQ1328600509)\n\n";
        system("pause");
        exit(1);
    }

    //cout << "等待 cstrike-online.exe 启动中......\n以下是使用须知和温馨提醒：\n- 请确保带启动项\"-disableauthui\"\n- 不保证不会出现程序BUG\n- 不保证游戏更新后仍可用\n- 如提示\"error TS6,1 (GetLastError 0x20)\"\n-- 无视并点击确定即可，提示2次后正常游戏\nMade by dounai2333 (QQ1328600509)\n\n";

    //while (!mem->Attach("cstrike-online.exe"))
        //Sleep(100);
    cout << "cstrike-online.exe (PID: "<< mem->m_dwProcessId << ") attached.\nAttach handle: "<< Misc->DecimalToHex((int)mem->m_hProcess, true)<<"\n\n";

    while (!mem->GetModule("hw.dll") && !mem->GetModule("filesystem_nar.dll"))
    {
        CloseHandle(mem->m_hProcess);
        Sleep(5);
        mem->Attach("cstrike-online.exe");
    }
    hw = mem->GetModule("hw.dll");
    filesystem = mem->GetModule("filesystem_nar.dll");
    cout << "hw \t\t- 0x" << (int*)hw->GetImage() << "\n";
    cout << "filesystem_nar\t- 0x" << (int*)filesystem->GetImage() << "\n";
    
    if (mem->GetModule("mp.dll"))
    {
        system("cls");
        cout << "游戏已完成加载！开启此程序不会生效！\n请确保在游戏运行前就打开本软件！\n";
        cout << "游戏已完成加载！开启此程序不会生效！\n请确保在游戏运行前就打开本软件！\n";
        cout << "游戏已完成加载！开启此程序不会生效！\n请确保在游戏运行前就打开本软件！\n";
        system("pause");
        exit(-1);
    }

    DebugActiveProcess(mem->m_dwProcessId);
    DebugSetProcessKillOnExit(false);

    t lan = mem->Read<t>(hw->GetImage() + 0x870DE8);
    t fix = mem->Read<t>(mem->Read<DWORD>(filesystem->GetImage() + 0xCC6E4) + 1);
    while ((string)fix.text == "")
    {
        Sleep(5);
        fix = mem->Read<t>(mem->Read<DWORD>(filesystem->GetImage() + 0xCC6E4) + 1);
    }

    if ((string)fix.text == "cstrike_chn/" || (string)lan.text == "chn")
    {
        system("cls");
        cout << "请勿重复操作!待游戏加载完毕后即可加载汉化\n";
        system("pause");
        exit(0);
    }
    else if ((string)fix.text != "fixtrike/" || (string)lan.text != "na_en")
    {
        system("cls");
        cout << "检测到非法内容,可能由于游戏更新基址已过期\n"; // \n或者仅仅是你手速太快或者慢了 :P\n都说了叫你右下角加载N社反作弊时开程序了:P\n
        system("pause");
        exit(-1);
    }

    cout << "\n已冻结游戏进程,正在进行操作...\n\n";
    
    char cstrike_chn[13] = { 0x63, 0x73, 0x74, 0x72, 0x69, 0x6B, 0x65, 0x5F, 0x63, 0x68, 0x6E, 0x2F, 0x00 };
    for (int i = 0; i < 13; i++)
        mem->Write<byte>(mem->Read<DWORD>(filesystem->GetImage() + 0xCC6E4) + 1 + i, cstrike_chn[i]);
    cout << "国服nar重定向已完成\n";

    char chn[4] = { 0x63, 0x68, 0x6E, 0x00 };
    for (int i=0;i<4;i++)
        mem->WriteProtected<byte>(hw->GetImage() + 0x870DE8 + i, chn[i]);
    cout << "选择国语已完成\n\n";
    
    // win10, 3 ways (looks it only works for myself)
    // mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(hw->GetImage() + 0x30AC63C) + 0xBC) + 0x584) + 0x8) + 0x174) + 0xC;
    // mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->GetModule("libcef.dll")->GetImage() + 0xB97DE0) + 0x4D4) + 0x584) + 0x8) + 0x174) + 0xC;
    // mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->GetModule("cstrike-online.exe")->GetImage() + 0x27DD59C) + 0x1D4) + 0x584) + 0x8) + 0x174) + 0xC;
    DWORD itemaddr = NULL;//mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(hw->GetImage() + 0x30AC63C) + 0xBC) + 0x584) + 0x8) + 0x174) + 0xC;
    
    string processname = ".\\MemScan.exe";
    string process = ".\\MemScan.exe -pid=" + to_string(mem->m_dwProcessId) + " -T=\"h\" -value=\"2F 63 73 74 72 69 6B 65 5F 63 68 6E 2F 72 65 73 6F 75 72 63 65 2F 62 61 63 6B 67 72 6F 75 6E 64 2F 38 30 30 5F 32 5F 63 5F 61 64 76 69 63 65 2E 74 67 61 00 33 00 00 00 33\"";
    LPSTR str = strdup(process.c_str());

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD exit_code;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    ifstream f(processname.c_str());
    if (f.good())
    {
        f.close();
        if (CreateProcess(NULL, str, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi))
        {
            WaitForSingleObject(pi.hProcess, INFINITE);
            GetExitCodeProcess(pi.hProcess, &exit_code);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            if ((string)mem->Read<t>(exit_code).text == "/cstrike_chn/resource/background/800_2_c_advice.tga")
                itemaddr = exit_code;
            else
                cout << "内存扫描出现意外错误!\n返回数值:" << exit_code<<"("<<Misc->DecimalToHex(exit_code, true) << ")\n";
        }
    }
    else
    {
        cout << "未找到MemScan.exe!跳过内存扫描...\n";
    }

    /*
    if (itemaddr == NULL || (string)mem->Read<t>(itemaddr).text != "/cstrike_chn/resource/background/800_2_c_advice.tga")
    {
        ifstream input(".\\Chinese.ini");
        if (!input.is_open())
        {
            cout << "扫描额外补丁失败,无法对item.csv等进行修补!\n请根据教程文本自行添加基址...\n";
        }
        else
        {
            string arrays[512];
            for (int i = 0; i < 512; i++)
                arrays[0] = "";
            int num = 0;
            for (std::string line; getline(input, line); )
            {
                arrays[num] = line;
                num++;
            }
            input.close();
            
            DWORD modulea = arrays[0] == "" ? 0xFFFFFFFF : mem->GetModule(arrays[0])->GetImage();
            DWORD address = arrays[1] == "" ? 0xFFFFFFFF : Misc->HexToDecimal(arrays[1]);
            DWORD o1 = arrays[2] == "" ? 0xFFFFFFFF : Misc->HexToDecimal(arrays[2]);
            DWORD o2 = arrays[3] == "" ? 0xFFFFFFFF : Misc->HexToDecimal(arrays[3]);
            DWORD o3 = arrays[4] == "" ? 0xFFFFFFFF : Misc->HexToDecimal(arrays[4]);
            DWORD o4 = arrays[5] == "" ? 0xFFFFFFFF : Misc->HexToDecimal(arrays[5]);
            DWORD o5 = arrays[6] == "" ? 0xFFFFFFFF : Misc->HexToDecimal(arrays[6]);

            if (modulea == 0xFFFFFFFF)
                cout << "\"Chinese.ini\"第1行扫描失败,未找到目标dll\n";
            else if (address == 0xFFFFFFFF)
                cout << "\"Chinese.ini\"第2行扫描失败,未获取到地址\n";
            else if (o1 == 0xFFFFFFFF)
                cout << "\"Chinese.ini\"第3行扫描失败,未获取到地址\n";
            else if (o2 == 0xFFFFFFFF)
                cout << "\"Chinese.ini\"第4行扫描失败,未获取到地址\n";
            else if (o3 == 0xFFFFFFFF)
                cout << "\"Chinese.ini\"第5行扫描失败,未获取到地址\n";
            else if (o4 == 0xFFFFFFFF)
                cout << "\"Chinese.ini\"第6行扫描失败,未获取到地址\n";
            else if (o5 == 0xFFFFFFFF)
                cout << "\"Chinese.ini\"第7行扫描失败,未获取到地址\n";
            else
                itemaddr = mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(mem->Read<DWORD>(modulea + address) + o1) + o2) + o3) + o4) + o5;

            if (itemaddr == NULL || (string)mem->Read<t>(itemaddr).text != "/cstrike_chn/resource/background/800_2_c_advice.tga")
                cout << "\"Chinese.ini\"基址读取失败,请进行检查!\n";
        }
    }
    */
    if (itemaddr != NULL)
    {
        char oitem[34] = { 0x2F, 0x63, 0x73, 0x74, 0x72, 0x69, 0x6B, 0x65, 0x5F, 0x6E, 0x61, 0x5F, 0x65, 0x6E, 0x2F, 0x72, 0x65, 0x73, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x2F, 0x69, 0x74, 0x65, 0x6D, 0x2E, 0x63, 0x73, 0x76 };
        char badword[38] = { 0x2F, 0x63, 0x73, 0x74, 0x72, 0x69, 0x6B, 0x65, 0x5F, 0x6E, 0x61, 0x5F, 0x65, 0x6E, 0x2F, 0x72, 0x65, 0x73, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x2F, 0x62, 0x61, 0x64, 0x5F, 0x77, 0x6F, 0x72, 0x64, 0x73, 0x2E, 0x63, 0x73, 0x76, 0x00 };
        char filter[45] = { 0x2F, 0x63, 0x73, 0x74, 0x72, 0x69, 0x6B, 0x65, 0x5F, 0x6E, 0x61, 0x5F, 0x65, 0x6E, 0x2F, 0x72, 0x65, 0x73, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x2F, 0x63, 0x68, 0x61, 0x74, 0x5F, 0x66, 0x69, 0x6C, 0x74, 0x65, 0x72, 0x5F, 0x6C, 0x69, 0x73, 0x74, 0x2E, 0x63, 0x73, 0x76, 0x00 };
        for (int i = 0; i < 2048; i++)
        {
            string temp1 = mem->Read<t>(itemaddr + i * 0x40).text;
            if (temp1.find("/cstrike_chn") == -1)
                break;

            if (temp1 == "/cstrike_chn/resource/bad_words.csv")
            {
                cout << "bad_words.csv屏蔽已完成\n";
                for (int j = 0; j < 38; j++)
                    mem->Write<byte>(itemaddr + i * 0x40 + j, badword[j]);
            }
            if (temp1 == "/cstrike_chn/resource/chat_filter_list.csv")
            {
                cout << "chat_filter_list.csv屏蔽已完成\n";
                for (int j = 0; j < 45; j++)
                    mem->Write<byte>(itemaddr + i * 0x40 + j, filter[j]);
            }
            if (temp1 == "/cstrike_chn/resource/item.csv")
            {
                cout << "item.csv修复已完成\n";
                for (int j = 0; j < 34; j++)
                    mem->Write<byte>(itemaddr + i * 0x40 + j, oitem[j]);
            }
        }
    }


    cout << "\n操作执行完毕,已加载汉化! :)\n按下任意键退出汉化程序解冻游戏进程!\n";
    system("pause");
    //Sleep(3000);

    mem->Detach();
    return 0;
}