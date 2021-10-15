#include <iostream>
#include "BuddyAllocator.h"
using namespace std;

int main() {
	auto alloc = BuddyAllocator(1<<26);
	char* p = alloc.allocate(777);
	alloc.free(p);
	char* p1 = alloc.allocate(111);
	alloc.free(p1);
}