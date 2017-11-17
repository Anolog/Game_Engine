#include "pch.h"

#if WIN32

#include "CPPList.h"

#define newtype_reg         1
#define newtype_array       2

#undef new

class MemObject : public CPPListNode
{
public:
    char* m_file;
    unsigned long m_line;
    int m_type;

    MemObject() :
        m_file(0),
        m_line(0),
        m_type(0)
    {
    }
};

CPPListHead g_Allocations;

void MyMemory_ValidateAllocations(bool AssertOnAnyAllocation)
{
    CPPListNode* pNode;
    for( pNode = g_Allocations.HeadNode.Next; pNode->Next; pNode = pNode->Next )
    {
        MemObject* obj = (MemObject*)pNode;
        assert( obj->m_type < 3 );
        assert( obj->Next != NULL );
        assert( obj->Prev != NULL );

        OutputMessage( "%s(%d): Memory unreleased.\n", obj->m_file, obj->m_line );
        if( AssertOnAnyAllocation )
            assert( false );
    }
}

void* operator new(size_t size, char* file, unsigned long line)
{
    MemObject* mo = (MemObject*)malloc( size + sizeof(MemObject) );
    mo->m_file = file;
    mo->m_line = line;
    mo->m_type = newtype_reg;
    g_Allocations.AddTail( mo );

    return ((char*)mo) + sizeof(MemObject);
}

void* operator new[](size_t size, char* file, unsigned long line)
{
    MemObject* mo = (MemObject*)malloc( size + sizeof(MemObject) );
    mo->m_file = file;
    mo->m_line = line;
    mo->m_type = newtype_array;
    g_Allocations.AddTail( mo );

    return ((char*)mo) + sizeof( MemObject );
}

void operator delete(void* m, char* file, unsigned long line)
{
    delete (char*)m;
}

void operator delete[](void* m, char* file, unsigned long line)
{
    delete (char*)m;
}




void* operator new(size_t size)
{
    MyAssert( size > 0 );

    MemObject* mo = (MemObject*)malloc( size + sizeof(MemObject) );
    mo->m_file = 0;
    mo->m_line = 0;
    mo->m_type = newtype_reg;
    g_Allocations.AddTail( mo );

    return ((char*)mo) + sizeof( MemObject );
}

void* operator new[](size_t size)
{
    MemObject* mo = (MemObject*)malloc( size + sizeof(MemObject) );
    mo->m_file = 0;
    mo->m_line = 0;
    mo->m_type = newtype_array;
    g_Allocations.AddTail( mo );

    return ((char*)mo) + sizeof( MemObject );
}

void operator delete(void* m)
{
    if( m == 0 )
        return;

    MemObject* mo = (MemObject*)(((char*)m) - sizeof(MemObject));
    assert( mo->m_type == newtype_reg );
    mo->Remove();

    free( mo );
}

void operator delete[](void* m)
{
    if( m == 0 )
        return;

    MemObject* mo = (MemObject*)( ((char*)m) - sizeof(MemObject) );
    assert( mo->m_type == newtype_array );
    mo->Remove();

    free( mo );
}

#endif // WIN32
