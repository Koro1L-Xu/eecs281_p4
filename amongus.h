// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7
#ifndef EECS281_PROJECT4_AMONGUS_H
#define EECS281_PROJECT4_AMONGUS_H

#include <iostream>
#include <cassert>
#include <limits>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

typedef struct vertexEntry{
    int x;
    int y;
    vertexEntry(int i = 0, int j = 0) : x(i), y{j} {}
}vertexEntry;

class Graph{
private:
    vector<vertexEntry> vertex;
    vector<char> state;
    vector<bool> isVisited;
    vector<int> parent;
    vector<double> distance;
    vector<vector<double>> Dis;
    double weight = 0;
    double cost = 0;

public:
    char mode = '0';
    void readVertex();
    double dist(int i, int j);
    void runMST();
    double mstLength(const vector<int> &index, int n);
    void runFASTTSP();
    void runOPTTSP();
    void genPerm(int pathLength, vector<int>& index);
    static void reverse_int(int s, int e, vector<int>& index);
    static void reverse_double(int s, int e, vector<double>& index);
};

void Graph::readVertex() {
    int N;
    cin >> N;
    vertex.resize(N);
    if (mode != 'M'){
        for (int i = 0; i < N; ++i) {
            cin >> vertex[i].x >> vertex[i].y;
        }
    } else{
        state.reserve(N);
        for (int i = 0; i < N; ++i) {
            cin >> vertex[i].x >> vertex[i].y;
            if (vertex[i].x < 0 && vertex[i].y < 0) {
                state.emplace_back('i');
            }
            else if (vertex[i].x > 0 || vertex[i].y > 0){
                state.emplace_back('o');
            } else
                state.emplace_back('m');
        }
    }
}

double Graph::dist(int i, int j) {
    double x = (double)vertex[i].x - (double)vertex[j].x;
    double y = (double)vertex[i].y - (double)vertex[j].y;
    return sqrt(x*x + y*y);
}

void Graph::runMST() {

    isVisited.resize(vertex.size(), false);
    parent.resize(vertex.size());
    distance.resize(vertex.size(), numeric_limits<double>::infinity());
    /*------------------- update the vertex ---------------------*/
    distance[0] = 0;
    for (int fuck = 0; fuck < (int)vertex.size(); ++fuck) {
        double d(numeric_limits<double>::infinity());
        int start = 0;
        for (int i = 0; i < (int)vertex.size(); ++i) {
            if (!isVisited[i] && distance[i] < d) {
                d = distance[i];
                start = i;
            }
        }
        isVisited[start] = true;
        weight += distance[start];
        if (state[start] == 'o'){
            for (int j = 0; j < (int)vertex.size(); ++j) {
                if (state[j] != 'i' && !isVisited[j]){
                    double dis = dist(start,j);
                    if ( dis < distance[j]){
                        parent[j] = start;
                        distance[j] = dis;
                    }
                }
            }
        } else if (state[start] == 'i'){
            for (int j = 0; j < (int)vertex.size(); ++j) {
                if (state[j] != 'o' && !isVisited[j]){
                    double dis = dist(start,j);
                    if ( dis < distance[j]){
                        parent[j] = start;
                        distance[j] = dis;
                    }
                }
            }
        } else{
            for (int j = 0; j < (int)vertex.size(); ++j) {
                if (!isVisited[j]){
                    double dis = dist(start,j);
                    if ( dis < distance[j]){
                        parent[j] = start;
                        distance[j] = dis;
                    }
                }
            }
        }
    }
    /* ------------------- Output --------------------- */
    cout << weight << '\n';
    for (int i = 1; i < (int) parent.size(); ++i) {
        if (distance[i] < numeric_limits<double>::infinity()) {
            int M = i > parent[i] ? i : parent[i];
            cout << i + parent[i] - M << ' ' << M << '\n';
        }
    }
}

void Graph::runFASTTSP() {

    distance.reserve(vertex.size());
    isVisited.resize(vertex.size(), false);
    parent.reserve(vertex.size()+1);
    int p = 0;
    while (true){
        isVisited[p] = true;
        parent.emplace_back(p);
        int t = -1;
        double dis = numeric_limits<double>::infinity();
        for (int i = 0; i < (int)vertex.size(); ++i) {
            if (!isVisited[i]){
                double d = dist(p, i);
                if (d < dis){
                    t = i;
                    dis = d;
                }
            }
        }
        if (t == -1)
            break;
        else {
            distance.emplace_back(dis);
            weight += dis;
            p = t;
        }
    }
    distance.emplace_back(dist(0, p));
    weight += distance.back();
    parent.emplace_back(0);

    for (int i = 1; i < (int) parent.size() - 2; ++i) {
        for (int j = i + 1; j < (int) parent.size() - 1; ++j) {
            double d1 = dist(parent[i], parent[j+1]);
            double d2 = dist(parent[i-1], parent[j]);
            if (distance[i-1] + distance[j] > d1 + d2){
                reverse_int(i, j, parent);
                reverse_double(i, j-1, distance);
                weight += - distance[i-1] - distance[j] + d2 + d1;
                distance[i-1] = d2; distance[j] = d1;
            }
        }
    }
    cout << weight << '\n';
    for (int i = 0; i < (int) parent.size() - 1; ++i) {
        cout << parent[i] << ' ';
    }
    cout << '\n';
}

void Graph::runOPTTSP() {

    /* ------------------- Approximation --------------------- */
    vector<int> index;
    distance.reserve(vertex.size());
    isVisited.resize(vertex.size(), false);
    index.reserve(vertex.size()+1);
    int p = 0;
    while (true){
        isVisited[p] = true;
        index.emplace_back(p);
        int t = -1;
        double dis = numeric_limits<double>::infinity();
        for (int i = 0; i < (int)vertex.size(); ++i) {
            if (!isVisited[i]){
                double d = dist(p, i);
                if (d < dis){
                    t = i;
                    dis = d;
                }
            }
        }
        if (t == -1)
            break;
        else {
            distance.emplace_back(dis);
            weight += dis;
            p = t;
        }
    }
    distance.emplace_back(dist(0, p));
    weight += distance.back();
    index.emplace_back(0);
    for (int i = 1; i < (int) index.size() - 2; ++i) {
        for (int j = i + 1; j < (int) index.size() - 1; ++j) {
            double d1 = dist(index[i], index[j+1]);
            double d2 = dist(index[i-1], index[j]);
            if (distance[i-1] + distance[j] > d1 + d2){
                reverse_int(i, j, index);
                reverse_double(i, j-1, distance);
                weight += - distance[i-1] - distance[j] + d2 + d1;
                distance[i-1] = d2; distance[j] = d1;
            }
        }
    }
    index.pop_back();
    parent = index;
    /* ------------------- Loop on each solution --------------------- */

    Dis.resize(vertex.size());
    for (int i = 0; i < (int) Dis.size(); ++i) {
        Dis[i].resize(vertex.size());
        for (int j = 0; j < i + 1; ++j) {
            Dis[i][j] = dist(i, j);
        }
    }
    for (int i = 0; i < (int) Dis.size(); ++i) {
        for (int j = i+1; j < (int) Dis.size(); ++j) {
            Dis[i][j] = Dis[j][i];
        }
    }
    cost = 0;
    genPerm(1, index);
    cout << weight << '\n';
    for (int i = 0; i < (int) parent.size(); ++i) {
        cout << parent[i] << ' ';
    }
    cout << '\n';
}

void Graph::reverse_int(int s, int e, vector<int> &index) {
    while (s < e){
        swap(index[s], index[e]);
        s++; e--;
    }
}

void Graph::reverse_double(int s, int e, vector<double> &index) {
    while (s < e){
        swap(index[s], index[e]);
        s++; e--;
    }
}

void Graph::genPerm(int pathLength, vector<int> &index) {
    if (pathLength == (int)index.size()){
        cost += Dis[0][index.back()];
        if (cost < weight){
            weight = cost;
            for (int i = 0; i < (int)parent.size(); ++i) {
                parent[i] = index[i];
            }
        }
        cost -= Dis[0][index.back()];
    }
    if ((int)index.size() - pathLength >= 1){
        if (mstLength(index, pathLength) + cost >= weight)
            return;
    }
    for (int i = pathLength; i < (int)index.size(); ++i) {
        swap(index[pathLength], index[i]);
        cost += Dis[index[pathLength]][index[pathLength-1]];
        genPerm(pathLength + 1, index);
        cost -= Dis[index[pathLength]][index[pathLength-1]];
        swap(index[pathLength], index[i]);
    }

}

double Graph::mstLength(const vector<int> &index, int n) {
    double re = 0;
    isVisited.resize((int)index.size() - n);
    for (int i = 0; i < (int)index.size() - n; ++i) {
        isVisited[i] = false;
    }
    distance.resize((int)index.size() - n);
    for (int i = 0; i < (int)index.size() - n; ++i) {
        distance[i] = numeric_limits<double>::infinity();
    }
    /*------------------- update the vertex ---------------------*/
    distance[0] = 0;
    for (int fuck = 0; fuck < (int)index.size() - n; ++fuck) {
        double d(numeric_limits<double>::infinity());
        int start = 0;
        for (int i = 0; i < (int)index.size() - n; ++i) {
            if (!isVisited[i] && distance[i] < d) {
                d = distance[i];
                start = i;
            }
        }
        isVisited[start] = true;
        re += distance[start];
        for (int j = 0; j < (int)index.size() - n; ++j) {
            if (!isVisited[j]){
                double dis = Dis[index[start+n]][index[n+j]];
                if ( dis < distance[j]){
                    distance[j] = dis;
                }
            }
        }
    }
    double d = numeric_limits<double>::infinity();
    for (int i = n; i < (int)index.size(); ++i) {
        if (Dis[index[i]][index[n-1]] < d){
            d = Dis[index[i]][index[n-1]];
        }
    }
    re += d;
    d = numeric_limits<double>::infinity();
    for (int i = n; i < (int)index.size(); ++i) {
        if (Dis[index[i]][0] < d){
            d = Dis[index[i]][0];
        }
    }
    return re + d;
}
#endif //EECS281_PROJECT4_AMONGUS_H
