//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_EXECUTIONUTIL_H
#define DNS_EXECUTIONUTIL_H

#include "../allLibs/allLibs.h"

class ExecutionUtil {
public:
    /**
     * 打印error，并终止程序
     * @param err
     */
    static void fatalError(string err) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
        printf("error：%s     【Error Code：%d】\n", err.data(), GetLastError());
        exit(-1);
    }

    /**
     * 打印warn
     * @param err
     */
    static void warning(string warn) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        printf("warn：%s     【Error Code：%d】\n", warn.data(), GetLastError());
        setDefaultFontColor();
    }

    static void log(string log){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
        printf("warn：%s     【Error Code：%d】\n", log.data(), GetLastError());
        setDefaultFontColor();
    }

private:
    static void setDefaultFontColor() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
};

#endif //DNS_EXECUTIONUTIL_H
