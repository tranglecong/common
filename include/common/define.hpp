#pragma once

#define UNCOPYABLE(classname)                        \
    classname(const classname&) = delete;            \
    classname& operator=(const classname&) = delete; \
    classname(classname&&) = delete;                 \
    classname& operator=(classname&&) = delete;
