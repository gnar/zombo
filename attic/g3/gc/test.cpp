#include <iostream>
using namespace std;

int bss;
int data = 1234;

int fn()
{
}

int main()
{
	int stack;
	int *heap = new int;

	cout << "DATA: " << (size_t)&data << endl;
	cout << "BSS: " << (size_t)&bss << endl;
	cout << "STACK: " << (size_t)&stack << endl;
	cout << "HEAP: " << (size_t)heap << endl;
	cout << "CODE: " << (size_t)fn << endl;
};


