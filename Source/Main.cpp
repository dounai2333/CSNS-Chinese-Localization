#include "Main.h"

int main()
{
    SetConsoleTitle("CSN:S汉化程序");
    Misc->CloseConsoleQuickEdit();
    Misc->SetConsoleSize(370, 380);
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
             <<"Made by dounai2333 (QQ1328600509)\n\n\n";
        system("pause");
        exit(1);
    }

    cout <<"cstrike-online.exe (PID: "<< mem->m_dwProcessId << ") attached.\n"
         <<"Attach handle: "<< (int)mem->m_hProcess <<"("<< Misc->DecimalToHex((int)mem->m_hProcess, true) <<")\n\n";


    // if user start program too early, we will hit exception in here.
    Module* hw = mem->GetModule("hw.dll");
    Module* filesystem = mem->GetModule("filesystem_nar.dll");
    cout << "hw\t\t- 0x" << (int*)hw->GetImage() << "\n";
    cout << "filesystem_nar\t- 0x" << (int*)filesystem->GetImage() << "\n";
    
    if (mem->GetModule("mp.dll"))
    {
        system("cls");
        cout << "游戏已完成加载!开启此程序不会生效!\n请确保在反作弊加载时再运行!\n\n";
        system("pause");
        exit(-1);
    }

    // string, original content: /cstrike_na_en/
    // if filesysyem_nar.dll get any update then we need to check this
    // address - 0x30 = /fixtrike/
    DWORD cstrike_na_en_addr = mem->Read<DWORD>(filesystem->GetImage() + 0xCC6E4) + 0x30;
    // string, original content: na_en (it's protected so we can not scan it with "writable")
    // this signature is little dangers, may it will not working anymore if has any update
    DWORD lang_addr = hw->GetImage() + find_ptr(hw, "C7 00 ? ? ? ? C7 40 04 ? ? ? ? E8 ? ? ? ? 83 C4 10 6A 00 56 57 83 EC 08 8B C4", 0x2, 0x3C);

    string cstrike_na_en_addr_text = mem->Read<str>(cstrike_na_en_addr).text;
    string lang_addr_text = mem->Read<str>(lang_addr).text;

    if (cstrike_na_en_addr_text == "/cstrike_chn/" && lang_addr_text == "chn")
    {
        system("cls");
        cout << "请勿重复操作!待游戏加载完毕后即可加载汉化\n";
        system("pause");
        exit(0);
    }
    else if (cstrike_na_en_addr_text != "/cstrike_na_en/" || lang_addr_text != "na_en")
    {
        system("cls");
        cout << "检测到非法内容,可能由于游戏更新基址已过期\n\n如果您的游戏语言不是英语,\n请修改为英语后重试!";
        system("pause");
        exit(-1);
    }

    // pause the game process so we can do the memory scan and override without trouble
    DebugActiveProcess(mem->m_dwProcessId);
    DebugSetProcessKillOnExit(false);
    cout << "\n已冻结游戏进程,正在进行操作...\n\n";
    
    mem->Write(cstrike_na_en_addr, "/cstrike_chn/");
    cout << "国服nar重定向已完成\n";

    mem->WriteProtected(lang_addr, "chn");
    cout << "选择中国语已完成\n\n";

    DWORD resource_addr = NULL;
    DWORD cstrike_na_en_packer_addr = NULL;

    string memfile_missing_list = CheckMemFile();
    if (memfile_missing_list == "")
    {
        cout << "正在执行内存扫描,请稍等...\n";
        DWORD exitcode = RunMemScanAndGetExitCode(mem->m_dwProcessId, "h", "2F 63 73 74 72 69 6B 65 5F 63 68 6E 2F 72 65 73 6F 75 72 63 65 2F 62 61 63 6B 67 72 6F 75 6E 64 2F 38 30 30 5F 32 5F 63 5F 61 64 76 69 63 65 2E 74 67 61 00 33 00 00 00 33");
        DWORD exitcode2 = RunMemScanAndGetExitCode(mem->m_dwProcessId, "s", "../Data/Packer/chn_00000.pak", "utf-16");
        if ((string)mem->Read<str>(exitcode).text == "/cstrike_chn/resource/background/800_2_c_advice.tga")
        {
            resource_addr = exitcode;
            if (exitcode2 != 2)
                cstrike_na_en_packer_addr = RunMemScanAndGetExitCode(mem->m_dwProcessId, "s", "/lstrike/locale_na_en/");
            else
                cout << "未检测到chn_00000.pak, 已跳过pak重定向...\n";
        }
        else if (exitcode == 2)
        {
            cout << "内存扫描失败!\n请检查fixtrike.nar和chn_00000.pak!\n其应分别被放置在Data和Packer内且不得改名!\n\n如果您正在尝试读取非chn的nar/pak文件,\n那么您应该对此程序进行二次开发:\ngithub.com/dounai2333\n";
        }
        else
        {
            cout << "内存扫描出现意外错误!\n返回数值:" << exitcode << "(" << Misc->DecimalToHex(exitcode, true) << ")\n";
        }
    }
    else
    {
        cout << "因缺少内存扫描必要文件,已跳过...\n不进行内存扫描可能会导致游戏出现问题!\n缺少文件: "<<memfile_missing_list<<"\n";
    }

    if (resource_addr != NULL)
    {
        int muted = 0;
        for (int i = 0; i < 2048; i++)
        {
            string filename = mem->Read<str>(resource_addr + i * 0x40).text;
            if (filename.find("/cstrike_chn") == -1)
                break;
            filename.replace(filename.begin(), filename.begin() + filename.find("/", 2) + 1, "");

            if (filename == "resource/bad_words.csv"               /*脏话屏蔽列表*/ ||
                filename == "resource/chat_filter_list.csv"        /*早期的脏话屏蔽列表*/ ||
                filename == "resource/item.csv"                    /*游戏里各种道具的定义*/ ||
                filename == "resource/relation_product_ver2.csv"   /*商城道具的定义*/ ||
                filename == "sound/training/cstrain11.wav"         /*教程关的语音提示*/ ||
                filename == "sound/training/cstrain1.wav"          /*(我要听小姐姐!!国服两声提示音爬爬爬)*/ ||
                filename == "sound/training/cstrain10.wav"         ||
                filename == "sound/training/cstrain12.wav"         ||
                filename == "sound/training/cstrain12a.wav"        ||
                filename == "sound/training/cstrain13.wav"         ||
                filename == "sound/training/cstrain15.wav"         ||
                filename == "sound/training/cstrain16.wav"         ||
                filename == "sound/training/cstrain17.wav"         ||
                filename == "sound/training/cstrain18.wav"         ||
                filename == "sound/training/cstrain19.wav"         ||
                filename == "sound/training/cstrain2.wav"          ||
                filename == "sound/training/cstrain2a.wav"         ||
                filename == "sound/training/cstrain3.wav"          ||
                filename == "sound/training/cstrain3a.wav"         ||
                filename == "sound/training/cstrain4.wav"          ||
                filename == "sound/training/cstrain5.wav"          ||
                filename == "sound/training/cstrain6.wav"          ||
                filename == "sound/training/cstrain7.wav"          ||
                filename == "sound/training/cstrain7a.wav"         ||
                filename == "sound/training/cstrain8.wav"          ||
                filename == "sound/training/cstrain9.wav"          ||
                filename == "sound/training/cstrain9a.wav"         ||
                false /* change the list here! */)
            {
                muted++;
                mem->Write(resource_addr + i * 0x40, "null");
            }
        }

        if (cstrike_na_en_packer_addr != NULL && cstrike_na_en_packer_addr != 2)
        {
            mem->Write(cstrike_na_en_packer_addr, "/lstrike/locale_chn/");
            cout << "国服pak重定向已完成\n";

            DWORD item = RunMemScanAndGetExitCode(mem->m_dwProcessId, "s", "lstrike/locale_chn/resource/item.csv", "utf-16");
            /* // backup code if Item.csv has been renamed to item.csv
            DWORD addresses[128];
            RunMemScanAndGetAllAddress(mem->m_dwProcessId, "s", "lstrike/locale_chn/resource/item.csv", addresses, "utf-16");
            DWORD item = addresses[0];
            */
            if (item != 2)
            {
                mem->Write(item, L"null");
                muted++;

                item = item - 0x58 * mem->Read<byte>(item - 3);
                for (int i = 0; i < 0xFF; i++)
                {
                    wstring temp1(mem->Read<bigstr>(item + 0x58 * i).text);
                    // not safe, will lost data if any word is a non-ascii character, find a better way to do this!
                    string filename(temp1.begin(), temp1.end());
                    if (filename.find("lstrike") == -1)
                        continue; // we don't break because some info is encrypted and it will cause problem
                    filename.replace(filename.begin(), filename.begin() + filename.find("/", 9) + 1, "");
                    
                    if (filename == "resource/item.csv")
                    {
                        mem->Write(item + 0x58 * i, L"null");
                        muted++;
                        break; // do not mute other string, can cause crash
                    }
                }

                // todo: add bad_words.csv, chat_filter_list.csv and relation_product_ver2.csv to mute list.
            }
        }
        
        cout << "已屏蔽不应该被加载的 " << muted << " 个文件.\n";
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

DWORD RunMemScanAndGetExitCode(DWORD ProcessID, string Type, string Value, string Encoding)
{
    string strtemp = "MemScan.exe -pid=" + to_string(ProcessID) + " -t=" + Type + " -value=\"" + Value + "\" " + ((Type == "s") ? ("-encoding=" + Encoding) : "");
    LPSTR start_str = strdup(strtemp.c_str());
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

void RunMemScanAndGetAllAddress(DWORD ProcessID, string Type, string Value, DWORD(&output)[128], string Encoding)
{
    string strtemp = "MemScan.exe -pid=" + to_string(ProcessID) + " -t=" + Type + " -value=\"" + Value + "\" " + ((Type == "s") ? ("-encoding=" + Encoding) : "") + " >%temp%\\address.log";
    system(strtemp.c_str());

    ifstream file(string(getenv("TEMP")) + "\\address.log");
    if (file.good())
    {
        int index = 0;
        for (std::string line; getline(file, line); )
        {
            output[index] = Misc->HexToDecimal(line);
            index++;
        }
        file.close();
        if (output[0] != NULL)
        {
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
            string strtemp1 = "cmd /c \"del /s /q %temp%\\address.log\"";
            LPSTR start_str = strdup(strtemp1.c_str());
            if (CreateProcess(NULL, start_str, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
            {
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        }
    }
}