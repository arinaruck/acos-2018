#include <stdio.h>
#include <stdlib.h>


int cmp(const void* a_v, const void* b_v, void* user_v) {
    const int* data = (const int*)user_v;
    int i = *(const int*)a_v;
    int j = *(const int*)b_v;
    int l = data[i];
    int r = data[j];
    if (l == r) {
        return (j < i) - (i < j);
    }
    return (r < l) - (l < r);
}
void process(size_t count, const int* data, int* order) {
    for (int i = 0; i < (int)count; ++i) {
        order[i] = i;
    }
    qsort_r(order, count, sizeof(int), cmp, (void*)data);
}

int main() {
    int data[5] = {5,4,3,2,1};
    int order[5];
    process(5, data, order);
    for (int i = 0; i < 5; ++i) {
        printf("%d ", order[i]);
    }
}