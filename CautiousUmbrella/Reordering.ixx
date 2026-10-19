export module Reordering;

export namespace Reordering {
	constexpr int THREAD_A_VALUE = 42;
	constexpr int THREAD_B_VALUE = 46;

	constexpr int NUMBER_OF_ITERATIONS = 1'000'000;
	int withoutProperMemoryOrdering();
	int withProperMemoryOrdering();
}