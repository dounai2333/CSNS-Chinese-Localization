## Counter-Strike Online (Nexon: Studio / Zombies) 汉化工具

[![](https://img.shields.io/badge/许可证-MIT-green)](./LICENSE_CN)
[![](https://img.shields.io/badge/发行版版本-1.3.4修复版-green)](https://github.com/dounai2333/CSNS-Chinese-Localization/releases/tag/1.3.4)
[English Readme](./README.md)

一个简易的汉化程序能让CSN:S使用汉语.

**本项目当前处于[MIT协议](./LICENSE_CN)下, 由C++编写, 编译平台VS2019.**

- 随时欢迎进行二次开发或发问题贴/合并请求!

### 此项目现已停止维护.
我将不会再提供支持或维护此项目, 你仍然可以阅读代码或尝试使用它, 但如果它出现问题, 请不要抱怨.

### 如何使用?
1. 确保你的游戏语言为英语 (English, na_en)
2. 从反恐精英Online(国服)里提取出cstrike_chn.nar
3. 复制, 并将其粘贴到CSN:S的Data文件夹内
4. 将其改名为fixtrike.nar或替换掉cstrike_na_en.nar
5. 从反恐精英Online(国服)里提取出chn_00000.pak*
6. 复制, 并将其粘贴到CSN:S的Data\Packer文件夹内
7. 当右下角加载N社反作弊*时开启程序
8. 完事! 享受吧!

*chn_00000.pak: 别疑惑了, 你也需要其他同名文件 (chn_00001.pak, 00002等...)

*N社反作弊: 长方形的蓝框, 中间显示白字NEXON GAME SECURITY

### 可使用的启动项

- -killac : 关闭反作弊进程, 可使游戏加载速度稍快一点. (如果你的NGS经常卡住则很有效)

- -dontblock : 不要尝试阻止任何文件, 让游戏可以正常读取它们.

- -autoexit : 当一切操作完毕后自动退出程序, 在退出之前会等待5秒钟.

### 如果我在Steam服里用汉化, 那我会被封号吗?
高概率**会**.

这是因为汉化程序的工作方式与其他常见的外挂无异(都具有频繁的内存读/写操作)

使用风险自负!

‮

‮

### 第三方库

- **使用了 KateBot 的 Memory 和 Misc 源代码 (作者 klorik).**

- **使用了 MemoryScan 程序 (作者 Bunsei).**
