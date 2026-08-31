@REM
@REM     For win32 platforrm layer.
@REM     - user32.lib
@REM     - Gdi32.lib
@REM     For wgpu_native.lib as it is rust-lang bindings, and rust contains those libs in its standard lib.
@REM     - ws2_32.lib
@REM     - ntdll.lib
@REM     - userenv.lib
@REM

mkdir build & cd src & cl /Zi /W3 /WX /Fo:..\build\ /Fd:..\build\ /Fe:..\build\app.exe app_win32.c app.c base_layer\base.c user32.lib Gdi32.lib ws2_32.lib ntdll.lib userenv.lib vendors\webgpu\wgpu_native.lib
@REM & call cd ..\build & app.exe