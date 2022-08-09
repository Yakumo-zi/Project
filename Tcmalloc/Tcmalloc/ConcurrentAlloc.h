//
// Created by devil on 2022-08-09.
//
#pragma once

#include "Common.h"
#include "ThreadCache.h"

static void* ConcurrentAlloc(size_t size) {
    assert(size <= MAX_BYTES);
    if (pTLSThreadCache == nullptr) {
        pTLSThreadCache = new ThreadCache;
    }
    cout << "Thread:" << std::this_thread::get_id();
    printf(" pTLSThreadCache£º%p\n", pTLSThreadCache);
    return pTLSThreadCache->Allocate(size);
}

static void ConcurrentFree(void* ptr, size_t size) {
    assert(size <= MAX_BYTES);
    assert(ptr);
    pTLSThreadCache->Deallocate(ptr, size);
}