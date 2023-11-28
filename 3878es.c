#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct data {
    int x, y;
} Data;

Data nd[MAX], md[MAX];
int nmnm = 0;

int ccw(Data a, Data b, Data c) {
    int n = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (n > 0) return 1;
    if (n < 0) return -1;
    return 0;
}

int cmp(Data a, Data b) {
    int n = ccw((nmnm == 0 ? nd[0] : md[0]), a, b);
    if (n == 0) return a.x + a.y < b.x + b.y;
    if (n > 0) return 1;
    return 0;
}

int big(Data a, Data b) {
    if (a.y == b.y) return a.x > b.x ? 1 : 0;
    return a.y > b.y ? 1 : 0;
}

int intersect(Data a, Data b, Data x, Data y) {
    int ccwa = ccw(a, b, x) * ccw(a, b, y);
    int ccwb = ccw(x, y, a) * ccw(x, y, b);
    if (ccwa == 0 && ccwb == 0) {
        if (big(a, b) > 0) {
            Data temp = a;
            a = b;
            b = temp;
        }
        if (big(x, y) > 0) {
            Data temp = x;
            x = y;
            y = temp;
        }
        return big(b, x) > 0 && big(y, a) > 0;
    }
    return ccwa <= 0 && ccwb <= 0;
}

void swap(Data* a, Data* b) {
    Data temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(Data arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (!cmp(arr[j], arr[j + 1]))
                swap(&arr[j], &arr[j + 1]);
}

void convexHull(Data points[], int n) {
    if (n < 3) return;

    Data hull[MAX];

    int l = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[l].x)
            l = i;

    int p = l, q;
    int hullSize = 0;

    do {
        hull[hullSize++] = points[p];
        q = (p + 1) % n;

        for (int i = 0; i < n; i++) {
            if (ccw(points[p], points[i], points[q]) == 2)
                q = i;
        }

        p = q;

    } while (p != l);
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        int n, m, idx;
        Data nm = { 99999, 99999 }, mm = { 99999, 99999 }, ns[MAX], ms[MAX];
        scanf("%d %d", &n, &m);
        nmnm = 0;
        for (int i = 0; i < n; i++) {
            scanf("%d %d", &nd[i].x, &nd[i].y);
            if (nm.y > nd[i].y || (nm.y == nd[i].y && nm.x > nd[i].x)) {
                nm.x = nd[i].x;
                nm.y = nd[i].y;
                idx = i;
            }
        }
        swap(&nd[0], &nd[idx]);
        bubbleSort(nd + 1, n - 1);
        ns[0] = nd[0];
        idx = 1;
        if (n > 1) {
            ns[1] = nd[1];
            idx = 2;
        }
        for (int i = 2; i < n; i++) {
            while (idx > 1 && ccw(ns[idx - 2], ns[idx - 1], nd[i]) <= 0)
                idx--;
            ns[idx++] = nd[i];
        }
        ns[idx] = ns[0];
        n = idx;
        nmnm = 1;
        for (int i = 0; i < m; i++) {
            scanf("%d %d", &md[i].x, &md[i].y);
            if (mm.y > md[i].y || (mm.y == md[i].y && mm.x > md[i].x)) {
                mm.x = md[i].x;
                mm.y = md[i].y;
                idx = i;
            }
        }
        swap(&md[0], &md[idx]);
        bubbleSort(md + 1, m - 1);
        ms[0] = md[0];
        idx = 1;
        if (m > 1) {
            ms[1] = md[1];
            idx = 2;
        }
        for (int i = 2; i < m; i++) {
            while (idx > 1 && ccw(ms[idx - 2], ms[idx - 1], md[i]) <= 0)
                idx--;
            ms[idx++] = md[i];
        }
        ms[idx] = ms[0];
        m = idx;
        if (n == 1 && m == 1) {
            printf("YES\n");
            continue;
        }
        int itst = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (intersect(ns[i], ns[i + 1], ms[j], ms[j + 1]) == 1) {
                    itst = 1;
                    break;
                }
            }
            if (itst == 1)
                break;
        }
        if (n > 2) {
            int ok = 0;
            for (int i = 0; i < n; i++)
                if (ccw(ns[i], ns[i + 1], ms[0]) != 1)
                    ok = 1;
            if (ok == 0)
                itst = 1;
        }
        if (m > 2) {
            int ok = 0;
            for (int i = 0; i < m; i++)
                if (ccw(ms[i], ms[i + 1], ns[0]) != 1)
                    ok = 1;
            if (ok == 0)
                itst = 1;
        }
        if (itst == 1)
            printf("NO\n");
        else
            printf("YES\n");
    }
    return 0;
}
