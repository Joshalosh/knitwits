@echo off


set CompilerFlags= /Zi /MDd /FC /nologo
REM set LinkerFlags=-subsystem:Console

IF NOT EXIST build mkdir build

pushd build

cl /c /D PLATFORM_DESKTOP /I..\external\Raylib\external\glfw\include ^
    ..\external\Raylib\core.c ^
    ..\external\Raylib\models.c ^
    ..\external\Raylib\raudio.c ^
    ..\external\Raylib\rglfw.c ^
    ..\external\Raylib\shapes.c ^
    ..\external\Raylib\text.c ^
    ..\external\Raylib\textures.c ^
    ..\external\Raylib\utils.c

cl %CompilerFlags% ^
    ..\main.cpp ^
    core.obj ^
    models.obj ^
    raudio.obj ^
    rglfw.obj ^
    shapes.obj ^
    text.obj ^
    textures.obj ^
    utils.obj ^
    /I ..\include/ /link /FORCE:MULTIPLE -incremental:no %LinkerFlags% -out:generator.exe ^
    Gdi32.lib ^
    winmm.lib ^
    user32.lib ^
    shell32.lib

popd
