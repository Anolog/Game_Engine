#ifndef __MyMemory_H__
#define __MyMemory_H__

#if WIN32

void MyMemory_ValidateAllocations(bool AssertOnAnyAllocation);

void* operator new(size_t size, char* file, unsigned long line);
void* operator new[](size_t size, char* file, unsigned long line);
void operator delete(void* m, char* file, unsigned long line);
void operator delete[](void* m, char* file, unsigned long line);

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* m);
void operator delete[](void* m);

#define new new(__FILE__, __LINE__)

#endif // WIN32

#endif //__MyMemory_H__
