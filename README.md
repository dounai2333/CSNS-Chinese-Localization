## Counter-Strike Online (Nexon: Studio / Zombies) Chinese Localization

[![](https://img.shields.io/badge/License-MIT-green)](./LICENSE)
[![](https://img.shields.io/badge/Release%20Version-1.3.2-green)](https://github.com/dounai2333/CSNS-Chinese-Localization/releases/tag/1.3.2)
[Chinese Readme | 中文说明文档](./README_CN.md)

A localization program that can let CSN:S load the Chinese.

**This project is current under [MIT license](./LICENSE) and written in C++, build with VS2019.**

- Secondary development and issues / pull requests are always welcome!

### How to use?
1. Make sure your game language is English
2. Get cstrike_chn.nar from CSO China client
3. Copy it and paste to CSN:S Data folder
4. Rename it to fixtrike.nar or replace cstrike_na_en.nar
5. Get chn_00000.pak* from CSO China client
6. Copy it and paste to CSN:S Data\Packer folder
7. Start the program when Nexon Anti-Cheat* is loading
8. We are done! Enjoy it!

*chn_00000.pak: Don't doubt, you also need other files as this (chn_00001.pak, 00002 etc..)

*Nexon Anti-Cheat: In the lower right of your screen with blue background, showing white text NEXON GAME SECURITY

### Available arguments

- -dontblock : Don't try to block any file, let game load them normally.

- -oldblockmethod ([Unfinished](https://github.com/dounai2333/CSNS-Chinese-Localization#why-old-block-method-is-unfinished)) : Use old block method instead of new one, save a lot performance.

- -autoexit : Automatically exit the program when everything is done, will wait 5 seconds before exiting.

### Will I be banned for using this on Steam server?
Will, most likely **YES**.

Thats because this program is working like other normal cheating software (memory reading / writing)

USE AT YOUR OWN RISK!

### Is this program will stop working after game updates?
I'm not sure, I have do it as much as possible to compatible with game update.

But still, you can always download the source code, update the address and build for yourself!

### My game has problem with this program! help!
Start a [new issue thread here](https://github.com/dounai2333/CSNS-Chinese-Localization/issues/new).

**Please give as much detail as possible!**

**The more detail you gave, the more possibility helps!**

‮

### Why old block method is unfinished?

The old blocking method relies heavily Bunsei's MemoryScan program, but his program has bugs,

that cause i cannot have all the memory address i want to get.

(For example, there have 2 addresses, 0x10000 and 0x1FFFF, the program can only catch the first one)

There has 2 bugs (or it shouldn't called as bug?) after you use old method:

1. bad_words.csv has been loaded from China client.

Function as what it called, Example images: [ZShelter gamemode message has bad word](https://raw.githubusercontent.com/dounai2333/CSNS-Chinese-Localization/master/images/badwordimg1.jpg), [player name has bad word](https://raw.githubusercontent.com/dounai2333/CSNS-Chinese-Localization/master/images/badwordimg2.jpg)

2. relation_product_ver2.csv has been loaded from China client.

It's shop items list, any item not on this list will be "null" in the shop, example images: [shop](https://raw.githubusercontent.com/dounai2333/CSNS-Chinese-Localization/master/images/shopimg.jpg)

‮

‮

### Third party library

- **Used KateBot's Memory and Misc code (Author klorik).**

- **Used MemoryScan program (Author Bunsei).**
