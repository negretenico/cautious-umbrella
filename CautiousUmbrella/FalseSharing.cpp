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

The below fucntions seem to be independent so having threads
*/

void updatePlayerAttack() {
	constexpr int playerAttackDelta = 5;
	FalseSharing::Hero.attack += playerAttackDelta;
}

void updatePlayerHealth() {
	constexpr int playerHealthDelta = 5;
	FalseSharing::Hero.health += playerHealthDelta;
}
void updatePlayerDefense() {
	constexpr int playerDefenseDelta = -1;
	FalseSharing::Hero.defense += playerDefenseDelta;
}
void updatePlayerAttackSpeed() {
	constexpr int playerAttackSpeedDelta = +1;
	FalseSharing::Hero.attackSpeed += playerAttackSpeedDelta;
}
void updatePlayerMovementSpeedDelta() {
	constexpr int playerMovementSpeed = +1;
	FalseSharing::Hero.movementSpeed += playerMovementSpeed;

}
void FalseSharing::runFalseSharingBenchmark() {
	for (auto i = 0; i < 3; i++) {
		threads[i] = std::jthread([&i]() {
			for (auto current_iteration = 0; current_iteration < FalseSharing::NUM_OF_THREAD_ITERATIONS; current_iteration++) {
				switch (i) {
				case 3:
					updatePlayerAttack();
					break;
				case 2:
					updatePlayerHealth();
					break;
				case 1:
					updatePlayerDefense();
					break;
				default:
					updatePlayerAttackSpeed();
				}
			}
			});
	}
	FalseSharing::joinThreads();
}