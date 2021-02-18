## Counter-Strike Online (Nexon: Studio / Zombies) 汉化工具

[![](https://img.shields.io/badge/许可证-MIT-green)](./LICENSE_CN)
[![](https://img.shields.io/badge/发行版版本-1.3.2-green)](https://github.com/dounai2333/CSNS-Chinese-Localization/releases/tag/1.3.2)
[English Readme](./README.md)

一个简易的汉化程序能让CSN:S使用汉语.

**本项目当前处于[MIT协议](./LICENSE_CN)下, 由C++编写, 编译平台VS2019.**

- 随时欢迎进行二次开发或发问题贴/合并请求!

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

### 可使用的启动项

- -dontblock : 不要尝试阻止任何文件, 让游戏可以正常读取它们.

- -oldblockmethod ([未完成](https://github.com/dounai2333/CSNS-Chinese-Localization/blob/master/README_CN.md#为什么旧的文件阻止方式未完成)) : 使用旧的文件阻止方式, 而不是用新的避免性能浪费.

- -autoexit : 当一切操作完毕后自动退出程序, 在退出之前会等待5秒钟.

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

### 为什么旧的文件阻止方式未完成?

旧的文件阻止方式严重依赖Bunsei的MemoryScan程序, 然后他的程序有BUG,

这导致我无法获取到所有我想要的内存地址.

(例如, 有2个地址, 0x10000和0x1FFFF, 程序只能抓到第1个)

如果你使用旧方式, 你会遇见2个BUG(或不应该将其称为BUG?):

1. 来自国服的bad_word.csv被加载.

用于加载屏蔽词, 示例图片: [昼夜2的模式消息有屏蔽词](https://raw.githubusercontent.com/dounai2333/CSNS-Chinese-Localization/master/images/badwordimg1.jpg), [玩家名称有屏蔽词](https://raw.githubusercontent.com/dounai2333/CSNS-Chinese-Localization/master/images/badwordimg2.jpg)

2. 来自国服的relation_product_ver2.csv被加载.

它定义了所有商城的道具, 而未在此列表的道具会被定义为"空", 示例图片: [商店](https://raw.githubusercontent.com/dounai2333/CSNS-Chinese-Localization/master/images/shopimg.jpg)

‮

‮

### 第三方库

- **使用了 KateBot 的 Memory 和 Misc 源代码 (作者 klorik).**

- **使用了 MemoryScan 程序 (作者 Bunsei).**
