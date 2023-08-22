#pragma once

#ifndef NONCOPY_HPP
#define NONCOPY_HPP

class NonCopyable {
protected:
    NonCopyable() {}
    ~NonCopyable() {}
private:
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&);
};

#endif