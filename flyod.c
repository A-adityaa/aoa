#include <stdio.h>
#include <limits.h>

#define MAX 10
#define INF 9999

int n;
int g[MAX][MAX];
int dist[MAX][MAX];
int visited[MAX];
int path[MAX];
int temp[MAX];
int minCost = INF;

void dfs(int s, int seen[]) {
    seen[s] = 1;
    for (int i = 0; i < n; i++) {
        if (g[s][i] != INF && !seen[i]) {
            dfs(i, seen);
        }
    }
}

void floydWarshall() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = g[i][j];
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

void tsp(int pos, int count, int cost) {
    if (count == n && dist[pos][0] != INF) {
        int total = cost + dist[pos][0];
        if (total < minCost) {
            minCost = total;
            for (int i = 0; i < n; i++)
                path[i] = temp[i];
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i] && dist[pos][i] != INF) {
            visited[i] = 1;
            temp[count] = i;
            tsp(i, count + 1, cost + dist[pos][i]);
            visited[i] = 0;
        }
    }
}

int mstCost() {
    int sel[MAX] = {0};
    int edges = 0, total = 0;
    sel[0] = 1;

    while (edges < n - 1) {
        int min = INF, u = 0, v = 0;
        for (int i = 0; i < n; i++) {
            if (sel[i]) {
                for (int j = 0; j < n; j++) {
                    if (!sel[j] && dist[i][j] < min) {
                        min = dist[i][j];
                        u = i;
                        v = j;
                    }
                }
            }
        }
        total += min;
        sel[v] = 1;
        edges++;
    }
    return total;
}

int main() {
    printf("Enter number of tourist spots: ");
    scanf("%d", &n);

    printf("Enter cost matrix (-1 for no direct path):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int x;
            scanf("%d", &x);
            if (x == -1)
                g[i][j] = INF;
            else
                g[i][j] = x;
        }
    }

    int seen[MAX] = {0};
    dfs(0, seen);
    int disconn = 0;
    for (int i = 0; i < n; i++) {
        if (!seen[i]) disconn = 1;
    }

    if (disconn) {
        printf("\nGraph is disconnected! Some spots not reachable.\n");
        printf("Hamiltonian path not possible.\n");
        return 0;
    }

    floydWarshall();

    printf("\nGraph is connected.\n");
    printf("\nShortest Distances (Floyd–Warshall):\n");

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (dist[i][j] != INF)
                printf("%c -> %c = %d\n", 'A' + i, 'A' + j, dist[i][j]);
        }
    }

    for (int i = 0; i < n; i++) visited[i] = 0;
    visited[0] = 1;
    temp[0] = 0;

    tsp(0, 1, 0);

    if (minCost == INF)
        printf("\nNo Hamiltonian cycle found.\n");
    else {
        printf("\nHamiltonian Cycle: ");
        for (int i = 0; i < n; i++)
            printf("%c -> ", 'A' + path[i]);
        printf("A\n");
        printf("Total Cost = %d\n", minCost);
    }

    int mst = mstCost();
    int approx = 2 * mst;

    printf("\nApproximation using MST = %d\n", approx);
    printf("\nComparison:\nExact (Backtracking) = %d\nMST Approximation = %d\n", minCost, approx);

    return 0;
}
