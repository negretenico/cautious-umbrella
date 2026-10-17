import Atomicity;
import std;

auto printSeperator() {
	std::println("===============================");
}
int main()
{
	auto baseline = Atomicity::calculateBaseline();
	auto regular = Atomicity::regularInt();
	auto volatileInt = Atomicity::volatileInt();
	auto atomicInt = Atomicity::atomicInt();
	auto mutexInt = Atomicity::mutexInt();
	std::println("Baseline: {}", baseline);
	printSeperator();
	std::println("Regular Int: {}", regular);
	printSeperator();
	std::println("Volatile Int: {}", volatileInt);
	printSeperator();
	std::println("Atomic Int: {}", atomicInt);
	printSeperator();
	std::println("Mutex Int: {}", mutexInt);
	return 0;
}