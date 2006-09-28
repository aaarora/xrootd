//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientVector                                                      // 
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
//                                                                      //
// A simple vector class based on low level functions                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//         $Id$


#ifndef XRD_CLIVEC_H
#define XRD_CLIVEC_H

#include <stdlib.h>

template<class T>
class XrdClientVector {


private:
    T *data;

    // Remember that the unit is sizeof(T)
    int size;
    int capacity;

    int BufRealloc(int newsize);

    inline void Init() {
	data = 0;
	size = 0;
	capacity = 0;
    }

public:

    int GetSize() {
	return size;
    }

    void Clear() {
	if (data) delete []data;
	Init();
    }

    XrdClientVector() {
	Init();
    }

    XrdClientVector(const XrdClientVector &v) {
	Init();
	BufRealloc(v.size);
	size = v.size;
	for (int i = 0; i < v.size; i++)
	    data[i] = v.data[i];
    }

    ~XrdClientVector() {
	Clear();
    }

    void Resize(int newsize) {
	BufRealloc(newsize);
    }

    void Push_back(T& item) {
      
	if ( !BufRealloc(size+1) ) {
	    data[size++] = item;
	}

    }

    // Inserts an item in the given position
    void Insert(T& item, int pos) {
      
	if (pos >= size) {
	    Push_back(item);
	    return;
	}

	if ( !BufRealloc(size+1) ) {

	    for (int i=size-1; i >= pos; i--)
		data[i+1] = data[i];

	    size++;

	    data[pos] = item;
	}

    }

    // Removes a single element in position pos
    T &Erase(unsigned int pos) {
	T &res = At(pos);

	for (int i=pos+1; i < size; i++)
	    data[i-1] = data[i];

	size--;
	BufRealloc(size);

	return res;
    }

    T &Pop_back() {
	T &r( At(size-1) );
	size--;
	return (r);
    }

    T Pop_front() {
	T res;

	res = At(0);
	for (int i=1; i < size; i++)
	    data[i-1] = data[i];
	size--;

	return (res);
    }


    // Bounded array like access
    T &At(int pos) {
        if ( (pos < 0) || (pos >= size) )
            abort();
        return data[pos];
    }
    T &operator[] (int pos) {
	return At(pos);
    }

};

template <class T>
int XrdClientVector<T>::BufRealloc(int newsize) {
    int sz, blks;
    T *newdata;

    if ((newsize <= 0) || (newsize <= capacity)) return 0;

    blks = (newsize) / 256 + 1;

    sz = blks * 256;

    newdata = new T[sz];


    if (newdata) {
	for (int i = 0; i < size; i++)
	    newdata[i] = data[i];

	delete []data;

	data = newdata;      
	capacity = sz;
	return 0;
    }

    abort();

    return 1;
}


#endif
