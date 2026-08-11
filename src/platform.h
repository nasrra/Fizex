typedef struct{
    void* win32_hwnd;
    void* win32_hdc;
    void* wind32_hinstance;
} WindowInfo;

#if OS_WINDOWS
#include "app_win32.h"
#else
#   error undefined operating system.
#endif
