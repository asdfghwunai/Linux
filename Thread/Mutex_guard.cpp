/*
使用mutex的时候需要手动上锁还有解锁，C++11提供了另一个利器–>lock_guard,它是与mutex配合使用，把锁放到lock_guard中时，mutex自动上锁，
lock_guard析构时，同时把mutex解锁。使用的应该就是所谓的RAII技法（RAII，,也称为“资源获取就是初始化”，是C++语言的一种管理资源、避免泄漏的惯用法。
C++标准保证任何情况下，已构造的对象最终会销毁，即它的析构函数最终会被调用。
简单的说，RAII 的做法是使用一个对象，在其构造时获取资源，在对象生命期控制对资源的访问使之始终保持有效，最后在对象析构的时候释放资源。） 
*/

#include <thread>
#include <mutex>

int g_i = 0;
std::mutex g_i_mutex;  // protects g_i

void safe_increment()
{
    std::lock_guard<std::mutex> lock(g_i_mutex);
    ++g_i;

    // g_i_mutex is automatically released when lock
    // goes out of scope
}

int main()
{
    std::thread t1(safe_increment);
    std::thread t2(safe_increment);

    t1.join();
    t2.join();
} 
