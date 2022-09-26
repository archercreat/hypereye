#include <ntddk.h>

#include "arch/arch.hpp"

#include "shared/trace.hpp"
#include "hv/hypervisor.hpp"

void unload(PDRIVER_OBJECT drv)
{
    UNREFERENCED_PARAMETER(drv);

    logger::info("Unloading...");
}

NTSTATUS DriverMain(PDRIVER_OBJECT drv, PUNICODE_STRING reg)
{
    UNREFERENCED_PARAMETER(reg);
    logger::info("Driver loaded!");
    // Make driver unloadable.
    //
    drv->DriverUnload = unload;
    // Enter vmm.
    //
    ept_t::get();
    auto& hv = hypervisor::get();
    if (!hv.enter())
    {
        logger::info("hypervisor initialization failed.");
        return STATUS_FAILED_DRIVER_ENTRY;
    }
    return STATUS_SUCCESS;
}
