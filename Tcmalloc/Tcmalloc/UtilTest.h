//
// Created by devil on 2022-08-09.
//
#pragma once

#include "ConcurrentAlloc.h"

void Alloc1() {
    for (int i = 0; i < 5; i++) {
        ConcurrentAlloc(6);
    }
    
}
void Alloc2() {
    for (int i = 0; i < 5; i++) {
        ConcurrentAlloc(7);
    }
}
void TestConcurrentAlloc() {
    thread t1(Alloc1);
    thread t2(Alloc2);
    t1.join();
    t2.join();
}