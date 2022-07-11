@SETLOCAL

pushd %~dp0
del /q .\build\*
mkdir .\build
pushd .\build

@SET SRC="../src/main.cpp" "../src/bytecode/chunk.cpp" "../src/bytecode/compiler.cpp" "../src/bytecode/debug.cpp" "../src/bytecode/obj_helper.cpp" "../src/bytecode/scanner.cpp" "../src/bytecode/stack.cpp" "../src/bytecode/value.cpp" "../src/bytecode/vm.cpp"

@SET OUTPUTEXE="main.exe"
@SET OUTPUTPDB="main.pdb"
@SET INSTALLDIR="../bin/"

@REM TODO(fusion): Warnings that I disabled but should eventually get fixed (?):
@REM	- C4101				unreferenced local variable

@REM NOTE(fusion): On MSVC We need -std:c++20 for designated initializers. On GCC I think this has always been a compiler extension.

@SET COMMONFLAGS=%* -Zi -MT -W3 -WX -wd4101 -std:c++20 -EHsc -MP -Gm-

cl.exe %COMMONFLAGS% -Fe%OUTPUTEXE% -Fd%OUTPUTPDB% %SRC% /link -stack:0x400000,0x100000 -subsystem:console -incremental:no -opt:ref -dynamicbase %LIBS%

IF EXIST %OUTPUTEXE% copy /Y %OUTPUTEXE% %INSTALLDIR%
IF EXIST %OUTPUTPDB% copy /Y %OUTPUTPDB% %INSTALLDIR%

popd
popd

@ENDLOCAL
