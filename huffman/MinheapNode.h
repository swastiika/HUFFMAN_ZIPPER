#ifndef MINHEAPNODE_H
#define MINHEAPNODE_H
struct MinHeapNode {
    char data;
    unsigned freq;
    MinHeapNode *left, *right;
    MinHeapNode(char data, unsigned freq) {
        this->data = data;
        this->freq = freq;
        left = right = nullptr;
    }
};
struct compare {
    bool operator()(MinHeapNode* l, MinHeapNode* r) {
        return l->freq > r->freq;
    }
};
#endif
