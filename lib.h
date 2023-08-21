#ifndef __VIMOS_LIB_H__
#define __VIMOS_LIB_H__

#include "uefi.h"

extern EFI_SYSTEM_TABLE *ST;
extern EFI_BOOT_SERVICES *BS;

void Print(CHAR16 *str);
void PrintLn(CHAR16 *str);
void convertint(UINT64 value, CHAR16 *buf, int digit);

void efi_error(EFI_STATUS status, unsigned int line);
#define EFI_ERROR() efi_error(status, __LINE__)

#define STATUS_IS_SUCCESS(status) (status == EFI_SUCCESS)

#endif
