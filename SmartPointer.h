#include <iostream>

template<class T>
class SmartPointer
{
public:
	SmartPointer<T>& operator=(const SmartPointer<T> &p) {
		if (ptr) release();
		ptr = p.ptr;
		ref_count = p.ref_count;
		if (ptr) (*ref_count)++;
		return *this;
	}

	SmartPointer<T>& operator=(T &obj) {
		if (ptr) release();
		ptr = &obj;
		(*ref_count)++;
		return *this;
	}

	SmartPointer(const SmartPointer<T> &p) { *this = p; }
	SmartPointer(T &obj) { *this = obj; }
	SmartPointer(T* obj) { *this = *obj; }
	SmartPointer(){	}

	~SmartPointer() {
		release();
	}

	T* operator->() { return ptr; }
	T operator*() { return *ptr; }


//private:
	T* ptr = nullptr;
	int* ref_count = new int[1]{ 0 };;
	void release() {
		if (*ref_count) {
			(*ref_count)--;
			if (!*ref_count) delete ptr;
		}
		ref_count = new int[1]{ 0 };
		ptr = nullptr;
	}
};

