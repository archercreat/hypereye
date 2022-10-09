#include <ntddk.h>
#include "shared/globals.hpp"

#include "shared.hpp"
#include "control.hpp"

EXTERN_C
VOID
NTAPI
HeyeUnload(
    _In_ PDRIVER_OBJECT DriverObject
    )
{
    PAGED_CODE();
    // Delete device.
    //
    UNICODE_STRING DosName{};
    RtlInitUnicodeString(&DosName, DOS_DEVICE_NAME);

    IoDeleteSymbolicLink(&DosName);
    IoDeleteDevice(DriverObject->DeviceObject);
    // Delete global objects.
    //
    globals::teardown();
}

EXTERN_C
NTSTATUS
NTAPI
HeyeCreateClose(
    _In_ PDEVICE_OBJECT DeviceObject,
    _In_ PIRP           Irp
    )
{
    UNREFERENCED_PARAMETER(DeviceObject);
    PAGED_CODE();

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status      = STATUS_SUCCESS;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return Irp->IoStatus.Status;
}

EXTERN_C
NTSTATUS
NTAPI
HeyeUnsupported(
    _In_ PDEVICE_OBJECT DeviceObject,
    _In_ PIRP           Irp
    )
{
    UNREFERENCED_PARAMETER(DeviceObject);
    PAGED_CODE();

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status      = STATUS_NOT_SUPPORTED;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return Irp->IoStatus.Status;
}

EXTERN_C
NTSTATUS
NTAPI
HeyeIoControl(
    _In_ PDEVICE_OBJECT DeviceObject,
    _In_ PIRP           Irp
    )
{
    UNREFERENCED_PARAMETER(DeviceObject);
    PAGED_CODE();

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status      = STATUS_SUCCESS;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return Irp->IoStatus.Status;
}

EXTERN_C
NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
{
    UNREFERENCED_PARAMETER(RegistryPath);
    // Make driver unloadable.
    //
    DriverObject->DriverUnload = HeyeUnload;
    // Setup device.
    //
    PDEVICE_OBJECT DeviceObject{};

    UNICODE_STRING DeviceName{}, DosName{};
    RtlInitUnicodeString(&DeviceName, DEVICE_NAME);
    RtlInitUnicodeString(&DosName, DOS_DEVICE_NAME);

    if (!NT_SUCCESS(IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject)))
    {
        return STATUS_IO_DEVICE_ERROR;
    }

    for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = &HeyeUnsupported;

    DriverObject->MajorFunction[IRP_MJ_CREATE]         = &HeyeCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]          = &HeyeCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &HeyeIoControl;

    if (!NT_SUCCESS(IoCreateSymbolicLink(&DosName, &DeviceName)))
    {
        IoDeleteDevice(DeviceObject);
        return STATUS_FAILED_DRIVER_ENTRY;
    }

    globals::initialize();

    if (!run())
    {
        globals::teardown();
        return STATUS_FAILED_DRIVER_ENTRY;
    }
    return STATUS_SUCCESS;
}
