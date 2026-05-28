@echo off
setlocal enabledelayedexpansion

set GLSLC=C:/VulkanSDK/1.4.328.1/Bin/glslc.exe
set SHADER_DIR=Shaders
set OUTPUT_DIR=Spv

if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%

echo.
echo ===== Compiling shaders =====
echo.

for /r %SHADER_DIR% %%f in (*.vert *.frag *.comp) do (

    rem chemin relatif : enlève SHADER_DIR
    set "REL=%%f"
    set "REL=!REL:%CD%\%SHADER_DIR%\=!"  > nul 2>&1
    rem Windows path fix : remplace \ par \ (pour batch mkdir)
    set "REL=!REL:/=\!"

    rem chemin complet de sortie
    set "OUT=%OUTPUT_DIR%\!REL!.spv"

    rem créer le dossier parent si nécessaire
    for %%d in ("!OUT!") do (
        if not exist "%%~dpd" mkdir "%%~dpd"
    )

    echo Compiling %%f
    "%GLSLC%" -I "%SHADER_DIR%" "%%f" -o "!OUT!"

)

echo.
echo ===== Done =====
pause