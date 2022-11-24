AGS-wrapper-binary-call
=======================

> [AutoIt Gui Skeleton](https://autoit-gui-skeleton.github.io/) package for wrapping the library [BinaryCall](https://www.autoitscript.com/forum/topic/162366-binarycall-udf-write-subroutines-in-c-call-in-autoit/) created by [Ward's](https://www.autoitscript.com/forum/profile/10768-ward/). See this package on [npmjs.com](https://www.npmjs.com/package/@autoit-gui-skeleton/ags-wrapper-binary-call)



<br/>

## How to install AGS-wrapper-binary-call ?

We assume that you have already install [Node.js](https://nodejs.org/) and [Yarn](https://yarnpkg.com/lang/en/), for example by taking a [Chocolatey](https://chocolatey.org/). AGS framework use it for manage dependencies.

To add this package into your AutoIt project, just type in the root folder of your AGS project where the `package.json` is stored. You can also modify the `dependencies` property of this json file and use the yarn [install](https://yarnpkg.com/en/docs/usage) command. It is easier to use the add command :

```
λ  yarn add @autoit-gui-skeleton/ags-wrapper-binary-call --modules-folder vendor
```

The property `dependencies` of the  `package.json` file is updated consequently, and all package dependencies, as well as daughter dependencies of parent dependencies, are installed in the `./vendor/@autoit-gui-skeleton/` directory.

Finally to use this library in your AutoIt program, you need to include this library in the main program. There is no need for additional configuration to use it.

```autoit
#include './vendor/@autoit-gui-skeleton/ags-wrapper-binary-call/BinaryCall.au3'
```



<br/>

## What is AGS (AutoIt Gui Skeleton) ?

[AutoIt Gui Skeleton](https://autoit-gui-skeleton.github.io/) give an environment for developers, that makes it easy to build AutoIt applications. To do this AGS proposes to use conventions and a standardized architecture in order to simplify the code organization, and thus its maintainability. It also gives tools to help developers in recurring tasks specific to software engineering.

> More information about [AGS framework](https://autoit-gui-skeleton.github.io/)

AGS provides a dependency manager for AutoIt library. It uses the Node.js ecosystem and its dependency manager npm and its evolution Yarn. All AGS packages are hosted in npmjs.org repository belong to the [@autoit-gui-skeleton](https://www.npmjs.com/search?q=autoit-gui-skeleton) organization. And in AGS you can find two types of package :

- An **AGS-component** is an AutoIt library, that you can easy use in your AutoIt project built with the AGS framework. It provides some features for application that can be implement without painless.
- An **AGS-wrapper** is a simple wrapper for an another library created by another team/developer.

> More information about [dependency manager for AutoIt in AGS](https://autoit-gui-skeleton.github.io//2018/07/10/ags_dependencies_manager_for_AutoIt.html)



<br/>

## BinaryCall : Write subroutines in C, call in AutoIt


> According to the ward's documentation for BinaryCall, see https://www.autoitscript.com/forum/topic/162366-binarycall-udf-write-subroutines-in-c-call-in-autoit/


### BinaryCall UDF - Write Subroutines In C, Call In AutoIt

I have wrote a lot of binary code library for AutoIt before. I also discover
many ways to generate binary code for AutoIt in the past. However, all of them
have limitation or need some extra effort.

Recently, I think I found the best and easiest way to generate the binary code.
So I wrote this UDF, may be my last one about binary code.


The Features:
    * Both AutoIt x86 and x64 version are supported.
    * Windows API and static variables can be use (code relocation supported).
    * Decompression at run-time with smallest footprint LZMA decoder.
    * Allocated memory blocks are released automatically.
    * Most C source code works without modification.
    * Two step or one step script generation, very easy to use.


How It Works:
    1. The C source code must be compiled by MinGW GCC with "-S -masm=intel"
       option. Output is GAS syntax assembly file.

    2. BinaryCall Tool is able to convert the GAS syntax assembly file (*.s)
       to FASM syntax (*.asm). During the conversion, global symbols will be
       stored as "Symbol Jump Table" at the head of the file. The output file
       should be able to be assembled to binary file under command line by
       FASM.EXE. This syntax conversion is step 1.

    3. The step 2 is to assemble the file. BinaryCall Tool will use the
       embedded FASM to assemble every file twice to generate the relocation
       table. "BinaryCall.inc" will be included automatically before
       assembling to detect the Windows API and generate the "API Jump table".
       All the results will be compressed and converted to AutoIt script output.

    4. There are two major functions in the output script. _BinaryCall_Create()
       function allocates memorys, decompress the binary, relocates the address
       in memory, and fills the "API Jump Table".

    5. _BinaryCall_SymbolList() converts the "Symbol Jump Table" to memory
       addresses, and then store them as pointers in a DllStruct variable.

    6. Finally, we can use DllCallAddress() to call the memory address stored
       in the DllStruct.


Step by Step Tutorial:

    1. Write C source code:

           #include <windows.h>
           void main()
           {
               MessageBox(0, "Hello", "Welcome Message", 1);
           }

    2. Use GCC MinGW 32/64 to compile the source code:

           gcc -S -masm=intel32 -m32 MessageBox.c

    3. Use BinaryCall Tool "GAS2AU3 Converter", select "MessageBox.s":

           If Not @AutoItX64 Then
               Local $Code = '...'
               Local $Reloc = '...'
               Local $Symbol[] = ["main"]

               Local $CodeBase = _BinaryCall_Create($Code, $Reloc)
               If @Error Then Exit

               Local $SymbolList = _BinaryCall_SymbolList($CodeBase, $Symbol)
               If @Error Then Exit
           EndIf

    4. Paste the output script, call the main() in AutoIt:

           #Include "BinaryCall.au3"

           ; Paste output here

           DllCallAddress("none:cdecl", DllStructGetData($SymbolList, "main"))

    5. Try to run it!


Change Log:

    v1.0
        * Initial release.

    v1.1
        * A lot of improvement for GAS2ASM converter and FASM header file.
        * Add many C Run-Time library as inline asm subroutines.
        * Add command-line to argc/argv parser for easy calling main() function.
        * Add ability to redirect stdio.

          More C source code can work without modification in this version.
          Following open source projects are tested.
          And Yes, they can run as binary code library in AutoIt now.

          SQLite 3.8.5
          TCC 0.9.26
          PuTTY beta 0.63

    v1.2
        * Dynamic-link library (DLL) calling is supported now.
          If the C program requires a DLL file to run, just put it together
	  with the source file. BinaryCall Tool will searches *.dll and exports
	  all the symbols in these DLL files automatically. Of course, you need
	  these DLL files when run the output script. However, it also works if
	  you loaded them by last version of MemoryDll UDF.

        * To add more Windows API library easily by editing the ini file.
        * Better error handling and more error messages in output script.
        * Add zero padding to avoid short jumps that crash the relocation table.
        * BinaryCall Tool accepts drag and drop files now.
        * Some small bug fixed.

2015.1.21
Ward


<br/>

## About

### Acknowledgments

Acknowledgments for [Ward's](https://www.autoitscript.com/forum/profile/10768-ward/) work and its library [BinaryCall.au3](https://www.autoitscript.com/forum/topic/162366-binarycall-udf-write-subroutines-in-c-call-in-autoit/)


### Contributing

Comments, pull-request & stars are always welcome !

### License

Copyright (c) 2018 by [v20100v](https://github.com/v20100v). Released under the MIT license.
