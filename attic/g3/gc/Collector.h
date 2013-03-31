#ifndef COLLECTOR_H
#define COLLECTOR_H

//#include <cstddef>
#include <vector>

#include "gc/Collectable.h"

class Collector
{
public:
	Collector();
	~Collector();
	
	void add_root(Collectable *obj);

	void breadth_first_mark();

private:
	void resize_mark_list(size_t new_size);
	void enlarge_mark_list();

	Collectable **mark_list; size_t mark_list_size;
	std::vector<Collectable*> roots;
};

#endif
