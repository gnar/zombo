#include "Collector.h"

#include <cstdlib> // malloc, realloc, free
#include <cassert>
#include <algorithm> // std::copy

#include <iostream>
#include <typeinfo>
using namespace std;

Collector::Collector() : mark_list(0), mark_list_size(4096)
{
	resize_mark_list(mark_list_size);

}

Collector::~Collector()
{
	std::free(mark_list);
}

void Collector::add_root(Collectable *obj)
{
	roots.push_back(obj);
}

void Collector::resize_mark_list(size_t new_size)
{
	mark_list_size = new_size;
	mark_list = (Collectable**)std::realloc(mark_list, sizeof(Collectable*) * mark_list_size);
}

void Collector::enlarge_mark_list()
{
	resize_mark_list(2 * mark_list_size);
}

void Collector::breadth_first_mark()
{
	Collectable **it0, **it1, **it2;
	Collectable **mark_list_end = mark_list + mark_list_size;

	// I. Push roots to mark list
	while (roots.size() > mark_list_size) enlarge_mark_list();
	it0 = mark_list;
	it1 = std::copy(roots.begin(), roots.end(), mark_list);
	it2 = it1;
	for (Collectable **it=it0; it<it1; ++it) (*it)->gc_set_mark(1);

	int cnt = 0;

	// II. Breadth first mark stage
	while (it0 != it1) {
		// one iteration: iterate through [it0, it1), copy reachable pointers to [it1, it2), increasing it2 as neccessary.
		for (Collectable **it=it0; it<it1; ++it) {
			Collectable *obj = *it;
			
			//cout << "GOT:" << (void*)obj << " " << (cnt++) << endl;
			try {
				it2 = (*it)->gc_mark(it2, mark_list_end);
			} catch (need_bigger_mark_list &e) {
				assert(0);
				/*const ptrdiff_t i0 = it0 - mark_list, i1 = it1 - mark_list, i2 = it2 - mark_list;
				enlarge_mark_list();
				it0 = mark_list + i0; it1 = mark_list + i1; it2 = mark_list + i2;
				mark_list_end = mark_list + mark_list_size;
				goto retry;*/
			}
		}

		// setup next iteration
		it0 = it1;
		it1 = it2;
	}

	assert(it0 == it1 && it1 == it2);

	for (Collectable **it=mark_list; it!=it1; ++it) {
		Collectable *obj = *it;
		cout << (void*)obj << ": " << typeid(*obj).name() << endl;
	}
}
