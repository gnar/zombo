#ifndef COLLECTABLE_H
#define COLLECTABLE_H

struct need_bigger_mark_list // used as an exception by Object::gc_mark
{
};

struct Collectable
{
	Collectable()
	{
		info.forward_ptr = 0;
		info.mark = 0;
	}

	virtual ~Collectable()
	{
	}

	Collectable *gc_get_forward_ptr() { return info.forward_ptr; }
	void gc_set_forward_ptr(Collectable *ptr) { info.forward_ptr = ptr; }

	virtual Collectable **gc_mark(Collectable **b, Collectable **e) = 0;
	virtual void gc_update_pointers() = 0;

	unsigned long gc_get_mark() { return info.mark; }
	void gc_set_mark(unsigned long mark) { info.mark = mark; }

private:
	mutable struct {
		Collectable *forward_ptr;
		unsigned long mark;
	} info;
};

#endif
