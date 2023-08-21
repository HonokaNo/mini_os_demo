#ifndef __VIMOS_UEFI_H__
#define __VIMOS_UEFI_H__

#include <stdint.h>

#define EFIAPI __attribute__((ms_abi))

typedef uint8_t BOOLEAN;
#define TRUE 1
#define FALSE 0

typedef int64_t INTN;
typedef uint64_t UINTN;

typedef int8_t INT8 ;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;
typedef uint32_t UINT32;
typedef int64_t INT64;
typedef uint64_t UINT64;

typedef uint8_t CHAR8;
typedef uint16_t CHAR16;

typedef void VOID;

typedef struct{
  int32_t guid0;
  int16_t guid1;
  int16_t guid2;
  int8_t  guid3[8];
}EFI_GUID;

typedef UINTN EFI_STATUS;
typedef VOID* EFI_HANDLE;
typedef VOID* EFI_EVENT;
typedef UINT64 EFI_LBA;
typedef UINTN EFI_TPL;

typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

typedef UINT64 EFI_LBA;

typedef struct{
  UINT16 Year;
  UINT8 Month;
  UINT8 Day;
  UINT8 Hour;
  UINT8 Minute;
  UINT8 Second;
  UINT8 Pad1;
  UINT32 Nanosecond;
  INT16 TimeZone;
  UINT8 Daylight;
  UINT8 Pad2;
}EFI_TIME;

typedef struct{
  UINT8 Addr[32];
}EFI_MAC_ADDRESS;

#define EFI_SUCCESS           0x0000000000000000
#define EFI_INVALID_PARAMETER 0x0000000000000002
#define EFI_UNSUPPORTED       0x0000000000000003
#define EFI_BAD_BUFFER_SIZE   0x0000000000000004
#define EFI_BUFFER_TOO_SMALL  0x0000000000000005
#define EFI_NOT_READY         0x0000000000000006

typedef struct _EFI_TABLE_HEADER EFI_TABLE_HEADER;
typedef struct _EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;
typedef struct _EFI_BOOT_SERVICES EFI_BOOT_SERVICES;
typedef struct _EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;
typedef struct _EFI_CONFIGURATION_TABLE EFI_CONFIGURATION_TABLE;
typedef struct _EFI_LOADED_IMAGE_PROTOCOL EFI_LOADED_IMAGE_PROTOCOL;
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL EFI_GRAPHICS_OUTPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;
typedef struct _EFI_FILE_PROTOCOL EFI_FILE_PROTOCOL;
typedef struct _EFI_FILE_INFO EFI_FILE_INFO;
typedef struct _EFI_BLOCK_IO_PROTOCOL EFI_BLOCK_IO_PROTOCOL;
typedef struct _EFI_SIMPLE_NETWORK_PROTOCOL EFI_SIMPLE_NETWORK_PROTOCOL;

#define EQUAL_GUID(g1, g2) ((g1.guid0 == g2.guid0) && (g1.guid1 == g2.guid1) && (g1.guid2 == g2.guid2) && \
    (g1.guid3[0] == g2.guid3[0]) && (g1.guid3[1] == g2.guid3[1]) && (g1.guid3[2] == g2.guid3[2]) && (g1.guid3[3] == g2.guid3[3]) && \
    (g1.guid3[4] == g2.guid3[4]) && (g1.guid3[5] == g2.guid3[5]) && (g1.guid3[6] == g2.guid3[6]) && (g1.guid3[7] == g2.guid3[7]))

struct _EFI_TABLE_HEADER{
  UINT64 Signature;
  UINT32 Revision;
  UINT32 HeaderSize;
  UINT32 CRC32;
  UINT32 Reserved;
};

struct _EFI_SYSTEM_TABLE{
  EFI_TABLE_HEADER Hdr;
  CHAR16 *FirmwareVendor;
  UINT32 FirmwareRevision;
  EFI_HANDLE ConsoleInHandle;
  void *ConIn;
  EFI_HANDLE ConsoleOutHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
  EFI_HANDLE StandardErrorHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
  EFI_RUNTIME_SERVICES *RuntimeServices;
  EFI_BOOT_SERVICES *BootServices;
  UINTN NumberOfTableEntries;
  EFI_CONFIGURATION_TABLE *ConfigurationTable;
};

typedef enum{
  AllocateAnyPages,
  AllocateMaxAddress,
  AllocateAddress,
  MaxAllocateType
}EFI_ALLOCATE_TYPE;

typedef enum{
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiMaxMemoryType
}EFI_MEMORY_TYPE;

typedef struct{
  UINT32 Type;
  EFI_PHYSICAL_ADDRESS PhysicalStart;
  EFI_VIRTUAL_ADDRESS VirtualStart;
  UINT64 NumberOfPage;
  UINT64 Attribute;
}EFI_MEMORY_DESCRIPTOR;

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

struct _EFI_BOOT_SERVICES{
  EFI_TABLE_HEADER Hdr;

  void *RaiseTPL;
  void *RestoreTPL;

  EFI_STATUS EFIAPI (*AllocatePages)(EFI_ALLOCATE_TYPE Type, EFI_MEMORY_TYPE MemoryType, UINTN Pages, EFI_PHYSICAL_ADDRESS *Memory);
  EFI_STATUS EFIAPI (*FreePages)(EFI_PHYSICAL_ADDRESS Memory, UINTN Pages);
  EFI_STATUS EFIAPI (*GetMemoryMap)(UINTN *MemoryMapSize, EFI_MEMORY_DESCRIPTOR *MemoryMap, UINTN *MapKey, UINTN *DescriptorSize, UINT32 *DescriptorVersion);
  EFI_STATUS EFIAPI (*AllocatePool)(EFI_MEMORY_TYPE PoolType, UINTN Size, VOID **Buffer);
  EFI_STATUS EFIAPI (*FreePool)(VOID *Buffer);

  void *CreateEvent;
  void *SetTimer;
  void *WaitForEvent;
  void *SignalEvent;
  void *CloseEvent;
  void *CheckEvent;

  void *InstallProtolInterface;
  void *ReinstallProtocolInterface;
  void *UninstallProtocolInterface;
  void *HandleProtocol;
  VOID *Reserved;
  void *RegisterProtocolNotify;
  void *LocateHandle;
  void *LocateDevicePath;
  void *InstallConfigurationTable;

  void *LoadImage;
  void *StartImage;
  void *Exit;
  void *UnloadImage;
  EFI_STATUS EFIAPI (*ExitBootServices)(EFI_HANDLE ImageHandle, UINTN MapKey);

  void *GetNextMonotonicCount;
  void *Stall;
  void *SetWatchdogTimer;

  void *ConnectController;
  void *DisconnectController;
 
  EFI_STATUS EFIAPI (*OpenProtocol)(EFI_HANDLE Handle, EFI_GUID *Protocol, VOID **Interface, EFI_HANDLE AgentHandle, EFI_HANDLE ControllerHandle, UINT32 Attributes);
  void *CloseProtocol;
  void *OpenProtocolInformation;

  void *ProtocolsPerHandle;
  void *LocateHandleBuffer;
  EFI_STATUS EFIAPI (*LocateProtocol)(EFI_GUID *Protocol, VOID *Registration, VOID **Interface);
};

typedef struct{
  UINT32 Resolution;
  UINT32 Accuracy;
  BOOLEAN SetsToZero;
}EFI_TIME_CAPABILITIES;

struct _EFI_RUNTIME_SERVICES{
  EFI_TABLE_HEADER Hdr;

  EFI_STATUS EFIAPI (*GetTime)(EFI_TIME *Time, EFI_TIME_CAPABILITIES *Capabilities);
  void *SetTime;
  void *GetWakeupTime;
  void *SetWakeupTime;

  void *SetVirtualAddressMap;
  void *ConvertPointer;

  void *GetVariable;
  void *GetNextVariableName;
  void *SetVariable;

  void *GetNextHighMonotonicCount;
  void *ResetSystem;

  void *UpdateCapsule;
  void *QueryCapsuleCapabilities;

  void *QueryVariableInfo;
};

struct _EFI_CONFIGURATION_TABLE{
  EFI_GUID VendorGuid;
  VOID *VendorTable;
};

#define EFI_LOADED_IMAGE_PROTOCOL_GUID {0x5b1b31a1, 0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

struct _EFI_LOADED_IMAGE_PROTOCOL{
  UINT32 Revision;
  EFI_HANDLE ParentHandle;
  EFI_SYSTEM_TABLE *SystemTable;

  /* Source location of the image */
  EFI_HANDLE DeviceHandle;
};

struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL{
  void *Reset;
  EFI_STATUS EFIAPI (*OutputString)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);
  void *TestString;
  void *QueryMode;
  void *SetMode;
  void *SetAttribute;
  EFI_STATUS EFIAPI (*ClearScreen)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
};

typedef enum{
  PixelRedGreenBlueReserved8BitPerColor,
  PixelBlueGreenRedReserved8BitPerColor,
  PixelBitMask,
  PixelBltOnly,
  PixelFormatMax
}EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct{
  UINT32 RedMask;
  UINT32 GreenMask;
  UINT32 BlueMask;
  UINT32 ReservedMask;
}EFI_PIXEL_BITMASK;

typedef struct{
  UINT32 Version;
  UINT32 HorizontalResolution;
  UINT32 VerticalResolution;
  EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
  EFI_PIXEL_BITMASK PixelInformation;
  UINT32 PixelsPerScanLine;
}EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct{
  UINT32 MaxMode;
  UINT32 Mode;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
  UINTN SizeOfInfo;
  EFI_PHYSICAL_ADDRESS FrameBufferBase;
  UINTN FrameBufferSize;
}EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct{
  UINT8 Blue;
  UINT8 Green;
  UINT8 Red;
  UINT8 Reserved;
}EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};

struct _EFI_GRAPHICS_OUTPUT_PROTOCOL{
  EFI_STATUS EFIAPI (*QueryMode)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This, UINT32 ModeNumber, UINTN *SizeOfInfo, EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info);
  EFI_STATUS EFIAPI (*SetMode)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This, UINT32 ModeNumber);
  void *Blt;
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
};

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID {0x964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL{
  UINT64 Revision;
  EFI_STATUS EFIAPI (*OpenVolume)(EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This, EFI_FILE_PROTOCOL **Root);
};

/* Open Modes */
#define EFI_FILE_MODE_READ   0x0000000000000001
#define EFI_FILE_MODE_WRITE  0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

/* File Attributes */
#define EFI_FILE_READ_ONLY  0x0000000000000001
#define EFI_FILE_HIDDEN     0x0000000000000002
#define EFI_FILE_SYSTEM     0x0000000000000004
#define EFI_FILE_RESERVED   0x0000000000000008
#define EFI_FILE_DIRECTORY  0x0000000000000010
#define EFI_FILE_ARCHIVE    0x0000000000000020
#define EFI_FILE_VALID_ATTR 0x0000000000000037

struct _EFI_FILE_PROTOCOL{
  UINT64 Revision;
  EFI_STATUS EFIAPI (*Open)(EFI_FILE_PROTOCOL *This, EFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, UINT64 OpenMode, UINT64 Attributes);
  EFI_STATUS EFIAPI (*Close)(EFI_FILE_PROTOCOL *This);
  EFI_STATUS EFIAPI (*Delete)(EFI_FILE_PROTOCOL *This);
  EFI_STATUS EFIAPI (*Read)(EFI_FILE_PROTOCOL *This, UINTN *BufferSize, VOID *Buffer);
  EFI_STATUS EFIAPI (*Write)(EFI_FILE_PROTOCOL *This, UINTN *BufferSize, VOID *Buffer);
  EFI_STATUS EFIAPI (*GetPosition)(EFI_FILE_PROTOCOL *This, UINT64 *Position);
  EFI_STATUS EFIAPI (*SetPosition)(EFI_FILE_PROTOCOL *This, UINT64 Position);
  EFI_STATUS EFIAPI (*GetInfo)(EFI_FILE_PROTOCOL *This, EFI_GUID *InformationType, UINTN *BufferSize, VOID *Buffer);
  EFI_STATUS EFIAPI (*SetInfo)(EFI_FILE_PROTOCOL *This, EFI_GUID *InformationType, UINTN BufferSize, VOID *Buffer);
};

#define EFI_FILE_INFO_ID {0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

struct _EFI_FILE_INFO{
  UINT64 Size;
  UINT64 FileSize;
  UINT64 PhysicalSize;
  EFI_TIME CreateTime;
  EFI_TIME LastAccessTime;
  EFI_TIME ModificationTime;
  UINT64 Attribute;
  CHAR16 FileName[];
};

typedef struct{
  UINT32 MediaId;
  BOOLEAN RemovableMedia;
  BOOLEAN MediaPresent;
  BOOLEAN LogicalPartition;
  BOOLEAN ReadOnly;
  BOOLEAN WriteCaching;
  UINT32 BlockSize;
  UINT32 IoAlign;
  EFI_LBA LastBlock;

  EFI_LBA LowestAlignedLba;
  UINT32 LogicalBlocksPerPhysicalBlock;
  UINT32 OptimalTransferLengthGranularity;
}EFI_BLOCK_IO_MEDIA;

#define EFI_BLOCK_IO_PROTOCOL_GUID {0x964e5b21, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

struct _EFI_BLOCK_IO_PROTOCOL{
  UINT64 Revision;
  EFI_BLOCK_IO_MEDIA *Media;
  void *Reset;
  EFI_STATUS EFIAPI (*ReadBlocks)(EFI_BLOCK_IO_PROTOCOL *This, UINT32 MediaId, EFI_LBA LBA, UINTN BufferSize, VOID *Buffer);
  void *WriteBlocks;
  void *FlushBlocks;
};

#endif
