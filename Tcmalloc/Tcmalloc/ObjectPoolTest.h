//
// Created by devil on 2022-08-09.
//
#pragma once
#include "ObjectPool.h"
#include "Common.h"
#include <ctime>
#include <malloc.h>

const int N = 1000000;
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode() {
        left = nullptr;
        right = nullptr;
        data = 0;
    }
};
void TestObjectPool() {
    ObjectPool<TreeNode> Iintp;
    std::vector<TreeNode*> Vint;
    std::vector<TreeNode*> Mint;
    size_t begin = clock();
    Vint.reserve(N);
    for (int i = 0; i < N; i++) {
        Vint.push_back(Iintp.New());
    }
    for (int i = 0; i < N; i++) {
        Iintp.Delete(Vint[i]);
    }
    size_t end = clock();


    size_t begin2 = clock();
    Mint.reserve(N);
    for (int i = 0; i < N; i++) {
        Mint.push_back((TreeNode*)malloc(sizeof(TreeNode)));
    }
    for (int i = 0; i < N; i++) {
        free(Mint[i]);
    }
    size_t end2 = clock();

    cout << "Malloc cost:" << end2 - begin2 << endl;
    cout << "ObjectPoll cost:" << end - begin << endl;
}