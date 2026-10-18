module FalseSharing;


/**
* Note: States is like this
* {
* 		long;
		long;
		long;
		long;
		long;
* }
* | attack  | health  | defense | attackS | moveSpe |
* | -8Byes- | -8Byes- | -8Byes- | -8Byes- | -8Byes- |

The below fucntions seem to be independent
*/

template <typename T>
void updatePlayerAttack(T& player) {
	constexpr int playerAttackDelta = 5;
	player.attack += playerAttackDelta;
}

template <typename T>
void updatePlayerHealth(T& player) {
	constexpr int playerHealthDelta = 5;
	player.health += playerHealthDelta;
}
template <typename T>
void updatePlayerDefense(T& player) {
	constexpr int playerDefenseDelta = -1;
	player.defense += playerDefenseDelta;
}
template <typename T>
void updatePlayerAttackSpeed(T& player) {
	constexpr int playerAttackSpeedDelta = +1;
	player.attackSpeed += playerAttackSpeedDelta;
}
template <typename T>
void updatePlayerMovementSpeedDelta(T& player) {
	constexpr int playerMovementSpeed = +1;
	player.movementSpeed += playerMovementSpeed;

}

void FalseSharing::runFalseSharingBenchmark() {
	for (auto i = 0; i < 3; i++) {
		threads[i] = std::jthread([i]() {
			for (auto current_iteration = 0; current_iteration < FalseSharing::NUM_OF_THREAD_ITERATIONS; current_iteration++) {
				switch (i) {
				case 3:
					updatePlayerAttack(FalseSharing::Hero);
					break;
				case 2:
					updatePlayerHealth(FalseSharing::Hero);
					break;
				case 1:
					updatePlayerDefense(FalseSharing::Hero);
					break;
				default:
					updatePlayerAttackSpeed(FalseSharing::Hero);
				}
			}
			});
	}
	FalseSharing::joinThreads();
}

void FalseSharing::runFalseSharingBenchmarkWithPadding() {
	for (auto i = 0; i < 3; i++) {
		threads[i] = std::jthread([i]() {
			for (auto current_iteration = 0; current_iteration < FalseSharing::NUM_OF_THREAD_ITERATIONS; current_iteration++) {
				switch (i) {
				case 3:
					updatePlayerAttack(FalseSharing::HeroWithPadding);
					break;
				case 2:
					updatePlayerHealth(FalseSharing::HeroWithPadding);
					break;
				case 1:
					updatePlayerDefense(FalseSharing::HeroWithPadding);
					break;
				default:
					updatePlayerAttackSpeed(FalseSharing::HeroWithPadding);
				}
			}
			});
	}
	FalseSharing::joinThreads();
}
void FalseSharing::runFalseSharingBenchmarkWithThreadLocal() {
	/*
	* Note: we need ot make this mutex stay alive unil all threads are joined, wihtout static
	* this mutex is only alive as long as the main thread is alive
	*/
	static std::mutex m;
	for (auto i = 0; i < 3; i++) {
		threads[i] = std::jthread([i]() {
			/*
			 * Note: While this eliminates false sharing via Thread-Local Storage (TLS) isolation,
			 * it is not a "clean" structural solution. It introduces duplication overhead:
			 * every thread must maintain its own full copy of the state in memory, and we are
			 * forced to write extra boilerplate logic to manually merge/aggregate these independent
			 * duplicates back into the global object at the very end.
			 */
			thread_local Stats localHero{};
			for (auto current_iteration = 0; current_iteration < FalseSharing::NUM_OF_THREAD_ITERATIONS; current_iteration++) {
				switch (i) {
				case 3:
					updatePlayerAttack(localHero);
					break;
				case 2:
					updatePlayerHealth(localHero);
					break;
				case 1:
					updatePlayerDefense(localHero);
					break;
				default:
					updatePlayerAttackSpeed(localHero);
				}
			}
			std::unique_lock lock(m);
			FalseSharing::Hero += localHero;
			});
	}
	FalseSharing::joinThreads();
}
