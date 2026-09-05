rem
rem     For win32 platforrm layer.
rem     - user32.lib
rem     - Gdi32.lib
rem     For wgpu_native.lib as it is rust-lang bindings, and rust contains those libs in its standard lib.
rem     - ws2_32.lib
rem     - ntdll.lib
rem     - userenv.lib
rem
rem     xcopy doc:
rem     
rem     /E copies all sub directories, including empty ones.
rem     /I assumes destination is a directory (avoids a prompt whne the destination doesn't exist).
rem     /Y suppresses "overwrite" confirmation prompts.
rem  

mkdir build & cd src & cl /Zi /W3 /WX /Fo:..\build\ /Fd:..\build\ /Fe:..\build\app.exe app_win32.c app.c base_layer\base.c user32.lib Gdi32.lib ws2_32.lib ntdll.lib userenv.lib vendors\webgpu\wgpu_native.lib & xcopy /E /I /Y "..\assets" "..\build\assets"
rem & call cd ..\build & app.exe