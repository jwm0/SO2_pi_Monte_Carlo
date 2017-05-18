#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <queue>


using namespace std;
queue <long long> kolejka;

void generuj(){
  long long random_value = 0; //deklaracja losowej liczby
    size_t size = sizeof(random_value); //rozmiar danych
    ifstream urandom("/dev/urandom", ios::in|ios::binary);
    if(urandom) //if open true
    {
      for(int i=0;i<10;i++){
        urandom.read(reinterpret_cast<char*>(&random_value), size); //read urandom
        if(urandom) //read ok
        {
            cout << "random: " << random_value << endl;
            kolejka.push(random_value);
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
  auto generatorThread = thread(generuj);
  cout<<kolejka.size()<<endl;
  generatorThread.join();
  cout<<kolejka.size()<<endl;

  return 0;
}
