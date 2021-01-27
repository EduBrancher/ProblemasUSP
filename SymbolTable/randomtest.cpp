
#include <iostream>          
#include <wchar.h>
#include <chrono>
#include <string>
#include <random>
#include <climits>

using namespace std;

int main(){


	{
  // obtain a seed from the system clock:
  unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();

  // obtain a seed from the user:

  int a = 2110;
  string str(to_string(a));
  std::seed_seq seed2 (str.begin(),str.end());
  int b = 4132;
  string str2(to_string(b));
  std::seed_seq seed3(str2.begin(), str2.end());

  std::minstd_rand0 g1 (seed1);  // minstd_rand0 is a standard linear_congruential_engine
  std::minstd_rand0 g2 (seed2);
  std::minstd_rand0 g3 (seed3);

  for (int i = 0; i < 10; i++){
  std::cout << "A time seed produced: " << g1() << std::endl;

  std::cout << "Your seed produced: " << g2() << std::endl;

  std::cout << "Your seed produced: " << g3() << std::endl;
}
  return 0;
}
	
}