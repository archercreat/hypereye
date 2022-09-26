#include "atexit.hpp"

#include <ntddk.h>

static PDRIVER_UNLOAD unload = nullptr;

NTSTATUS DriverMain(PDRIVER_OBJECT, PUNICODE_STRING);

extern "C"
NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING regpath)
{
    if (doinit())
        return STATUS_APP_INIT_FAILURE;

    auto status = DriverMain(driver, regpath);

    if (NT_SUCCESS(status))
    {
        // Set user unload function.
        //
        unload = driver->DriverUnload;
        // Hook unload with crt function.
        //
        driver->DriverUnload = [](PDRIVER_OBJECT driver)
        {
            if (unload)
                unload(driver);
            doexit();
        };
    }
    else
        doexit();
    return status;
}
