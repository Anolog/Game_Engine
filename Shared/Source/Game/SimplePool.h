#ifndef __SimplePool_H__
#define __SimplePool_H__

template <class MyType> class SimplePool
{
protected:
    std::vector<MyType*> m_InactiveObjects;

public:
    SimplePool()
    {
    }

    virtual ~SimplePool()
    {
        while( m_InactiveObjects.empty() == false )
        {
            delete m_InactiveObjects.back();
            m_InactiveObjects.pop_back();
        }
    }

    void AddNewObjectToPool(MyType* object)
    {
        m_InactiveObjects.push_back( object );
    }

    MyType* GetObjectFromPool()
    {
        if( m_InactiveObjects.size() == 0 )
            return 0;

        MyType* pObject = m_InactiveObjects.back();
        if( pObject != 0 )
            m_InactiveObjects.pop_back();
        return pObject;
    }

    void ReturnObject(MyType* object)
    {
        m_InactiveObjects.push_back( object );
    }
};

#endif //__SimplePool_H__
