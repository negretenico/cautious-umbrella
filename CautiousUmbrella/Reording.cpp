module Reordering;
import std;
int Reordering::withoutProperMemoryOrdering() {
	std::atomic<int> numOfStaleData;
	for (auto i = 0; i < Reordering::NUMBER_OF_ITERATIONS; i++) {
		std::atomic<int> x{ 0 };
		std::atomic<int> y{ 0 };
		int t1 = 1;
		int t2 = 1;
		std::thread threadA([&t1, &x, &y]() {
			//Note: relaxed memory ordering will allow the compiler to move the load of `y` to after 
			//Note: the store of 'x'
			x.store(1, std::memory_order_relaxed);
			t1 = y.load(std::memory_order_relaxed);
			});
		std::thread threadB([&t2, &x, &y]() {
			//Note: relaxed memory ordering will allow the compiler to move the load of `x` to after 
			//Note: the store of 'y'
			y.store(1, std::memory_order_relaxed);
			t2 = x.load(std::memory_order_relaxed);
			});
		threadA.join();
		threadB.join();
		//If things happened in order this should be impossible
		if (t1 == 0 && t2 == 0) {
			numOfStaleData += 1;
		}
	}
	return numOfStaleData.load();
}

int Reordering::withProperMemoryOrdering() {
	std::atomic<int> numOfStaleData;
	for (auto i = 0; i < Reordering::NUMBER_OF_ITERATIONS; i++) {
		std::atomic<int> x{ 0 };
		std::atomic<int> y{ 0 };
		int t1 = 1;
		int t2 = 1;
		std::thread threadA([&t1, &x, &y]() {
			//Note: relaxed memory ordering will allow the compiler to move the load of `y` to after 
			//Note: the store of 'x'
			x.store(1, std::memory_order_release);
			t1 = y.load(std::memory_order_acquire);
			});
		std::thread threadB([&t2, &x, &y]() {
			//Note: relaxed memory ordering will allow the compiler to move the load of `x` to after 
			//Note: the store of 'y'
			y.store(1, std::memory_order_release);
			t2 = x.load(std::memory_order_acquire);
			});
		threadA.join();
		threadB.join();
		//If things happened in order this should be impossible
		if (t1 == 0 && t2 == 0) {
			numOfStaleData += 1;
		}
	}
	return numOfStaleData.load();
}