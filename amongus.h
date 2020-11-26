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

    parent.reserve(vertex.size()+1);
    parent.emplace_back(0);
    parent.emplace_back(0);

    for (int i = 1; i < (int)vertex.size(); ++i) {
        double d = numeric_limits<double>::infinity();
        int t = -1;
        for (int j = 0; j < (int)parent.size() - 1; ++j) {
            double x = dist(parent[j], i) + dist(parent[j+1], i) - dist(parent[j], parent[j+1]);
            if (x < d){
                d = x;
                t = j;
            }
        }
        weight += d;
        parent.emplace(parent.begin() + t + 1, i);
    }

    cout << weight << '\n';
    for (int i = 0; i < (int) parent.size() - 1; ++i) {
        cout << parent[i] << ' ';
    }
    cout << '\n';
}

void Graph::runOPTTSP() {

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

    /* ------------------- Approximation --------------------- */
    vector<int> index;
    distance.reserve(vertex.size());
    isVisited.resize(vertex.size(), false);
    index.resize(vertex.size()+1);
    weight = numeric_limits<double>::infinity();
    for (int i = 0; i < (int)vertex.size(); ++i) {
        int p = i;
        cost = 0;
        vector<int> vec;
        vec.reserve(vertex.size() + 1);
        vec.emplace_back(p);
        vec.emplace_back(p);
        for (int j = p + 1; j < p + (int)vertex.size(); ++j) {
            int t = j % (int)vertex.size();
            double val = numeric_limits<double>::infinity();
            int n;
            for (int k = 0; k < (int)vec.size() - 1; ++k) {
                double v = Dis[vec[k]][t] + Dis[vec[k+1]][t] - Dis[vec[k]][vec[k+1]];
                if (v < val) {
                    n = k;
                    val = v;
                }
            }
            cost += val;
            vec.emplace(vec.begin() + 1 + n, t);
        }
        if (cost < weight){
            weight = cost;
            for (int j = 0; j < (int)vertex.size() + 1; ++j) {
                index[j] = vec[j];
            }
        }
    }
    index.pop_back();
    parent = index;
    /* ------------------- Loop on each solution --------------------- */

    cost = 0;
    genPerm(1, index);
    cout << weight << '\n';
    for (int i = 0; i < (int) parent.size(); ++i) {
        cout << parent[i] << ' ';
    }
    cout << '\n';
}

void Graph::genPerm(int pathLength, vector<int> &index) {
    if (pathLength == (int)index.size()){
        cost += Dis[index.front()][index.back()];
        if (cost < weight){
            weight = cost;
            for (int i = 0; i < (int)parent.size(); ++i) {
                parent[i] = index[i];
            }
        }
        cost -= Dis[index.front()][index.back()];
    }
    if (mstLength(index, pathLength) + cost >= weight)
        return;
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
        if (Dis[index[i]][index.front()] < d){
            d = Dis[index[i]][index.front()];
        }
    }
    return re + d;
}
#endif //EECS281_PROJECT4_AMONGUS_H
