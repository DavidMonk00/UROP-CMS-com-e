#include <iostream>
#include <stdio.h>
#include <boost/array.hpp>

class Animal {
public:
  int x;
  Animal(void) {x = 3;}
  void printInt(void) {printf("%d\n", x);}
  virtual void eat(void) {printf("Food.\n");}
};
class Cat : public Animal {
public:
  Cat(int a) {x = a;}
  void eat(void) {printf("Rat.\n");}
};

void foo(Animal* a) {a->eat();}

using namespace std;
int main(){
  Animal* a = new Animal;
  Animal* c = new Cat(2);
  foo(a);
  foo(c);
  a->printInt();
  c->printInt();
  return 0;
}
