#include <windows.h>
#include <stdio.h>

int main() {
  SYSTEM_INFO sSysInfo;

  GetSystemInfo( & sSysInfo);

  DWORD dwPageSize = sSysInfo.dwPageSize;

  LPVOID lpvBase = VirtualAlloc(
    NULL,
    dwPageSize,
    MEM_RESERVE | MEM_COMMIT,
    PAGE_EXECUTE_READWRITE);
  if (lpvBase == NULL)
    printf("VirtualAlloc reserve failed.");

  BYTE * endExec = (BYTE * ) lpvBase;
  memset(lpvBase, 0x90, dwPageSize);
  endExec[1024] = 0xC3;

  INT64 t0 = 0;
  INT64 t1 = 0;
  _asm {
    rdtsc
    mov DWORD PTR[t0], eax
    mov DWORD PTR[t0 + 4], edx
    call endExec
    rdtsc
    mov DWORD PTR[t1], eax
    mov DWORD PTR[t1 + 4], edx
  }

  printf("%d", t1 - t0);

}