//
// Created by devil on 2022-08-08.
//
#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <thread>
using std::thread;
using std::cout;
using std::endl;

static void*& NextObj(void* obj) {
    return *(void**)obj;
}
class FreeList {
public:
    void Push(void* obj) {
        assert(obj);
        NextObj(obj) = _freelist;
        _freelist = obj;
    }
    void* Pop() {
        assert(_freelist);
        void* res = _freelist;
        _freelist = NextObj(_freelist);
        return res;
    }
    bool Empty() {
        return _freelist == nullptr;
    }
private:
    void* _freelist = nullptr;
};


const int MAX_BYTES = 256 * 1024;
const int NFREE_LISTS = 208;

class SizeClass {//重点之一
public:
    static inline size_t _RoundUp(int bytes, int alignNum) {
        return (bytes + alignNum - 1) & ~(alignNum - 1);
    }
    static inline size_t RoundUp(int bytes) {
        assert(bytes <= MAX_BYTES);
        if (bytes <= 128) {
            return _RoundUp(bytes, 8);
        }
        else if (bytes <= 1024) {
            return _RoundUp(bytes, 16);
        }
        else if (bytes <= 8 * 1024) {
            return _RoundUp(bytes, 128);
        }
        else if (bytes <= 64 * 1024) {
            return _RoundUp(bytes, 1024);
        }
        else if (bytes <= 256 * 1024) {
            return _RoundUp(bytes, 8 * 1024);
        }
        else {
            assert(false);
        }
        return -1;
    }
    static inline size_t _Index(int bytes, int align_shift) {
        return ((bytes + (1 << align_shift) - 1) >> align_shift) - 1;
    }

    static inline size_t Index(int size) {
        assert(size <= MAX_BYTES);
        static int group_array[4] = { 16, 56, 56, 56 };
        if (size <= 128) {
            return _Index(size, 3);
        }
        else if (size <= 1024) {
            return _Index(size - 128, 4) + group_array[0];
        }
        else if (size <= 8 * 1024) {
            return _Index(size - 1024, 7) + group_array[0] + group_array[1];
        }
        else if (size <= 64 * 1024) {
            return _Index(size - 8 * 1024, 10) + group_array[0] + group_array[1] + group_array[2];
        }
        else if (size <= 256 * 1024) {
            return _Index(size - 64 * 1024, 13) + group_array[0] + group_array[1] + group_array[2] + group_array[3];
        }
        else {
            assert(false);
        }
        return -1;
    }
};

