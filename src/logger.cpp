//
// Created by Dante Rendell on 2023-11-06.
//

#include "logger.h"
#include <iostream>

Logger::Logger(char* p_name) {
    name = p_name;
    severity = LogSeverity::Note;
}

void Logger::setNote() {
    severity = LogSeverity::Note;
}

void Logger::setAlert() {
    severity = LogSeverity::Alert;
}

void Logger::setErr() {
    severity = LogSeverity::Error;
}

void Logger::post(const char* msg) {
    std::string prefix;

    switch (severity) {
        case Note:
            prefix = "NOTE::";
            break;

        case Alert:
            prefix = "ALERT::";
            break;

        case Error:
            prefix = "ERROR::";
            break;
    }

    std::cout << prefix << name << "::" << msg << std::endl;
}