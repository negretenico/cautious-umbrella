module; // Global module fragment for legacy headers
#include <atomic>
#include <mutex>
#include <print>
#include <thread>
#include <windows.h>
module Scheduling; // Tells the compiler this implements the WinMemory module

std::atomic_flag isReady;
std::mutex m;
std::thread lowPriority;
std::thread mediumPriority;
std::thread highPriority;
void Scheduling::pinAllThreads() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	std::print("We have {} cores", sysInfo.dwNumberOfProcessors);
	//Note: in order to pin the core, set a bit in, bitmask, the bit mask is a represetnation 
	//Note: of what cores our process can run on, so 
	//Note: 0000000000000001 -> can only run on core 0, but 1000000000000001 -> can run on cores 15 and 0
	//Note: important to note that if we we call "GetSysteminfo" to figure out how many cores we have availabble
	//Note: now all threads that are children to this process will run on core 0 

	bool didWePinOurThreads = SetProcessAffinityMask(GetCurrentProcess(), 1);
	if (!didWePinOurThreads) {
		std::logic_error("We could not pin our threads");
	}
	std::println("Successfuly pinned process and child threads to core 0");
	bool didWeElevateOurProcessPriority = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	if (!didWeElevateOurProcessPriority) {
		std::logic_error("We could not elevate our priority");
	}
	std::println("Successfuly elevated proces priority");
}

void lowPriorityWork() {
	std::println("Low Priority worker spawned");
	while (!isReady.test(std::memory_order_acquire)) {
		//spin
		std::this_thread::yield();
	};
	std::println("Low Priority work started");
	m.lock(); // 1. Low grabs the mutex

	std::println("Low Priority holding mutex -> Doing 'slow' work");
	// Simulate slow work by spinning for just a moment to let High/Medium wake up
	for (volatile int i = 0; i < 1000000; ++i);

	// If priority inversion occurs, Low gets starved here and can never reach unlock!
	m.unlock();
	std::println("Low Priority work ended");
}

void mediumPriorityWork() {
	std::println("Medium Priority worker spawned");
	while (!isReady.test(std::memory_order_acquire)) {
		//spin
		std::this_thread::yield();
	};
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::println("Medium Priority work started");

	while (true) { /* simulated priority inversion barrier */
	};
	std::println("Medium Priority work ended");
}
void highPriorityWork() {
	std::println("High Priority worker spawned");
	while (!isReady.test(std::memory_order_acquire)) {
		//spin
		std::this_thread::yield();
	};
	// Give Low enough time to grab the mutex first
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	std::println("High Priority work started -> Attempting to acquire Mutex");

	// 2. High wakes up and tries to lock the mutex. It blocks because Low holds it.
	m.lock();

	std::println("High Priority successfully acquired Mutex!");
	m.unlock();
	std::println("High Priority work ended");
}

void Scheduling::spawnThreadsWithTheirPrioritySet() {
	lowPriority = std::thread(lowPriorityWork);
	mediumPriority = std::thread(mediumPriorityWork);
	highPriority = std::thread(highPriorityWork);
	std::println("   :: Created the threads  :: ");
	HANDLE low = lowPriority.native_handle();
	HANDLE medium = mediumPriority.native_handle();
	HANDLE high = highPriority.native_handle();
	SetThreadPriority(low, THREAD_PRIORITY_LOWEST);
	SetThreadPriority(medium, THREAD_PRIORITY_ABOVE_NORMAL);
	SetThreadPriority(high, THREAD_PRIORITY_TIME_CRITICAL);
	std::println("   :: Set the thread priority  :: ");
}

void Scheduling::runSimulation() {
	std::println("   :: Firing global start signal ::");
	isReady.test_and_set(std::memory_order_release);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::println("   :: Main thread checking status (Expect High/Low to be starved) ::");

	std::println("Closing simulation. (Medium thread is intentionally spinning infinitely)");
	lowPriority.detach();
	mediumPriority.detach();
	highPriority.detach();
}