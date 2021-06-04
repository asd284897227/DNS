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
        fflush(stdout);
        exit(-1);
    }

    /**
     * 打印warn
     * @param err
     */
    static void warning(string warn) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        printf("warn：%s\n", warn.data());
        setDefaultFontColor();
        fflush(stdout);
    }

    static void log(string log){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
        printf("log：%s\n", log.data());
        setDefaultFontColor();
        fflush(stdout);
    }

    static void debug(string log){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
        printf("debug：%s\n", log.data());
        setDefaultFontColor();
        fflush(stdout);
    }

private:
    static void setDefaultFontColor() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
};

#endif //DNS_EXECUTIONUTIL_H
