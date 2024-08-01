#include <iostream>
#include <memory>

class Resource {
    public :
        
        Resource() {
            std::cout << "Resource accquired\n";
        }

        ~Resource() {
            std::cout << "Resource destroyed\n";
        }

        // 友元函数重载，需要显示地传递所有参数
        friend std::ostream& operator<<(std::ostream& out, const Resource &res) {
            out << "I am a resource\n";
            return out;
        }
};

void test1() {
    std::unique_ptr<Resource> res1{ new Resource{} };
    std::unique_ptr<Resource> res2{};

    std::cout << "res1 is " << (static_cast<bool>(res1) ? "not null\n" : "null\n");
    std::cout << "res2 is " << (static_cast<bool>(res2) ? "not null\n" : "null\n");

    res2 = std::move(res1);

    std::cout << "Ownership transferred\n";

    std::cout << "res1 is " << (static_cast<bool>(res1) ? "not null\n" : "null\n");
    std::cout << "res2 is " << (static_cast<bool>(res2) ? "not null\n" : "null\n");

    return ;
}

void test2() {
    std::unique_ptr<Resource> res{ new Resource{} };

    if (res) // use implicit cast to bool to ensure res contains a Resource
        std::cout << *res; // print the Resource that res is owning
}

class Fraction {
    private:
        int m_numerator{0};
        int m_denominator{1};
    
    public:
        Fraction(int numerator = 0, int denominator = 1) :
        m_denominator { denominator }, m_numerator { numerator }
        {}

        friend std::ostream& operator<<(std::ostream& out, const Fraction &f1)
        {
            out << f1.m_numerator << '/' << f1.m_denominator;
            return out;
        }
};

/*
    使用 std::make_unique 构造
    相比于 直接使用 new 方法的好处有：
        1.防止内存泄漏，分配内存和构造对象是一个原子操作，避免构造函数抛出异常导致内存泄漏；
        2.代码可读性更高
        3.提高代码的安全性
*/
void test3() {
    // Create a single dynamically allocated Fraction with numerator 3 and denominator 5
    // We can also use automatic type deduction to good effect here
    auto f1{ std::make_unique<Fraction>(3, 5) };
    std::cout << *f1 << '\n';

    // Create a dynamically allocated array of Fractions of length 4
    auto f2{ std::make_unique<Fraction[]>(4) };
    std::cout << f2[0] << '\n';
}

/*
    从函数返回 std::unique_ptr
*/
std::unique_ptr<Resource> createResource()
{
    return std::make_unique<Resource>();
}

void test4() {
    auto ptr{ createResource() };
}

/*
    向函数传递智能指针的方式：
        1.unique_ptr禁止值拷贝，只能使用语义移动来转移资源管理权，但是这个智能指针会在被调用函数结束时自动结束生命周期。
        2.大多数情况下，我们并不想让资源在被调用函数结束后释放。因此可以使用get()函数来获得原来的资源指针，这允许函数保持调用者管理资源。
*/
void takeOwnership(std::unique_ptr<Resource> res) {
    if (res) 
        std::cout << *res << '\n';
}

void useResource(Resource* res) {
    if (res)
        std::cout << *res << '\n';
}

void test5() {
    auto ptr {std::make_unique<Resource>()};

    // method2
    useResource(ptr.get());

    // method1
    takeOwnership(std::move(ptr));

    std::cout << "Ending program\n" ;
}

/*
    错误展示：
    1.禁止共享一个资源的管理权
        Resource *res{ new Resource() };
        std::unique_ptr<Resource> res1{ res };
        std::unique_ptr<Resource> res2{ res };
    2.禁止手动删除资源指针
        Resource *res{ new Resource() };
        std::unique_ptr<Resource> res1{ res };
        delete res;
*/

int main() {
    printf("\nTest1 for unique_ptr (语义转移):\n");
    test1();

    printf("\nTest2 for unique_ptr (访问资源):\n");
    test2();

    printf("\nTest3 for unique_ptr (构造方法):\n");
    test3();

    printf("\nTest4 for unique_ptr (函数返回指针):\n");
    test4();

    printf("\nTest5 for unique_ptr (函数传入指针参数):\n");
    test5();
}