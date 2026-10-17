module ApplicationRunner;
import Atomicity;
import Display;
import std;
void ApplicationRunner::printAtomicityResults() {
	Display::printHeader();
	std::println("  :: Concurrency Benchmarks ::                       (N = {}, Threads = {})", Atomicity::N, Atomicity::NUM_THREADS);
	std::println("");
	Display::printSeperator();

	auto baseline = Atomicity::calculateBaseline();
	std::println("  Baseline (expected sum 1..N):  {}", baseline);
	Display::printSeperator();

	auto regular = Atomicity::regularInt();
	std::println("  Regular Int:                   {}  {}", regular, regular == baseline ? "PASS" : "FAIL (data race)");

	auto volatileInt = Atomicity::volatileInt();
	std::println("  Volatile Int:                  {}  {}", volatileInt, volatileInt == baseline ? "PASS" : "FAIL (data race)");

	auto atomicInt = Atomicity::atomicInt();
	std::println("  Atomic Int:                    {}  {}", atomicInt, atomicInt == baseline ? "PASS" : "FAIL");

	auto mutexInt = Atomicity::mutexInt();
	std::println("  Mutex Int:                     {}  {}", mutexInt, mutexInt == baseline ? "PASS" : "FAIL");

	Display::printSeperator();
	std::println("");
}