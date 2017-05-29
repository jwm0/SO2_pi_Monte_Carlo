#include "Plansza.h"

void Plansza::incrementTotal() {
	std::lock_guard<std::mutex> guard(boardGuard);
	total++;
}

void Plansza::incrementHits() {
	std::lock_guard<std::mutex> guard(boardGuard);
	hits++;
}

void Plansza::setTotal(long long newTotal) {
	std::lock_guard<std::mutex> guard(boardGuard);
	total=newTotal;
}

void Plansza::setHits(long long newHits) {
	std::lock_guard<std::mutex> guard(boardGuard);
	hits=newHits;
}

long long Plansza::getTotal() {
	std::lock_guard<std::mutex> guard(boardGuard);
	return total;
}

long long Plansza::getHits() {
	std::lock_guard<std::mutex> guard(boardGuard);
	return hits;
}
