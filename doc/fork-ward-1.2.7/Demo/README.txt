
GNU C Compiler:

    MinGW-w64/gcc version 4.9.1

    http://mingw-w64.sourceforge.net/
    http://sourceforge.net/projects/mingw-w64/files/

Compile Command Suggestion:

    For 32 bits version:
        gcc/g++ -S -Os -masm=intel -m32 filename.c

    For 64 bits version:
        gcc/g++ -S -Os -masm=intel -m64 filename.c

License Of Source Code:

    2048.c (ez-draw.c, jeu-2048.c)

        http://pageperso.lif.univ-mrs.fr/~edouard.thiel/ez-draw/index-en.html
        Copyright (c) 2008-2014 by Edouard Thiel <Edouard.Thiel@lif.univ-mrs.fr>

    calc.c

        http://stevehanov.ca/blog/index.php?id=26
	Copyright: Steve Hanov <steve.hanov@gmail.com>

    maze.c

        http://en.wikipedia.org/wiki/User:Dllu/Maze


    miniz.c

        https://code.google.com/p/miniz/ (v1.15)
        Copyright (C) Rich Geldreich <richgel99@gmail.com>

        Modification to avoid _ftelli64/_fseeki64 issue:
        Define all MZ_FTELL64/MZ_FSEEK64 to ftell/fseek

    sha3.c

        https://github.com/rhash/RHash/blob/master/librhash/sha3.c
	Copyright: 2013 Aleksey Kravchenko <rhash.admin@gmail.com>

    tetris.c

        Copyright (C) 2014 Ward

    xxHash.c

        https://code.google.com/p/xxhash/ (r34)
        Copyright (C) 2012-2014, Yann Collet.
