@REM
@REM     For win32 platforrm layer.
@REM     - user32.lib
@REM     - Gdi32.lib
@REM     For wgpu_native.lib as it is rust-lang bindings, and rust contains those libs in its standard lib.
@REM     - ws2_32.lib
@REM     - ntdll.lib
@REM     - userenv.lib
@REM
@REM     xcopy doc:
@REM
@REM     /E copies all sub directories, including empty ones.
@REM     /I assumes destination is a directory (avoids a prompt whne the destination doesn't exist).
@REM     /Y suppresses "overwrite" confirmation prompts.
@REM    
@REM    .bat doc:
@REM    `for` has several modes, chosen by the switch you give it:
@REM    
@REM    for %f in (...) : plain loop over a set of items you list
@REM    for /r : recursive, walks a directory tree and matches files in every folder
@REM    for /d : loops over directories (folders) instead of files
@REM    for /l : counts through a list of numbers (start, step, end)
@REM    for /f : parses the contents of a file, a string, or a command’s output line by line
@REM
@REM    %%f, the dual percent means to literally expand to '%f' rather than what %f is.

echo %CL%
mkdir build\app & 
for /r "src\vendors" %%f in (*.dll) do copy "%%f" "build\app\" & 
cd src & 
cl /Zi /W3 /WX ^
/I vendors\freetype ^
/Fo:..\build\app\ /Fd:..\build\app\ /Fe:..\build\app\app.exe ^
app_win32.c app.c base_layer\base.c input/input.c ^
Kernel32.lib user32.lib Gdi32.lib ws2_32.lib ntdll.lib userenv.lib vendors\webgpu\wgpu_native.lib vendors\freetype\freetype-x64.lib & ^
xcopy /E /I /Y "..\assets" "..\build\assets"

@rem  mkdir build & cd src & cl /O2 /Oi /GL /DNDEBUG /MD /W3 /WX /I vendors\freetype /Fo:..\build\ /Fd:..\build\ /Fe:..\build\app.exe app_win32.c app.c base_layer\base.c input/input.c user32.lib Gdi32.lib ws2_32.lib ntdll.lib userenv.lib vendors\webgpu\wgpu_native.lib vendors\freetype\freetype-x64.lib & xcopy /E /I /Y "..\assets" "..\build\assets"

@REM mkdir build & cd src & cl /O2 /Oi /GL /DNDEBUG /MD /W3 /WX
@REM  & call cd ..\build & app.exe