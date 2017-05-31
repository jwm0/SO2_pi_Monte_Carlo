//
// Project for SO2
// JWM
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <queue>
#include <cmath>
#include "Plansza.h"

using namespace std;

void addRandomNumber(unsigned long long &random_value, size_t size, ifstream &urandom);
void totalHitSum(shared_ptr<Plansza> totalHits);
long long getValueAndPop(queue <long long> &kolejka);

mutex QUEUE_GUARD;
condition_variable RANDOM_GENERATED;
queue <long long> LICZBY_LOSOWE; // kolejka przechowujaca liczby losowe
const int THREAD_COUNT = 4; // ilosc watkow
bool THREAD_STOP = false;
vector<shared_ptr<Plansza>> scores;
long RADIUS = 100000;

void generuj()
{
    unsigned long long random_value = 0; //deklaracja losowej liczby
    size_t size = sizeof(random_value); //rozmiar danych
    ifstream urandom("/dev/urandom", ios::in|ios::binary);
    if(urandom)
    {
      while(!THREAD_STOP){
        urandom.read(reinterpret_cast<char*>(&random_value), size); //read urandom
        if(urandom) //read ok
        {
            lock_guard<mutex> guard(QUEUE_GUARD);
            addRandomNumber(random_value, size, urandom);
            addRandomNumber(random_value, size, urandom);
            RANDOM_GENERATED.notify_one();
        }
        else
        {
            cerr << "Cannot read /dev/urandom" << endl;
        }
      }
        urandom.close(); //close stream
    }
    else
    {
        cerr << "Cannot open /dev/urandom" << endl;
    }
};

int main()
{
  vector<shared_ptr<Plansza>> scores;
  vector<thread> threads;

  auto GENERATOR = thread(generuj);
  auto plansza = make_shared<Plansza>();
  scores.push_back(plansza);
  threads.push_back(thread(totalHitSum, plansza));

  GENERATOR.join();

  return 0;
}

void addRandomNumber(unsigned long long &random_value, size_t size, ifstream &urandom)
{
	urandom.read(reinterpret_cast<char *>(&random_value), size);
	LICZBY_LOSOWE.push(random_value % (2 * RADIUS) - RADIUS);
  //cout<<LICZBY_LOSOWE.size()<<endl; // Sprawdzam czy kolejka zwieksza rozmiar
}

long long getValueAndPop(queue <long long> &kolejka)
{
  long long var = kolejka.front();
  kolejka.pop();
  return var;
}

void totalHitSum(shared_ptr<Plansza> totalHits)
{
	while (!THREAD_STOP) {
		unique_lock<mutex> lock(QUEUE_GUARD);
		RANDOM_GENERATED.wait(lock);
    long long x = getValueAndPop(LICZBY_LOSOWE);
    long long y = getValueAndPop(LICZBY_LOSOWE);
    long long z = sqrt(pow(x,2)+pow(y,2));
		totalHits->incrementTotal();
		if (z <= RADIUS)
			totalHits->incrementHits();
      cout<<totalHits->getHits() << endl;
	}
}
