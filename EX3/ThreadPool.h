//
// Created by shawn on 1/06/2018.
//

#ifndef CPP_PRJ_THREADPOOL_H
#define CPP_PRJ_THREADPOOL_H

#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class ThreadPool
{
public:
    ThreadPool (int numOftThreads);
    ~ThreadPool();
    void doJob (std::function <void (void)> func);
    ThreadPool();

private:
    std::mutex lock_;
    std::condition_variable condVar_;
    bool shutdown_;
    std::queue <std::function <void (void)>> jobs_;
    std::vector <std::thread> vectorOfThreads;

    void threadEntry (int i);

};



#endif //CPP_PRJ_THREADPOOL_H
