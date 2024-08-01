# include <iostream>

template<class T>
class Auto_ptr1
{
    T* m_ptr;

    public:
    // Pass in a pointer to "own" via the constructor
    Auto_ptr1(T* ptr=nullptr)
    :m_ptr(ptr)
    {
    }

    // The destructor will make sure it gets deallocated
    ~Auto_ptr1()
    {
        delete m_ptr;
    }

    // Overload dereference and operator-> so we can use Auto_ptr1 like m_ptr.
    T& operator*() const { return*m_ptr; }
    T* operator->() const { return m_ptr; }

    
};

// A sample class to prove the above works
class Resource
{
    public:
        Resource() { std::cout << "Resource acquired\n"; }
        ~Resource() { std::cout << "Resource destroyed\n"; }
        void sayHi() { std::cout << "Hi!\n"; }
};

void someFunction()
{
    Auto_ptr1<Resource> ptr(new Resource); // ptr now owns the Resource
                                           // 指针现在拥有了 Resource

    int x;
    std::cout << "Enter an integer: ";
    std::cin >> x;

    if (x == 0)
        return; // the function returns early
                // 函数提前返回

    // do stuff with ptr here
    // 使用指针
    ptr->sayHi();
}

void test1() {
    Auto_ptr1<Resource> res(new Resource); // Note the allocation of memory here

            // ... but no explicit delete needed

    // Also note that the Resource in angled braces doesn't need a * symbol, since that's supplied by the template

    return ;
}

void test2() {
    someFunction();

    return ;
}

/*
    这个智能指针存在一个致命错误。
    两个智能指针通过假拷贝复制，导致两个对象都指向同一个资源。因此会造成多次资源的释放，导致第二次释放时，会导致悬空指针的释放。
*/
void test3_fatal_error() {
    Auto_ptr1<Resource> res1(new Resource);
    Auto_ptr1<Resource> res2(res1);
}

template<class T>
class Auto_ptr2
{
    T* m_ptr;

    public:
    // Pass in a pointer to "own" via the constructor
    Auto_ptr2(T* ptr=nullptr)
    :m_ptr(ptr)
    {
    }

    ~Auto_ptr2() {
        delete m_ptr;
    }

    // The destructor will make sure it gets deallocated
    Auto_ptr2(Auto_ptr2& ap)
    {   
        m_ptr = ap.m_ptr;
        ap.m_ptr = m_ptr;
    }

    // Overload dereference and operator-> so we can use Auto_ptr1 like m_ptr.
    T& operator*() const { return*m_ptr; }
    T* operator->() const { return m_ptr; }

    //  语义移动，相当于把资源指针的管理权"转移"给新的对象。
    Auto_ptr2& operator=(Auto_ptr2& a) {
        if (&a == this) 
            return *this;
        
        delete m_ptr;
        m_ptr = a.m_ptr;
        a.m_ptr = nullptr;
        return *this;
    }

    bool isNull() const {
        return m_ptr == nullptr;
    }
};

void test4() {
    Auto_ptr2<Resource> res1(new Resource);
    Auto_ptr2<Resource> res2; // Start as nullptr

    std::cout << "res1 is " << (res1.isNull() ? "null\n" : "not null\n");
    std::cout << "res2 is " << (res2.isNull() ? "null\n" : "not null\n");

    res2 = res1; // res2 assumes ownership, res1 is set to null

    std::cout << "Ownership transferred\n";

    std::cout << "res1 is " << (res1.isNull() ? "null\n" : "not null\n");
    std::cout << "res2 is " << (res2.isNull() ? "null\n" : "not null\n");

}

int main()
{
    printf("Test 1:\n");
    test1();

    printf("\nTest 2:\n");
    test2();

    // printf("\nTest 3:\n");
    // test3_fatal_error();

    printf("\nTest 4:\n");
    test4();

    return 0;
} // res goes out of scope here, and destroys the allocated Resource for us