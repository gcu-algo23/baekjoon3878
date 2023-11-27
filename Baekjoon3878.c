#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_POINTS 10000

typedef long long ll;

typedef struct {
    ll x, y;
} Point;

typedef struct {
    Point* points;
    int size;
} PointArray;

Point pivot;



int ccw(Point a, Point b, Point c) {
    ll res = a.x * b.y + b.x * c.y + c.x * a.y;
    res -= b.x * a.y + c.x * b.y + a.x * c.y;
    if (res > 0) return 1;
    if (res) return -1;
    return 0;
}

ll dist(Point a, Point b) {
    ll dx = a.x - b.x;
    ll dy = a.y - b.y;
    return dx * dx + dy * dy;
}

void init(PointArray* arr) {
    arr->points = NULL;
    arr->size = 0;
}

int compare(const void* a, const void* b) {
    Point p1 = *((Point*)a);
    Point p2 = *((Point*)b);

    int orientation = ccw(pivot, p1, p2);

    if (orientation == 0) {
        return (dist(pivot, p2) >= dist(pivot, p1)) ? -1 : 1;
    }

    return (orientation == 1) ? -1 : 1;
}

// �Ѳ��� ���ϱ�
void getHull(PointArray* arr, PointArray* hull) {
    int n = arr->size;

    // Graham's Scan�� ���� ������ ã��
    pivot = arr->points[0];
    int pivotIndex = 0;
    for (int i = 1; i < n; i++) {
        if (arr->points[i].y < pivot.y || (arr->points[i].y == pivot.y && arr->points[i].x < pivot.x)) {
            pivot = arr->points[i];
            pivotIndex = i;
        }
    }

    // �������� �迭�� ù ��° ���ҷ� �̵�
    Point temp = arr->points[0];
    arr->points[0] = arr->points[pivotIndex];
    arr->points[pivotIndex] = temp;

    // �������� �������� ����
    qsort(&arr->points[1], n - 1, sizeof(Point), compare);

    // ���� ���� ���
    hull->size = 0;

    for (int i = 0; i < n; i++) {
        while (hull->size >= 2 && ccw(hull->points[hull->size - 2], hull->points[hull->size - 1], arr->points[i]) <= 0) {
            hull->size--;
        }
        hull->points[hull->size++] = arr->points[i];
    }
}

// ���� �� now�� �Ѳ��� ���� �ִ��� Ȯ��
bool chk(Point now, PointArray* arr) {
    int cnt = 0;

    Point q = now;

    for (int i = 0; i < arr->size; i++) {
        int j = (i + 1) % arr->size;

        Point ii = arr->points[i];
        Point jj = arr->points[j];

        if ((ii.y > q.y) != (jj.y > q.y)) {
            double X = (jj.x - ii.x) * (q.y - ii.y) / (double)(jj.y - ii.y) + ii.x;
            if (q.x < X) {
                cnt++;
            }
        }
    }

    return cnt % 2 > 0;
}
int disjoint(ll a, ll b, ll c, ll d) {
    if (a > b) {
        ll temp = a;
        a = b;
        b = temp;
    }
    if (c > d) {
        ll temp = c;
        c = d;
        d = temp;
    }
    return b < c || d < a;
}

bool crash(Point a, Point b, Point c, Point d) {
    int ab = ccw(a, b, c) * ccw(a, b, d);
    int cd = ccw(c, d, a) * ccw(c, d, b);
    if (ab == 0 && cd == 0) {
        return !disjoint(a.x, b.x, c.x, d.x) && !disjoint(a.y, b.y, c.y, d.y);
    }
    return ab <= 0 && cd <= 0;
}

void initPointArray(PointArray* arr, int capacity) {
    arr->size = 0;
    arr->points = (Point*)malloc(sizeof(Point) * capacity);
}

void freePointArray(PointArray* arr) {
    free(arr->points);
    arr->size = 0;
    arr->points = NULL;
}

void solve() {
    PointArray a, b, aHull, bHull;
    int n, m;

    // �Է� �ޱ�
    scanf("%d %d", &n, &m);

    // a �迭�� b �迭 �ʱ�ȭ
    init(&a);
    init(&b);

    // a �迭�� b �迭 ���� �Ҵ�
    a.points = (Point*)malloc(sizeof(Point) * n);
    b.points = (Point*)malloc(sizeof(Point) * m);

    // a �迭�� b �迭 �Է� �ޱ�
    for (int i = 0; i < n; i++) {
        Point p;
        scanf("%lld %lld", &p.x, &p.y);
        a.points[i] = p;
        a.size++;
    }

    for (int i = 0; i < m; i++) {
        Point p;
        scanf("%lld %lld", &p.x, &p.y);
        b.points[i] = p;
        b.size++;
    }

    initPointArray(&aHull, 100);
    initPointArray(&bHull, 100);

    // aHull, bHull ���
    getHull(&a, &aHull);
    getHull(&b, &bHull);

    // ���� ���� Ȯ��
    int ans = 0;

    if (chk(aHull.points[0], &bHull) || chk(bHull.points[0], &aHull)) {
        ans = 1;
    }

    for (int i = 0; i < aHull.size; i++) {
        if (chk(aHull.points[i], &bHull)) {
            ans = 1;
        }
    }

    for (int i = 0; i < bHull.size; i++) {
        if (chk(bHull.points[i], &aHull)) {
            ans = 1;
        }
    }

    n = aHull.size;
    m = bHull.size;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (crash(aHull.points[i], aHull.points[(i + 1) % n], bHull.points[j], bHull.points[(j + 1) % m])) {
                ans = 1;
            }
        }
    }

    // ��� ���
    if (ans) {
        printf("NO\n");
    }
    else {
        printf("YES\n");
    }

    // �޸� ����
    freePointArray(&a);
    freePointArray(&b);
    freePointArray(&aHull);
    freePointArray(&bHull);
}

int main() {
    int t;
    scanf("%d", &t);

    while (t--) {
        solve();
    }

    return 0;
}