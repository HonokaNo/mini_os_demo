#include "lib.h"
#include "uefi.h"

#define NULL ((void *)0)

struct UEFI_MemoryMap{
  unsigned char *buffer;
  uint64_t mapsize;
  uint64_t descsize;
};

void *memcpy(void *buf1, const void *buf2, size_t n){
  UINT8 *x = (UINT8 *)buf1;
  const UINT8 *y = (const UINT8 *)buf2;

  while(n--){
    *(x++) = *(y++);
  }

  return buf1;
}

void *memset(void *buf, int ch, size_t n){
  unsigned char *s = buf;
  while(n--) *s++ = (unsigned char)ch;

  return buf;
}

UINTN GetMemoryMap(struct UEFI_MemoryMap *memmap, UINT32 mapsize){
  UINT32 j;
  UINTN key;

  memmap->mapsize = mapsize;
  EFI_STATUS status = BS->GetMemoryMap(&memmap->mapsize, (EFI_MEMORY_DESCRIPTOR *)memmap->buffer, &key, &memmap->descsize, &j);
  EFI_ERROR();

  return key;
}

void converthex(UINT64 value, CHAR16 *buf, int digit){
  CHAR16 *ch = L"0123456789ABCDEF";
  int j;

  buf[0] = L'0';
  buf[1] = L'x';

  for(j = 1 + digit; j >= 2; j--){
    buf[j] = ch[value % 16];
    value >>= 4;
  }

  buf[2 + digit] = 0x0000;
}

EFI_STATUS EFIAPI BootMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
  EFI_STATUS status;
  UINTN key;

  ST = SystemTable;
  BS = ST->BootServices;

  CHAR8 memmap_buf[1024 * 16];
  struct UEFI_MemoryMap memmap = {memmap_buf, sizeof(memmap_buf), 0};

  ST->ConOut->ClearScreen(ST->ConOut);
  PrintLn(L"Hello OS.");
  PrintLn(L"");

  CHAR16 buf1[20];

  EFI_GUID grahics_output_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
  status = BS->LocateProtocol(&grahics_output_guid, NULL, (VOID **)&gop);
  EFI_ERROR();
  /* currently, not support */
  if(gop->Mode->Info->PixelFormat == PixelBitMask || gop->Mode->Info->PixelFormat == PixelBltOnly){
    status = EFI_UNSUPPORTED;
    EFI_ERROR();
  }
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *mode = gop->Mode;

  converthex(mode->FrameBufferBase, &buf1[0], 16);
  uint64_t frameBufferBase = mode->FrameBufferBase;
  PrintLn(&buf1[0]);

  int width = mode->Info->HorizontalResolution, height = mode->Info->VerticalResolution;
  uint64_t pixels_per_scanline = mode->Info->PixelsPerScanLine;

  /* Alloc Page Tables */
  UINT64 pml4_addr, pdp_addr, page_dir, actual_page_dir;
  UINT64 kernel_pdp, kernel_page_directory_addr, kernel_page_table_addr;
  UINT64 kernel_pdp2, actual_page_dir2;

  status = BS->AllocatePages(AllocateAnyPages, EfiUnusableMemory, 1, &pml4_addr);
  EFI_ERROR();
  memset((void *)pml4_addr, 0, 4096 * 1);
  status = BS->AllocatePages(AllocateAnyPages, EfiLoaderData, 1, &pdp_addr);
  EFI_ERROR();
  memset((void *)pdp_addr, 0, 4096 * 1);
  status = BS->AllocatePages(AllocateAnyPages, EfiLoaderData, 4, &page_dir);
  EFI_ERROR();
  memset((void *)page_dir, 0, 4096 * 4);
  status = BS->AllocatePages(AllocateAnyPages, EfiUnusableMemory, 128, &actual_page_dir);
  EFI_ERROR();
  memset((void *)actual_page_dir, 0, 4096 * 128);
  status = BS->AllocatePages(AllocateAnyPages, EfiUnusableMemory, (512 - 128), &actual_page_dir2);
  EFI_ERROR();
  memset((void *)actual_page_dir2, 0, 4096 * (512 - 128));
  status = BS->AllocatePages(AllocateAnyPages, EfiUnusableMemory, 1, &kernel_pdp);
  EFI_ERROR();
  memset((void *)kernel_pdp, 0, 4096 * 1);
  status = BS->AllocatePages(AllocateAnyPages, EfiLoaderData, 1, &kernel_pdp2);
  EFI_ERROR();
  memset((void *)kernel_pdp2, 0, 4096 * 1);
  status = BS->AllocatePages(AllocateAnyPages, EfiUnusableMemory, 1, &kernel_page_directory_addr);
  EFI_ERROR();
  memset((void *)kernel_page_directory_addr, 0, 4096 * 1);
  status = BS->AllocatePages(AllocateAnyPages, EfiUnusableMemory, 16, &kernel_page_table_addr);
  EFI_ERROR();
  memset((void *)kernel_page_table_addr, 0, 4096 * 16);

  volatile UINT64 *pml4_table = (UINT64 *)pml4_addr, *pdp_table = (UINT64 *)pdp_addr;
  volatile UINT64 *actual_page_directory = (UINT64 *)actual_page_dir, *page_directory = (UINT64 *)page_dir;

  volatile UINT64 *kernel_pdp_table = (UINT64 *)kernel_pdp, *kernel_page_directory = (UINT64 *)kernel_page_directory_addr;
  volatile UINT64 *kernel_page_table = (UINT64 *)kernel_page_table_addr;

  volatile UINT64 *kernel_pdp_table2 = (UINT64 *)kernel_pdp2;
  volatile UINT64 *actual_page_directory2 = (UINT64 *)actual_page_dir2;

  pml4_table[0] = (UINT64)&pdp_table[0] | 0x003;
  pml4_table[256] = (UINT64)&kernel_pdp_table[0] | 0x003;
  pml4_table[257] = (UINT64)&kernel_pdp_table2[0] | 0x003;

  /* physical mapping(4GB) */
  for(uint64_t i = 0; i < 4; i++){
    pdp_table[i] = (UINT64)&page_directory[i * 512] | 0x003;
    for(uint64_t j = 0; j < 512; j++){
      page_directory[i * 512 + j] = (i * 512 * 512 * 4096) + (j * 512 * 4096) | 0x083;
    }
  }

  kernel_pdp_table[0] = (UINT64)&kernel_page_directory[0] | 0x003;
  /* kernel mapping(32MB) */
  for(uint64_t i = 0; i < 16; i++){
    kernel_page_directory[i] = (UINT64)&kernel_page_table[i * 512] | 0x003;
    for(uint64_t j = 0; j < 512; j++){
      kernel_page_table[i * 512 + j] = 0x100000 + (i * 4096 * 512) + (j * 4096) | 0x003;
    }
  }

  /* physical address mapping(512GB) */
  for(uint64_t i = 512 - 128; i < 512; i++){
    uint64_t index = i - (512 - 128);

    kernel_pdp_table[i] = (UINT64)&actual_page_directory[index * 512] | 0x003;
    for(uint64_t j = 0; j < 512; j++){
      uint64_t addr = (index * 512 * 512 * 4096) + (j * 512 * 4096);
      actual_page_directory[index * 512 + j] = addr | 0x083;
    }
  }

  uint64_t base_addr = (uint64_t)128 * (512 * 512 * 4096);
  for(uint64_t i = 0; i < (512 - 128); i++){
    kernel_pdp_table2[i] = (UINT64)&actual_page_directory2[i * 512] | 0x003;
    for(uint64_t j = 0; j < 512; j++){
      uint64_t addr = base_addr + (i * 512 * 512 * 4096) + (j * 512 * 4096);
      actual_page_directory2[i * 512 + j] = addr | 0x083;
    }
  }

  PrintLn(L"Booting...");

  converthex((uint64_t)&pml4_table[0], &buf1[0], 16);
  PrintLn(&buf1[0]);

  uint64_t control_register;

  __asm__ __volatile__("pushfq; popq %%rax":"=a"(control_register));
  converthex(control_register, &buf1[0], 16);
  PrintLn(&buf1[0]);

  __asm__ __volatile__("movq %%cr0,%%rax":"=a"(control_register));
  converthex(control_register, &buf1[0], 16);
  PrintLn(&buf1[0]);

  __asm__ __volatile__("movq %%cr4,%%rax":"=a"(control_register));
  converthex(control_register, &buf1[0], 16);
  PrintLn(&buf1[0]);

  uint32_t hefer, lefer;
  __asm__ __volatile__(
    "movq $0xc0000080,%%rcx\n"
    "rdmsr":"=d"(hefer),"=a"(lefer));
  converthex(hefer, &buf1[0], 8);
  PrintLn(&buf1[0]);
  converthex(lefer, &buf1[0], 8);
  PrintLn(&buf1[0]);

  /* ExitBootServicesのためにメモリマップを要求 */
  key = GetMemoryMap(&memmap, sizeof(memmap_buf));

  status = BS->ExitBootServices(ImageHandle, key);
  if(status != EFI_SUCCESS){
    /* エラーが起きているがこっそり隠す */

    /* retry */
    key = GetMemoryMap(&memmap, sizeof(memmap_buf));
    status = BS->ExitBootServices(ImageHandle, key);
    /* どうしても起動できなさそうならここでエラーメッセージだして止まる */
    EFI_ERROR();
  }

  void *page_tables = (void *)&pml4_table[0];
  /* set page table */
  __asm__ __volatile__("movq %%rax,%%cr3"::"a"(page_tables));

  unsigned char *frame_buf = (unsigned char *)(0xffff806000000000 + frameBufferBase);
  for(int j = 0; j < height; j++){
    for(int i = 0; i < width; i++){
      frame_buf[(j * pixels_per_scanline + i) * 4 + 0] = 0xff;
      frame_buf[(j * pixels_per_scanline + i) * 4 + 1] = 0xff;
      frame_buf[(j * pixels_per_scanline + i) * 4 + 2] = 0xff;
      frame_buf[(j * pixels_per_scanline + i) * 4 + 3] = 0xff;
    }
  }

  return EFI_SUCCESS;
}
