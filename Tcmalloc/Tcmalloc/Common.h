#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <cstdint>
#include <thread>
#include <mutex>
using std::thread;
using std::cout;
using std::endl;

const int MAX_BYTES = 256 * 1024;
const int NFREE_LISTS = 208;

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
    void PushRange(void* start, void* end) {
        NextObj(end) = _freelist;
        _freelist = start;
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
    size_t& MaxSize() {
        return maxSize;
    }
private:
    void* _freelist = nullptr;
    size_t maxSize = 0;
};



class SizeClass {//重点之一
public:
    static inline size_t _RoundUp(size_t bytes, size_t alignNum) {
        return (bytes + alignNum - 1) & ~(alignNum - 1);
    }
    static inline size_t RoundUp(size_t bytes) {
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
    static inline size_t _Index(size_t bytes, size_t align_shift) {
        return ((bytes + (1 << align_shift) - 1) >> align_shift) - 1;
    }

    static inline size_t Index(size_t size) {
        assert(size <= MAX_BYTES);
        static size_t group_array[4] = { 16, 56, 56, 56 };
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
    static size_t NumMoveSize(size_t size)
    {
        assert(size > 0);

        // [2, 512]，一次批量移动多少个对象的(慢启动)上限值
        // 小对象一次批量上限高
        // 小对象一次批量上限低
        size_t num = MAX_BYTES / size;
        if (num < 2)
            num = 2;

        if (num > 512)
            num = 512;

        return num;
    }
};

#ifdef _WIN64
typedef uint64_t PAGE_ID;
#elif _WIN32
typedef uint32_t PAGE_ID;
#endif

struct Span {
    PAGE_ID page_id;
    size_t n;//页数
    
    Span* prev = nullptr;
    Span* next = nullptr;

    void* list = nullptr;
    size_t useCount = 0;
};

class SpanList {
public:
    SpanList() {
        head = new Span;
        head->next = head;
        head->prev = head;
    }
    void Insert(Span* pos, Span* newSpan) {
        assert(pos);
        assert(newSpan);
        Span* prev = pos->prev;
        prev->next = newSpan;
        newSpan->prev = prev;
        newSpan->next = pos;
        pos->prev = newSpan;
    }
    void Erase(Span* em) {
        assert(em != head);
        Span* next = em->next;
        Span* prev = em->prev;
        next->prev = prev;
        prev->next = next;
    }
private:
    Span* head = nullptr;
public:
    std::mutex mtx;
};