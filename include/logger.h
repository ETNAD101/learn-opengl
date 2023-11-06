//
// Created by Dante Rendell on 2023-11-06.
//


#ifndef LEARN_OPENGL_LOGGER_H
#define LEARN_OPENGL_LOGGER_H

#include <iostream>

enum LogSeverity {
    Note,
    Alert,
    Error,
};

class Logger {
private:
    char* name;
    LogSeverity severity;

public:
    Logger(char* p_name);
    void setNote();
    void setAlert();
    void setErr();

    void post(const char* msg);
};

#endif //LEARN_OPENGL_LOGGER_H
