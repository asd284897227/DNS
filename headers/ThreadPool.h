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
            ExecutionUtil::fatalError("�����ź���ʧ�ܣ�");
        }
        ExecutionUtil::log("�����ź����ɹ���");
        cout << "�̳߳���������׼ȷ��" << "--" << count << "---" << endl;
    };

    /**
     * �����߳�ǰ����
     */
    void tryToCreateThread() {
        //�ź������
        HANDLE hSemaphore;
        //������ź���ʧ��
        if ((hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "maxThread")) == NULL) {
            ExecutionUtil::fatalError("���ź���ʧ�ܣ�");
        }
        //��ʽ�ź�����1
        WaitForSingleObject(hSemaphore, INFINITE);
        count.operator--();
        cout << "�̳߳�����" << "--" << count << "---" << endl;
    }

    /**
     * �����̺߳����
     */
    void destroyThread() {
        //�ź������
        HANDLE hSemaphore;
        //������ź���ʧ��
        if ((hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "maxThread")) == NULL) {
            ExecutionUtil::fatalError("���ź���ʧ�ܣ�");
        }
        //��ʽ�ź�����1
        ReleaseSemaphore(hSemaphore, 1, NULL);
        count.operator++();
        cout << "�̳߳�����" << "--" << count << "---" << endl;
    }
};

#endif //DNS_THREADPOOL_H
