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
#include <algorithm>
#include <ncurses.h>
#include "Plansza.h"

using namespace std;

void addRandomNumber(unsigned long long &random_value, size_t size, ifstream &urandom);
void totalBoardHit(shared_ptr<Plansza> totalHits);
long long getValueAndPop(queue <long long> &kolejka);
void print(shared_ptr<Plansza>wynik);
void totalHitSum(shared_ptr<Plansza> total, vector<shared_ptr<Plansza>> wynik);

mutex QUEUE_GUARD;
condition_variable RANDOM_GENERATED;
queue <long long> LICZBY_LOSOWE; // kolejka przechowujaca liczby losowe
const int THREAD_COUNT = 2; // ilosc watkow
bool THREAD_STOP = false;
vector<shared_ptr<Plansza>> scores;
long RADIUS = 1000000;

void generuj()
{
    unsigned long long random_value = 0; //deklaracja losowej liczby
    size_t size = sizeof(random_value); //rozmiar danych
    ifstream urandom("/dev/urandom", ios::in|ios::binary);
    if(urandom)
    {
      while(!THREAD_STOP){
        //urandom.read(reinterpret_cast<char*>(&random_value), size); //read urandom
        lock_guard<mutex> guard(QUEUE_GUARD);
        addRandomNumber(random_value, size, urandom);
        addRandomNumber(random_value, size, urandom);
        RANDOM_GENERATED.notify_one();
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
  initscr();

  vector<shared_ptr<Plansza>> scores;
  vector<thread> threads;

  auto GENERATOR = thread(generuj);

  for(int i=0; i<THREAD_COUNT;i++){
    auto plansza = make_shared<Plansza>();
    scores.push_back(plansza);
    threads.push_back(thread(totalBoardHit, plansza));
  }


  auto total = make_shared<Plansza>();
  auto SUMATOR = thread(totalHitSum, total, scores);
  auto APROKSYMATOR = thread(print, total);

  GENERATOR.join();
  APROKSYMATOR.join();

  endwin();
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

void totalBoardHit(shared_ptr<Plansza> totalHits)
{
	while (!THREAD_STOP) {
		unique_lock<mutex> lock(QUEUE_GUARD); //can be unlocked
		RANDOM_GENERATED.wait(lock);
    long long x = getValueAndPop(LICZBY_LOSOWE);
    long long y = getValueAndPop(LICZBY_LOSOWE);
    long long z = sqrt(pow(x,2)+pow(y,2));
		totalHits->incrementTotal();
		if (z <= RADIUS)
			totalHits->incrementHits();
	}
}

void totalHitSum(shared_ptr<Plansza> totalScore, vector<shared_ptr<Plansza>> wynik) //funkcja sumujaca PRZETWARZACZE
{
  while (!THREAD_STOP){
    long long hit=0;
    long long total=0;

    for_each(wynik.begin(), wynik.end(), [&hit, &total](shared_ptr<Plansza> plansza){
      hit += plansza->getHits();
      total += plansza->getTotal();
    });
    totalScore->setHits(hit);
    totalScore->setTotal(total);
  }
}

void print(shared_ptr<Plansza>wynik)
{
  while (!THREAD_STOP){
    this_thread::sleep_for(chrono::milliseconds(100));
    if (wynik->getTotal() != 0){
      double ratio = ( (double) wynik->getHits() ) / ( (double) wynik->getTotal() );
      double pi = ratio * 4.0f;
      refresh();
      clear();
      wcout.precision(30);
      wcout << pi << endl;
    }
  }
}
