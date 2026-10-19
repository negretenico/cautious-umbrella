module; // Global module fragment for legacy headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <memoryapi.h>
#include <sysinfoapi.h>

#include <print>
module Paging; // Tells the compiler this implements the WinMemory module

Paging::PointerUnsignedLongLong Paging::getTotalPhysicalRAM() {
	/*
	* Things to note on windows:
	* GlobalMemoryStatusEx  - will give you the amount of memeory available to the OS
	* GetPhysicallyInstalledSystemMemory  - will give you the amount of physical memory installed on the machine
	*
	* GetPhysicallyInstalledSystemMemory (KB) >= GlobalMemoryStatusEx
	*
	* also
	* GetPhysicallyInstalledSystemMemory -> PULONGLONG but we made our own type for it to be more clear in the code
	*/
	Paging::PointerUnsignedLongLong totalPhysicallyInstalledMemory = new ULONGLONG;
	GetPhysicallyInstalledSystemMemory(totalPhysicallyInstalledMemory);
	return totalPhysicallyInstalledMemory;
}

void Paging::allocateMemoryPerPage() {
	auto totalRAM = getTotalPhysicalRAM();
	//Note: memory is installed in GB, but this API returns the value in KB
	auto amountToAllocate = (*totalRAM) * 4 * 1000; // 4x RAM
	auto pageSize = 4096; // 4 KB pages
	auto numPages = amountToAllocate / pageSize;
	std::println("Attempting to allocate {} bytes ({} pages of {} bytes each)", amountToAllocate, numPages, pageSize);
	// Reserve the memory, but don't commit it yet
	// Note: VirtualAlloc with MEM_RESERVE will reserve the address space but not commit physical memory
	// Note: we use a char* instead of the returned void* to make pointer arithmetic easier
	char* allocatedMemory = static_cast<char*>(VirtualAlloc(nullptr, amountToAllocate, MEM_RESERVE, PAGE_READWRITE));
	if (allocatedMemory == nullptr) {
		std::println("Failed to reserve memory: {}", GetLastError());
		return;
	}
	for (size_t i{}; i < numPages; ++i) {
		//calclaute the pageaddress 
		char* pageAddress = allocatedMemory + (i * pageSize);
		// Write 1 byte per page to force the OS to commit the page
		void* page = VirtualAlloc(pageAddress, 1, MEM_COMMIT, PAGE_READWRITE);
		if (page == nullptr) {
			std::println("Failed to commit page {}: {}", i, GetLastError());
			break;
		}
		*pageAddress = 'A' + i;
		std::println("Committed page {}: {}", i, *pageAddress);
	}
	std::println("Finished allocating memory.");
	//We giveth the memory so we must taketh backeth the memory, lest we leaketh it and cause a memory leaketh
	VirtualFree(allocatedMemory, 0, MEM_RELEASE);
}

void Paging::triggerSegFault() {
	auto simluateSegFault = []() {
		// Dereference a null pointer to trigger a segmentation fault
		int* p = nullptr;
		*p = 42; // This will cause a segmentation fault
		};
	//Note: we use __try / __except when triggering faults that are sys\os layer
//Note: but we use try / catch when triggering faults that are C++ layer
	__try {
		simluateSegFault();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		/*
	 1. EXCEPTION_CONTINUE_EXECUTION (-1): "I fixed the problem. Clear the fault and re-try running the exact same line of code that just crashed."
	 2. EXCEPTION_EXECUTE_HANDLER (1): "I cannot fix this line, but I want to catch it. Stop executing the code inside __try and step directly into the code body inside { ... } underneath __except."
	  3. EXC EPTION_CONTINUE_SEARCH (0): "I don't recognize this type of crash (e.g., a Divide-by-Zero instead of an Access Violation). Pass it up to the next exception handler or let the OS crash dump catch it."
*/
		std::println("  Segmentation fault triggered and caught successfully.");
	}
}

void Paging::triggerOOM() {
	//Note: OOM is a C++ layer fault, so we use try / catch instead of __try / __except
	auto simulateOOMModernCPP = []() {
		// Attempt to allocate a very large amount of memory to trigger an out-of-memory condition
		size_t hugSize = Paging::HUGE_SIZE;
		char* leak = new char[hugSize];
		};
	auto simluateOOMUsingVirtualAlloc = []() {
		// Attempt to allocate a very large amount of memory using VirtualAlloc to trigger an out-of-memory condition
		size_t hugeSize = Paging::HUGE_SIZE;
		void* leak = VirtualAlloc(nullptr, hugeSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (leak == nullptr) {
			std::println("	Failed to allocate memory using VirtualAlloc: {}", GetLastError());
		}
		};

	try {
		simulateOOMModernCPP();
	}
	catch (const std::bad_alloc& _) {
		std::println("  Modern CPP Out-of-memory condition triggered and caught successfully.");
	}
	simluateOOMUsingVirtualAlloc();
}