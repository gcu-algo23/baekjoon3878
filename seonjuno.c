#include <stdio.h>
#include <stdlib.h>

#define INITIAL_HULL_SIZE 10

typedef struct {
  int x, y;
} Point;

typedef struct {
  Point *points;
  int size;
  int capacity;
} PointArray;

void initPointArray(PointArray *pa, int initialSize) {
  pa->points = (Point *)malloc(initialSize * sizeof(Point));
  if (pa->points == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  pa->size = 0;
  pa->capacity = initialSize;
}

void insertPointArray(PointArray *pa, Point p) {
  if (pa->size == pa->capacity) {
    pa->capacity *= 2;
    pa->points = (Point *)realloc(pa->points, pa->capacity * sizeof(Point));
    if (pa->points == NULL) {
      fprintf(stderr, "Memory reallocation failed\n");
      exit(EXIT_FAILURE);
    }
  }
  pa->points[pa->size++] = p;
}

void freePointArray(PointArray *pa) {
  free(pa->points);
  pa->points = NULL;
  pa->size = pa->capacity = 0;
}

int compAscXAscY(const void *a, const void *b) {
  Point *p1 = (Point *)a;
  Point *p2 = (Point *)b;
  if (p1->x == p2->x)
    return p1->y - p2->y;
  else
    return p1->x - p2->x;
}

Point basePoint;

int ccw(Point p1, Point p2, Point p3) {
  int val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);
  if (val == 0)
    return 0;
  return (val > 0) ? 1 : -1;
}

int comp(const void *a, const void *b) {
  Point *p1 = (Point *)a;
  Point *p2 = (Point *)b;
  int res = ccw(basePoint, *p1, *p2);
  if (res == 0) {
    if (p1->x == p2->x)
      return p1->y - p2->y;
    else
      return p1->x - p2->x;
  } else {
    return (res > 0) ? -1 : 1;
  }
}

void createConvexHull(PointArray *pa, PointArray *hull) {
  qsort(pa->points, pa->size, sizeof(Point), compAscXAscY);
  basePoint = pa->points[0];

  qsort(pa->points + 1, pa->size - 1, sizeof(Point), comp);

  insertPointArray(hull, pa->points[0]);
  if (pa->size == 1)
    return;
  insertPointArray(hull, pa->points[1]);

  for (int i = 2; i < pa->size; i++) {
    while (hull->size >= 2) {
      Point secondLast = hull->points[hull->size - 2];
      Point last = hull->points[hull->size - 1];
      if (ccw(secondLast, last, pa->points[i]) > 0)
        break;
      hull->size--;
    }
    insertPointArray(hull, pa->points[i]);
  }
}

int isInside(Point *hull, int hullSize, Point p) {
  if (hullSize <= 2)
    return 0;
  int cwBase = ccw(hull[0], hull[1], p);
  for (int i = 1; i < hullSize; i++) {
    int cw = ccw(hull[i], hull[(i + 1) % hullSize], p);
    if (cw != cwBase)
      return 0;
  }
  return 1;
}

int isCross(Point a, Point b, Point c, Point d) {
  int ab = ccw(a, b, c) * ccw(a, b, d);
  int cd = ccw(c, d, a) * ccw(c, d, b);
  if (ab == 0 && cd == 0) {
    if (a.x > b.x || (a.x == b.x && a.y > b.y)) {
      Point temp = a;
      a = b;
      b = temp;
    }
    if (c.x > d.x || (c.x == d.x && c.y > d.y)) {
      Point temp = c;
      c = d;
      d = temp;
    }
    return c.x <= b.x && a.x <= d.x && c.y <= b.y && a.y <= d.y;
  } else {
    return ab <= 0 && cd <= 0;
  }
}

int isDivide(PointArray *blackHull, PointArray *whiteHull) {
  if (isInside(blackHull->points, blackHull->size, whiteHull->points[0]) ||
      isInside(whiteHull->points, whiteHull->size, blackHull->points[0]))
    return 0;

  for (int i = 0; i < blackHull->size; i++) {
    for (int j = 0; j < whiteHull->size; j++) {
      if (isCross(blackHull->points[i],
                  blackHull->points[(i + 1) % blackHull->size],
                  whiteHull->points[j],
                  whiteHull->points[(j + 1) % whiteHull->size])) {
        return 0;
      }
    }
  }
  return 1;
}

int main(void) {
  int t, n, m;
  scanf("%d", &t);

  while (t--) {
    scanf("%d %d", &n, &m);
    PointArray blackPoints, whitePoints, blackHull, whiteHull;
    initPointArray(&blackPoints, n);
    initPointArray(&whitePoints, m);
    initPointArray(&blackHull, INITIAL_HULL_SIZE);
    initPointArray(&whiteHull, INITIAL_HULL_SIZE);

    for (int i = 0; i < n; i++) {
      Point p;
      scanf("%d %d", &p.x, &p.y);
      insertPointArray(&blackPoints, p);
    }

    for (int i = 0; i < m; i++) {
      Point p;
      scanf("%d %d", &p.x, &p.y);
      insertPointArray(&whitePoints, p);
    }

    createConvexHull(&blackPoints, &blackHull);
    createConvexHull(&whitePoints, &whiteHull);

    if (isDivide(&blackHull, &whiteHull)) {
      printf("YES\n");
    } else {
      printf("NO\n");
    }

    freePointArray(&blackPoints);
    freePointArray(&whitePoints);
    freePointArray(&blackHull);
    freePointArray(&whiteHull);
  }

  return 0;
}
