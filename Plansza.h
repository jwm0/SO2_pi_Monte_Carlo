#include <mutex>

class Plansza {
private:
  long long total = 0; // wszystkie strzaly
  long long hits = 0; // trafione strzaly
  std::mutex boardGuard;

public:
	void incrementTotal();

	void incrementHits();

	void setTotal(long long newTotal);

	void setHits(long long newHits);

	long long getTotal();

	long long getHits();
};
