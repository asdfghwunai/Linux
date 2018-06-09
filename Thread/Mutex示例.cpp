#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

std::map<std::string, std::string> g_pages;
std::mutex g_pages_mutex;   //保护全局变量用

void save_page(const std::string &url)
{
    // simulate a long page fetch
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::string result = "fake content";

    g_pages_mutex.lock();
    g_pages[url] = result;
    g_pages_mutex.unlock();
}

int main()
{
    std::thread t1(save_page, "http://foo");
    std::thread t2(save_page, "http://bar");
    t1.join();   //连接线程1，线程执行完才轮到主线程然后又执行线程2，是不是不是顺序执行啊，就是一下子启动俩子线程
    t2.join();

    g_pages_mutex.lock(); // not necessary as the threads are joined, but good style
    for (const auto &pair : g_pages)
    {
        std::cout << pair.first << " => " << pair.second << '\n';
    }
    g_pages_mutex.unlock();

    

    return 0;
}
