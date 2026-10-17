export module FalseSharing;
import std;

export namespace FalseSharing {
	constexpr int NUM_OF_THREAD_ITERATIONS = 2'000'000;
	struct Stats {
		long health;
		long attack;
		long defense;
		long attackSpeed;
		long movementSpeed;
	};

	Stats Hero{
		   100,
		   50,
		   150,
		   2,
		   50
	};
	std::jthread threads[4] = { std::jthread(),std::jthread(),std::jthread() };
	void joinThreads() {
		for (auto& thread : threads) {
			if (thread.joinable()) {
				thread.join();
			}
		}
	}
	void runFalseSharingBenchmark();
	void runFalseSharingBenchmarkWithPadding();
	void runFalseSharingBenchmarkWithThreadLocal();
}