export module Atomicity;
import <thread>;
/*
Start 8 threads, and have each one increment a shared counter 10M times.
Try four versions: a plain int, a volatile int, std::atomic with fetch_add, and a mutex. Compare the final counts and the timings.
Goal: explain in one sentence why volatile still loses updates.
*/
export namespace Atomicity {
	using IntegerSize = unsigned long long;
	constexpr unsigned int NUM_THREADS = 8;
	std::jthread threads[NUM_THREADS] = { std::jthread(),std::jthread(),std::jthread(),std::jthread(),std::jthread(),std::jthread(),std::jthread(),std::jthread() };
	constexpr IntegerSize N = 1000000;

	IntegerSize getStartIndex(IntegerSize index) {
		return index * (Atomicity::N / Atomicity::NUM_THREADS) + 1;
	}
	IntegerSize getEndIndex(IntegerSize index) {
		return (index + 1) * (Atomicity::N / Atomicity::NUM_THREADS);
	}
	/**
	 * Joins all threads.
	 * This is only needed becuase we declare our threads in static storage duration,
	 * so they live as long as the program does, we need to join them before the program exits,
	 * else we won't see the intended behavior of the program, and we will have a race condition on the counter variable.
	 */
	void joinThreads() {
		for (auto& thread : threads) {
			if (thread.joinable()) {
				thread.join();
			}
		}
	}
	IntegerSize regularInt();
	IntegerSize volatileInt();
	IntegerSize atomicInt();
	IntegerSize mutexInt();
	IntegerSize calculateBaseline() {
		return (Atomicity::N * (Atomicity::N + 1)) / 2;
	};
}