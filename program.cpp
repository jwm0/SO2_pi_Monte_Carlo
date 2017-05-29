//
// Project for SO2
// JWM
//

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <queue>

using namespace std;

queue <long long> LICZBY_LOSOWE; // kolejka przechowujaca liczby losowe
const int THREAD_COUNT = 4; // ilosc watkow
bool THREAD_STOP = false;

void generuj(){
  long long random_value = 0; //deklaracja losowej liczby
    size_t size = sizeof(random_value); //rozmiar danych
    ifstream urandom("/dev/urandom", ios::in|ios::binary);
    if(urandom)
    {
      while(!THREAD_STOP){
        urandom.read(reinterpret_cast<char*>(&random_value), size); //read urandom
        if(urandom) //read ok
        {
            cout << "random: " << random_value << endl;
            LICZBY_LOSOWE.push(random_value);
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

int main(){
  auto GENERATOR = thread(generuj);
  cout<<LICZBY_LOSOWE.size()<<endl;
  GENERATOR.join();
  cout<<LICZBY_LOSOWE.size()<<endl;

  return 0;
}
