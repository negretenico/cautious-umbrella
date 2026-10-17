module ApplicationRunner;
import Paging;
import Atomicity;
import Display;
import FalseSharing;
import std;
//TODO: our applicaiton runner should just be a thin orcherstaor from the command line
//TODO: we shoudl create instatsions of each print result instead of havin this cpp know about the how they 
//TODO: implemented
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


void ApplicationRunner::printPagingResults() {
	Display::printHeader();
	std::println("  :: Paging Benchmarks ::");
	std::println("");
	Display::printSeperator();
	auto totalRAM = Paging::getTotalPhysicalRAM();
	//Note: memory is installed in GB, but this API returns the value in KB
	std::println("  Total Physical RAM:            {} KB", (*totalRAM) / 1024);
	Display::printSeperator();
	//Note: we are not going to trigger a segfault or OOM in this example, as it would crash the application and make it hard to run the rest of the benchmarks
	Paging::allocateMemoryPerPage();
	Display::printSeperator();
	std::println("");
	Paging::triggerSegFault();
	Display::printSeperator();
	std::println("");
	Paging::triggerOOM();
}

void ApplicationRunner::printFalseShardBenchmarkResults() {
	Display::printHeader();
	std::println("  :: FalseSharing Benchmarks ::");
	std::println("");
	Display::printSeperator();
	auto start = std::chrono::steady_clock::now();
	FalseSharing::runFalseSharingBenchmark();
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::println("  :: FalseSharing            ::     {}", elapsed.count());
}