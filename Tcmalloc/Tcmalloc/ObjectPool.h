//
// Created by devil on 2022-07-29.
//
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#endif
using std::cout;
using std::endl;
inline static void* SystemAlloc(size_t kpage) {
#ifdef _WIN32
    void* ptr = VirtualAlloc(nullptr, kpage << 13, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
#endif
    if (ptr == nullptr) {
        throw std::bad_alloc();
    }
    return ptr;
}
template <class T>
class ObjectPool {
public:
    T* New() {
        T* obj;
        if (_freeList != nullptr) {
            void* next = *(void**)_freeList;
            obj = (T*)_freeList;
            _freeList = next;
        }
        else {
            //剩余空间不够赋给一个T类型的，重新申请一块大内存
            if (_remainBytes < sizeof(T)) {
                _remainBytes = 128 * 1024;
                _memory = (char*)SystemAlloc(_remainBytes>>13);
                if (_memory == nullptr) {
                    throw std::bad_alloc();
                }
            }
            obj = (T*)_memory;
            size_t objSize = sizeof(obj) < sizeof(void*) ? sizeof(void*) : sizeof(obj);
            _memory += objSize;
            _remainBytes -= objSize;
        }
        new(obj)T;
        return obj;

    }
    void Delete(T* obj) {
        obj->~T();//显式调用析构函数
        *(void**)obj = _freeList;//二级指针保证在32位和64位环境下都能取出能存储一个指针大小的空间
        _freeList = obj;
    }
private:
    char* _memory = nullptr;
    void* _freeList = nullptr;
    size_t _remainBytes = 0;
};