//
// Created by 55044 on 2021/6/4.
//

#ifndef DNS_THREADPOOL_H
#define DNS_THREADPOOL_H

#include "../allLibs/allLibs.h"

class ThreadPool {
public:
    HANDLE maxThread;
    std::atomic_int count = 100;

    ThreadPool() {
        if ((maxThread = CreateSemaphore(NULL, THREAD_COUNT, THREAD_COUNT, "maxThread")) == NULL) {
            ExecutionUtil::fatalError("创建信号量失败！");
        }
        ExecutionUtil::log("创建信号量成功！");
        cout << "线程池余量（不准确）" << "--" << count << "---" << endl;
    };

    /**
     * 创建线程前调用
     */
    void tryToCreateThread() {
        //信号量句柄
        HANDLE hSemaphore;
        //如果打开信号量失败
        if ((hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "maxThread")) == NULL) {
            ExecutionUtil::fatalError("打开信号量失败！");
        }
        //隐式信号量减1
        WaitForSingleObject(hSemaphore, INFINITE);
        count.operator--();
        cout << "线程池余量" << "--" << count << "---" << endl;
    }

    /**
     * 销毁线程后调用
     */
    void destroyThread() {
        //信号量句柄
        HANDLE hSemaphore;
        //如果打开信号量失败
        if ((hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "maxThread")) == NULL) {
            ExecutionUtil::fatalError("打开信号量失败！");
        }
        //显式信号量加1
        ReleaseSemaphore(hSemaphore, 1, NULL);
        count.operator++();
        cout << "线程池余量" << "--" << count << "---" << endl;
    }
};

#endif //DNS_THREADPOOL_H
