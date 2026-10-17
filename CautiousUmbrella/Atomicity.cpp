module Atomicity;
import std;

template<typename  T>
void increment(T& counter, Atomicity::IntegerSize index) {
	for (Atomicity::IntegerSize i = Atomicity::getStartIndex(index); i <= Atomicity::getEndIndex(index); ++i) {
		counter += i;
	}
};

Atomicity::IntegerSize Atomicity::regularInt() {
	Atomicity::IntegerSize counter = 0;
	for (auto index = 0uz; index < NUM_THREADS; ++index) {
		threads[index] = std::jthread([&counter, index]() {
			increment(counter, index);
			});
	}
	Atomicity::joinThreads();
	return counter;
}

Atomicity::IntegerSize Atomicity::volatileInt() {
	/*
	We can see that voliate does fuck all for us here
	volatile means that it can change at any time, but that is in regards to external factos
	to the process not other threads within the process, so this is not a solution to the problem of atomicity
	*/
	volatile Atomicity::IntegerSize counter = 0;
	for (auto index = 0uz; index < NUM_THREADS; ++index) {
		threads[index] = std::jthread([&counter, index]() {
			increment(counter, index);
			});
	}
	Atomicity::joinThreads();
	return counter;
}

Atomicity::IntegerSize Atomicity::atomicInt() {
	std::atomic<Atomicity::IntegerSize	> counter = 0;
	for (auto index = 0uz; index < NUM_THREADS; ++index) {
		threads[index] = std::jthread([&counter, index]() {
			for (auto i = getStartIndex(index); i <= getEndIndex(index); ++i) {
				counter.fetch_add(i, std::memory_order_relaxed);
			}
			});
	}
	Atomicity::joinThreads();
	return counter;
}

Atomicity::IntegerSize Atomicity::mutexInt() {
	Atomicity::IntegerSize counter = 0;
	std::mutex mtx;
	for (auto index = 0uz; index < NUM_THREADS; ++index) {
		threads[index] = std::jthread([&counter, &mtx, index]() {
			std::scoped_lock<std::mutex> lock(mtx);
			increment(counter, index);
			});
	}
	Atomicity::joinThreads();
	return counter;
}


