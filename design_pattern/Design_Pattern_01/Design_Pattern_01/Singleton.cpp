#include <iostream>
#include <memory>
#include <mutex>
#include <string>


namespace
{
    std::once_flag singletonFlag;
}

class Singleton1
{
private:
    Singleton1()
    {
        sObjCnt++;
    }

public:
    virtual ~Singleton1() = default;

    static std::shared_ptr<Singleton1> getInstance()
    {
        static std::shared_ptr<Singleton1> s_instance = std::shared_ptr<Singleton1>(new Singleton1);
        return s_instance;
    }

    static int getCounter()
    {
        return sObjCnt;
    }

private:
    static int sObjCnt;
};
int Singleton1::sObjCnt = 0;


class Singleton2
{
private:
    Singleton2()
    {
        sObjCnt++;
    }

public:
    virtual ~Singleton2() = default;

    static std::shared_ptr<Singleton2> getInstance()
    {
        return s_instance;
    }

    static int getCounter()
    {
        return sObjCnt;
    }

private:
    static std::shared_ptr<Singleton2> s_instance;
    static int sObjCnt;
};
std::shared_ptr<Singleton2> Singleton2::s_instance = std::shared_ptr<Singleton2>(new Singleton2);
int Singleton2::sObjCnt = 0;

class Singleton3
{
private:
    Singleton3()
    {
        sObjCnt++;
    }

public:
    virtual ~Singleton3() = default;

    static std::shared_ptr<Singleton3> getInstance()
    {
        std::call_once(singletonFlag, [&] {
            s_instance = std::shared_ptr<Singleton3>(new Singleton3());
        });

        return s_instance;
    }

    static int getCounter()
    {
        return sObjCnt;
    }

private:
    //static std::once_flag singletonFlag;
    static std::shared_ptr<Singleton3> s_instance;
    static int sObjCnt;
};
std::shared_ptr<Singleton3> Singleton3::s_instance = nullptr;
int Singleton3::sObjCnt = 0;

// 非线程安全，需要加锁
class Singleton4
{
private:
    Singleton4()
    {
        sObjCnt++;
    }

public:
    virtual ~Singleton4() = default;

    static std::shared_ptr<Singleton4> getInstance()
    {
        if (s_instance == nullptr) {
            s_instance = std::shared_ptr<Singleton4>(new Singleton4());
        }

        return s_instance;
    }

    static int getCounter()
    {
        return sObjCnt;
    }

private:
    //static std::once_flag singletonFlag;
    static std::shared_ptr<Singleton4> s_instance;
    static int sObjCnt;
};
std::shared_ptr<Singleton4> Singleton4::s_instance = nullptr;
int Singleton4::sObjCnt = 0;


int main(void)
{
    std::cout << "----------------Singleton1----------------\n";
    std::cout << "Singleton1 Obj Counter: " << Singleton1::getCounter() << std::endl;
    auto obj11 = Singleton1::getInstance();
    std::cout << "Singleton1 Obj Counter: " << Singleton1::getCounter() << std::endl;
    auto obj12 = Singleton1::getInstance();
    std::cout << "Singleton1 Obj Counter: " << Singleton1::getCounter() << std::endl;

    std::cout << "----------------Singleton2----------------\n";
    std::cout << "Singleton2 Obj Counter: " << Singleton2::getCounter() << std::endl;
    auto obj21 = Singleton2::getInstance();
    std::cout << "Singleton2 Obj Counter: " << Singleton2::getCounter() << std::endl;
    auto obj22 = Singleton2::getInstance();
    std::cout << "Singleton2 Obj Counter: " << Singleton2::getCounter() << std::endl;

    std::cout << "----------------Singleton3----------------\n";
    std::cout << "Singleton3 Obj Counter: " << Singleton3::getCounter() << std::endl;
    auto obj31 = Singleton3::getInstance();
    std::cout << "Singleton3 Obj Counter: " << Singleton3::getCounter() << std::endl;
    auto obj32 = Singleton3::getInstance();
    std::cout << "Singleton3 Obj Counter: " << Singleton3::getCounter() << std::endl;


    return 0;
}

