export module Paging;
/*
*
* malloc 4x your RAM and check the return value.
Write one byte per 4 KB page, and
watch RSS in /proc/self/status climb until the
OOM killer shows up in dmesg. Segfault on purpose in a separate run, and compare the signal you get.
Bonus: rerun with vm.overcommit_memory=2 and see where it   fails instead.
*
  1. Malloc 4x RAM + watch working set grow
																																								   // Use GlobalMemoryStatusEx to get total physical RAM
  // malloc(4 * totalRAM), then loop writing 1 byte per 4KB page                                                                                                   // After each chunk of pages, call GetProcessMemoryInfo() and print WorkingSetSize
  // Windows will page to disk, slow to a crawl, and eventually malloc returns NULL

  Key APIs:
  - GetPhysicallyInstalledSystemMemory() — get total physical RAM
  - VirtualAlloc() - allocate the memory
  _ VirtualFree() - free the memory

  2. Segfault on purpose

  // Dereference a null pointer or write to a VirtualAlloc'd page with PAGE_NOACCESS
  // Windows raises a Structured Exception: EXCEPTION_ACCESS_VIOLATION (0xC0000005)
  // Use __try / __except to catch it, or just let it crash and check the exit code

  Compare: Linux gives SIGSEGV (signal 11), Windows gives exit code 0xC0000005.

  3.  OOM on purpose
*/

export namespace Paging {
	constexpr size_t HUGE_SIZE = 0xFFFFFFFFFFFFFFFF; // 2^64 - 1, maximum size_t value
	using PointerUnsignedLongLong = unsigned __int64*;
	void triggerSegFault();
	void triggerOOM();
	PointerUnsignedLongLong getTotalPhysicalRAM();
	void allocateMemoryPerPage();
}