## Counter-Strike Online (Nexon: Studio / Zombies) 汉化工具

[![](https://img.shields.io/badge/许可证-MIT-green)](./LICENSE_CN)
[![](https://img.shields.io/badge/发行版版本-1.2-green)](https://github.com/dounai2333/CSNS-Chinese-Localization/releases/tag/1.2)
[English Readme](./README.md)

一个简易的汉化程序能让CSN:S使用汉语.

**本项目当前处于[MIT协议](./LICENSE_CN)下, 由C++编写, 编译平台VS2019.**

- 随时欢迎进行二次开发或发问题贴/合并请求!

### 告示
当前1.2发行版已无法再屏蔽国服的bad_words.csv!

这意味着, 如果你的帐号/密码里有不雅词汇, 你将无法再正常登录你的帐号!

也许你会想试试[具有严重性能问题的1.3试发行版](https://github.com/dounai2333/CSNS-Chinese-Localization/releases/tag/1.3).

此外我正在寻求1.3的帮助, [点击这里发起新的合并请求](https://github.com/dounai2333/CSNS-Chinese-Localization/compare)!

### 如何使用?
1. 保证你的游戏语言为英语 (English, na_en)
2. 从反恐精英Online(国服)里提取出cstrike_chn.nar
3. 复制, 并将其粘贴到CSN:S的Data文件夹内
4. 将其改名为fixtrike.nar或替换掉cstrike_na_en.nar
5. 从反恐精英Online(国服)里提取出chn_00000.pak*
6. 复制, 并将其粘贴到CSN:S的Data\Packer文件夹内
7. 当右下角加载N社反作弊*时开启程序
8. 完事! 享受吧!

*chn_00000.pak: 别疑惑了, 你也需要其他同名文件 (chn_00001.pak, 00002等...)

*N社反作弊: 长方形的蓝框, 中间显示白字NEXON GAME SECURITY

### 如果我在Steam服里用汉化, 那我会被封号吗?
高概率**会**.

这是因为汉化程序的工作方式与其他常见的外挂无异(都具有频繁的内存读/写操作)

使用风险自负!

### 是不是游戏更新后汉化就失效了?
我不确定, 我已经尽我所能做了兼容, 所以应该可以无视游戏更新.

不过, 如果你有能力的话, 你一直都可以下载源代码, 修改内存基址再自行编译并为你自己所用!

### 我的游戏在用了这个程序后出现问题! HELP!
[点击这里](https://github.com/dounai2333/CSNS-Chinese-Localization/issues/new)发起一个新的问题帖.

**请尽可能地提供更多信息!**

**你提供的描述越多, 获得帮助的可能性也就越大!**

‮

‮

### 第三方库

- **使用了 KateBot 的 Memory 和 Misc 源代码 (作者 klorik).**

- **使用了 MemoryScan 程序 (作者 Bunsei).**
