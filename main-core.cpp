
#include <mi/base.h>
#include <mi/mdl/mdl.h>

#ifndef MI_PLATFORM_WINDOWS
#include <dlfcn.h>
#include <unistd.h>
#include <dirent.h>
#endif

#ifdef MI_PLATFORM_MACOSX
#include <mach-o/dyld.h>   // _NSGetExecutablePath
#endif

#include <cstdio>
#include <cstdlib>
#include <cassert>

// printf() format specifier for arguments of type LPTSTR (Windows only).
#ifdef MI_PLATFORM_WINDOWS
#ifdef UNICODE
#define FMT_LPTSTR "%ls"
#else // UNICODE
#define FMT_LPTSTR "%s"
#endif // UNICODE
#endif // MI_PLATFORM_WINDOWS

int main(int argc, char* argv[])
{
#ifdef MI_PLATFORM_WINDOWS
    void* handle = LoadLibraryA((LPSTR) MDL_CORE_LIBRARY);
    if (!handle) {
        LPTSTR buffer = 0;
        LPCTSTR message = TEXT("unknown failure");
        DWORD error_code = GetLastError();
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS, 0, error_code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &buffer, 0, 0))
            message = buffer;
        fprintf(stderr, "Failed to load library (%u): " FMT_LPTSTR, error_code, message);
        if (buffer)
            LocalFree(buffer);
        return -1;
    }
    void* symbol = GetProcAddress((HMODULE) handle, "mi_mdl_factory");
    if (!symbol) {
        LPTSTR buffer = 0;
        LPCTSTR message = TEXT("unknown failure");
        DWORD error_code = GetLastError();
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS, 0, error_code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &buffer, 0, 0))
            message = buffer;
        fprintf(stderr, "GetProcAddress error (%u): " FMT_LPTSTR, error_code, message);
        if (buffer)
            LocalFree(buffer);
        return -1;
    }
#else // MI_PLATFORM_WINDOWS
    void* handle = dlopen(MDL_CORE_LIBRARY, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }
    void* symbol = dlsym(handle, "mi_mdl_factory");
    if (!symbol) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }
#endif // MI_PLATFORM_WINDOWS

    {
        using IMDL_factory = mi::mdl::IMDL *(*)(mi::base::IAllocator *);

        mi::base::Handle<mi::mdl::IMDL> mdl_compiler(reinterpret_cast<IMDL_factory>(symbol)(nullptr));
        assert(mdl_compiler);
        
    }

#ifdef MI_PLATFORM_WINDOWS
    int result = FreeLibrary((HMODULE)handle);
    if (result == 0) {
        LPTSTR buffer = 0;
        LPCTSTR message = TEXT("unknown failure");
        DWORD error_code = GetLastError();
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS, 0, error_code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &buffer, 0, 0))
            message = buffer;
        fprintf(stderr, "Failed to unload library (%u): " FMT_LPTSTR, error_code, message);
        if (buffer)
            LocalFree(buffer);
        return -2;
    }
#else
    int result = dlclose(handle);
    if (result != 0) {
        printf("%s\n", dlerror());
        return -2;
    }
#endif

    return 0;
}
    