#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

#define MAX 100

typedef struct data {
    int x, y;
}Data;

Data nd[MAX], md[MAX];
int nmnm = 0;

int ccw(Data p, Data q, Data r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;  // 일직선
    return (val > 0) ? 1 : 2;  // 시계방향 or 시계반대방향
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
    // 적어도 3개의 점 필요
    if (n < 3) return;

    //  hull이 가장 큰 경우를 저장
    Data hull[MAX];

    // 가장 왼쪽 점 찾음
    int l = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[l].x)
            l = i;

    // 가장 왼쪽 점에서 시작해서 시계 반대 방향으로 이동
    // 시작점에 도달할 때 까지 작동
    int p = l, q;
    int hullSize = 0;

    do {
        // 현재 점을 결과에 더함
        hull[hullSize++] = points[p];

        // 'q'라는 점을 찾습니다. 이때 orientation(p, x, q)가 모든 'x'에 대해 반시계 방향이어야 합니다.
        // 핵심 아이디어는 가장 최근에 방문한 가장 반시계 방향에 있는 'q'를 추적하는 것입니다.
        // 만약 어떤 점 'i'가 'q'보다 더 반시계 방향에 있다면, 'q'를 업데이트합니다.
        q = (p + 1) % n;

        for (int i = 0; i < n; i++) {
            // 만약 어떤 점 'i'가 'q'보다 더 반시계 방향에 있다면, 'q'를 업데이트합니다.
            if (ccw(points[p], points[i], points[q]) == 2)
                q = i;
        }

        // 이제 'q'는 'p'에 대해 가장 반시계 방향에 위치한 점입니다.
        // 다음 반복을 위해 'p'를 'q'로 설정하여, 'q'가 결과 'hull'에 추가되도록 합니다.
        p = q;

    } while (p != l);  // 처음 점에 도착할 때까지 반복
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
