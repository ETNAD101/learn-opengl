//
// Created by Dante Rendell on 2023-11-06.
//

#include "logger.h"
#include <iostream>
#include <utility>

Logger::Logger(const char* p_name) {
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
    errorType = NULL;
    severity = LogSeverity::Error;
}

void Logger::setErr(const char* p_errType) {
    errorType = p_errType;
    severity = LogSeverity::Error;
}

void Logger::post(const char* msg) {
    if (log) {
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

        if (errorType) {
            std::cout << prefix << name << "::" << errorType << "::" << msg << std::endl;
            return;
        }

        std::cout << prefix << name << "::" << msg << std::endl;
    }
}