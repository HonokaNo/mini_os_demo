#include "lib.h"

EFI_SYSTEM_TABLE *ST;
EFI_BOOT_SERVICES *BS;

void Print(CHAR16 *str){
  ST->ConOut->OutputString(ST->ConOut, str);
}

void PrintLn(CHAR16 *str){
  Print(str);
  Print(L"\r\n");
}

void convertint(UINT64 value, CHAR16 *buf, int digit){
  int j;

  for(j = digit - 1; j >= 0; j--){
    buf[j] = L'0' + value % 10;
    value /= 10;
  }

  buf[digit] = 0x0000;
}

void efi_error(EFI_STATUS status, unsigned int line){
  CHAR16 *msg[15] = {
    L"EFI_LOAD_ERROR",
    L"EFI_INVALID_PARAMETER",
    L"EFI_UNSUPPORTED",
    L"EFI_BAD_BUFFER_SIZE",
    L"EFI_BUFFER_TOO_SMALL",
    L"EFI_NOT_READY",
    L"EFI_DEVICE_ERROR",
    L"EFI_WRITE_PROTECTED",
    L"EFI_OUT_OF_RESOURCES",
    L"EFI_VOLUME_CORRUPTED",
    L"EFI_VOLUME_FULL",
    L"EFI_NO_MEDIA",
    L"EFI_MEDIA_CHANGED",
    L"EFI_NOT_FOUND",
    L"EFI_ACCESS_DENIED"
  };
  CHAR16 s[5];

  if(STATUS_IS_SUCCESS(status)) return;
  else{
    Print(L"Error! ");
    PrintLn(msg[status - 1]);

    Print(L"Line:");
    convertint(line, s, 4);
    PrintLn(s);

    while(1) __asm__("hlt");
  }
}
