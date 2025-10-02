#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <chrono>
#include <cstring>
#include <cassert>
#include <algorithm>

struct Node {
    size_t idx;
    Node*  next;
    Node(size_t i) : idx(i), next(nullptr) {}
};

struct LinkedList {
    Node* head = nullptr;
    Node* tail = nullptr;
    size_t sz  = 0;

    inline void push_back(size_t i) {
        Node* nd = new Node(i);
        if (!head) head = tail = nd;
        else { tail->next = nd; tail = nd; }
        ++sz;
    }
    
    struct It {
        Node* cur;
        bool operator!=(const It& o) const { return cur != o.cur; }
        void operator++() { cur = cur->next; }
        size_t operator*() const { return cur->idx; }
    };
    It begin() const { return It{head}; }
    It end() const { return It{nullptr}; }

    void clear_nodes() {
        Node* p = head;
        while (p) { Node* nx = p->next; delete p; p = nx; }
        head = tail = nullptr; sz = 0;
    }
    ~LinkedList(){ clear_nodes(); }
};

// Lectura y borrado.
Poscode *readCodes(const std::string &strfile, size_t n){
    Poscode *codes = new Poscode[n];

    std::ifstream inputFile(strfile);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the file!" << std::endl;
        return nullptr;
    }
    std::string line;
    for(size_t i = 0; i < n; i++){
        std::getline(inputFile, line);
        codes[i] = Poscode(line);
    }

    inputFile.close();
    return codes;
}

void deleteCodes(Poscode *codes){
    if (codes != nullptr) {
        delete[] codes;
    }
}

// Helpers de ordenamiento.
static inline bool pos_less(const Poscode& a, const Poscode& b){
    return a.getData() < b.getData();
}

static inline int get_key_at_pos(const Poscode& c, int p){
    assert(p >= 1 && p <= 6);
    char ch = c.getValue(static_cast<size_t>(p-1));
    if (p <= 4){
        return static_cast<int>(ch - '0');
    }else{
        return static_cast<int>(ch - 'A');
    }
}

static void counting_sort_by(Poscode* A, Poscode* T, size_t n, int p, int M){
    struct Node { size_t idx; Node* next; };
    struct LL   { Node* head=nullptr; Node* tail=nullptr; };

    std::vector<Node> pool(n);
    size_t pool_pos = 0;
    std::vector<LL> buckets(static_cast<size_t>(M));

    auto push = [&](LL& ll, size_t i){
        Node* nd = &pool[pool_pos++];
        nd->idx = i;
        nd->next = nullptr;
        if (!ll.head) ll.head = ll.tail = nd;
        else { ll.tail->next = nd; ll.tail = nd; }
    };

    for (size_t i = 0; i < n; ++i){
        int key = get_key_at_pos(A[i], p);
        push(buckets[static_cast<size_t>(key)], i);
    }

    size_t k = 0;
    for (int b = 0; b < M; ++b){
        for (Node* nd = buckets[static_cast<size_t>(b)].head; nd; nd = nd->next){
            T[k++] = std::move(A[nd->idx]);
        }
    }

    for (size_t i = 0; i < n; ++i){
        A[i] = std::move(T[i]);
    }
}

// RadixSort (LSD) usando CountingSort.
void radix_sort(Poscode *A, size_t n){
    Poscode* T = new Poscode[n];

    counting_sort_by(A, T, n, 6, 26);
    counting_sort_by(A, T, n, 5, 26);
    for (int p = 4; p >= 1; --p){
        counting_sort_by(A, T, n, p, 10);
    }

    delete[] T;
}

// QuickSort.
static inline void swap_pos(Poscode& a, Poscode& b){
    Poscode tmp = a; a = b; b = tmp;
}

static size_t partition(Poscode* A, size_t lo, size_t hi){
    // Último como pivote.
    const Poscode& pivot = A[hi];
    size_t i = lo;
    for (size_t j = lo; j < hi; ++j){
        if (pos_less(A[j], pivot)){
            swap_pos(A[i], A[j]);
            ++i;
        }
    }
    swap_pos(A[i], A[hi]);
    return i;
}

void quick_sort(Poscode *A, size_t n){
    if (n <= 1) return;
    std::stack<std::pair<size_t,size_t>> st;
    st.push({0, n-1});

    while (!st.empty()){
        std::pair<size_t,size_t> top = st.top();
        st.pop();
        size_t lo = top.first;
        size_t hi = top.second;

        if (lo >= hi) continue;

        size_t p = partition(A, lo, hi);

        if (p > 0){
            size_t left_lo = lo, left_hi = p - 1;
            size_t right_lo = p + 1, right_hi = hi;

            size_t left_sz = (left_hi >= left_lo) ? (left_hi - left_lo + 1) : 0;
            size_t right_sz = (right_hi >= right_lo)? (right_hi - right_lo + 1): 0;

            if (left_sz < right_sz){
                if (right_lo <= right_hi) st.push({right_lo, right_hi});
                if (left_lo <= left_hi ) st.push({left_lo, left_hi});
            }else{
                if (left_lo <= left_hi ) st.push({left_lo, left_hi});
                if (right_lo <= right_hi) st.push({right_lo, right_hi});
            }
        }else{
            if (p + 1 <= hi) st.push({p + 1, hi});
        }
    }
}

// MergeSort.
void merge_sort(Poscode *A, size_t n){
    if (n <= 1) return;
    Poscode* B = new Poscode[n];

    for (size_t width = 1; width < n; width <<= 1){
        for (size_t i = 0; i < n; i += (width << 1)){
            size_t left = i;
            size_t mid = std::min(i + width, n);
            size_t right = std::min(i + (width << 1), n);

            size_t p = left, q = mid, k = left;
            while (p < mid && q < right){
                if (!pos_less(A[q], A[p])){
                    B[k++] = A[p++];
                }else{
                    B[k++] = A[q++];
                }
            }
            while (p < mid) B[k++] = A[p++];
            while (q < right) B[k++] = A[q++];
        }
        for (size_t i = 0; i < n; ++i) {
            A[i] = B[i];
        }
    }

    delete[] B;
}