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
		void operator+= (const Stats& other) {
			this->attack += other.attack;
			this->health += other.health;
			this->defense += other.defense;
			this->attackSpeed += other.attackSpeed;
			this->movementSpeed += other.movementSpeed;
		}
	};


	struct PaddedStats {
		alignas(std::hardware_destructive_interference_size)	long health;
		alignas(std::hardware_destructive_interference_size) long attack;
		alignas(std::hardware_destructive_interference_size) long defense;
		alignas(std::hardware_destructive_interference_size) long attackSpeed;
		alignas(std::hardware_destructive_interference_size) long movementSpeed;
	};

	Stats Hero{
		   100,
		   50,
		   150,
		   2,
		   50
	};

	PaddedStats HeroWithPadding{
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