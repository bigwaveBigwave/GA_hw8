#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

// 메모를 하며 cost를 계산하는 함수. 자식 해를 계산할 때 쓴다. local search를 위해 메모를 할 필요가 있기 때문. 
int calcMemCost(int n, vector<int>& memo, vector<int>& child, vector<pair<int, int>> v[])
{
    if (memo[n] > 0)return memo[n];

    int cost;
    int sum = 0;
    for (int j = 0; j < v[n].size(); j++) {


        if (child[n] != child[v[n][j].first] && n > v[n][j].first) sum += v[n][j].second;
    }
    if (n == 1) return 0;
    else cost = (calcMemCost(n - 1, memo, child, v)) + sum;

    memo[n] = cost;
    return cost;


}


// 메모를 하지 않고 cost를 재귀로 구하는 함수. k부터 하나씩 줄어들다가 end가 되면 끝남. 
int calcCost(int k, int r, vector<int>& child, vector<pair<int, int>> v[])
{
    if (k == r - 1) return 0;
    int cost = 0;
    int sum = 0;
    for (int j = 0; j < v[k].size(); j++) {
        if (child[k] != child[v[k][j].first] && k > v[k][j].first) sum += v[k][j].second;
    }

    if (k == 1 || k == 0) cost = 0;
    else cost = calcCost(k - 1, r, child, v) + sum;
    return cost;
}


// 하나만 달라지는 해의 표현의 cost. memo와 재귀 사용해서 구하기 
// r이 그룹이 바뀌는 노드임. n은 전체 노드 수
int calcLocal(int n, vector<int>& newX, int r, vector<int>& memo, vector<pair<int, int>> v[])
{

    int rest = calcCost(n, r, newX, v);
    return memo[r - 1] + rest;
}

int main() {

    int node, edge;

    double p;

    srand(time(NULL));


    vector <pair<int, int>> v[1001];

    ifstream fp("test.txt");

    ofstream fout("maxcut.out");

    if (fout.fail()) {


        cerr << "파일 열기 에러" << endl; return -1;
    }

    if (fp.is_open()) {
        fp >> node >> edge;
        v->resize(node + 1);
        for (int i = 0; i < edge; i++) {
            int n1, n2, weight;
            fp >> n1 >> n2 >> weight;
            v[n1].push_back(make_pair(n2, weight));
            v[n2].push_back(make_pair(n1, weight));
        }
    }
    else {
        cout << "no file exist" << endl;
        return -1;

    }

    int parentNum = 10;

    int cost[10];
    vector<vector<int>> parents(parentNum, vector<int>(node + 1));

    for (int i = 0; i < parentNum; i++) {
        for (int j = 1; j <= node; j++) {
            parents[i][j] = rand() % 2;
        }
    }


    for (int k = 0; k < parentNum; k++) {

        int sum = 0;
        for (int i = 1; i <= node; i++) {
            for (int j = 0; j < v[i].size(); j++) {
                if (parents[k][i] != parents[k][v[i][j].first] && i < v[i][j].first) sum += v[i][j].second;


            }
        }
        cost[k] = sum;
    }

    // 부모 2개를 선택. 토너먼트 방식으로 함. 

    int childCost[5];
    int childNum = 5;

    vector<vector<int>> child(childNum, vector<int>(node + 1));
    int maxP = 0;
    int countNum = 0;

    do {


        for (int a = 0; a < childNum; ++a) {
            int n1, n2, s1, s2;
            do {
                n1 = rand() % parentNum;
                n2 = rand() % parentNum;
            } while (n1 == n2);

            int randNum = rand() % 100;
            if (randNum < 60) s1 = (cost[n1] < cost[n2] ? n1 : n2);

            else s1 = (cost[n1] > cost[n2] ? n1 : n2);

            do {
                n1 = rand() % parentNum;
                n2 = rand() % parentNum;
            } while (n1 == n2 || n1 == s1 || n2 == s1);

            randNum = rand() % 100;
            if (randNum < 60) s2 = (cost[n1] < cost[n2] ? n1 : n2);
            else s2 = (cost[n1] > cost[n2] ? n1 : n2);

            // 반반해서 자식 만들기
            for (int i = 1; i <= node; i++) {
                if (i <= (node / 2)) child[a][i] = parents[s1][i];
                else child[a][i] = parents[s2][i];
            }

            // mutation


            for (int i = 1; i <= node; i++) {
                randNum = rand() % 1000;
                if (randNum < 1) {
                    if (child[a][i] == 0) child[a][i] = 1;
                    else child[a][i] = 0;

                }
            }

            // 만들어진 세대의 cost 구하기 -> 함수로 위에 있음. 재귀로 구현
           // memoization 배열 memo[]


            vector<int> memo(node + 1);
            for (int i = 0; i < memo.size(); i++) memo[i] = 0;

            childCost[a] = calcMemCost(node, memo, child[a], v);



            // 지역 최적화 
            
            // 노드 크기만큼의 무작위 순열 만들기


            vector<int> rand;
            for (int i = 1; i <= node; ++i) {


                rand.push_back(i);
            }
            random_shuffle(rand.begin(), rand.end());


            // pesudo code 2행부터 
            // 
            // i는 무작위 순열 rand를 순회 


            vector<int> candidate(child[a]);
            int candiCost = childCost[a];
            bool improved = true;
            while (improved) {
                improved = false;
                for (int i = 0; i < node; i++) {
                    // 노드 rand[i]를 반대 그룹으로 바꾼 새로운 해 만들기

                    vector<int> newX(child[a]);
                    int newC = 0;
                    newX[rand[i]] = 1 - newX[rand[i]];

                    //새로운 해의 cost 구하기
                    int newCost = calcLocal(node, newX, rand[i], memo, v);

                    // 새로운 해가 더 나으면 바꾸기
                    if (newCost - candiCost > 0) {
                        candidate[rand[i]] = 1 - child[a][rand[i]];
                        candiCost = newCost;
                        improved = true;

                    }
                }
            }

            for (int i = 1; i <= node; i++) child[a][i] = candidate[i];
            childCost[a] = candiCost;
        }
        // replace

        int minP = 0;
        for (int i = 1; i < parentNum; i++)
            if (cost[minP] > cost[i]) {
                minP = i;
            }

        for (int i = 0; i < childNum; i++) {
            if (cost[minP] < childCost[i]) {
                cost[minP] = childCost[i];
                for (int k = 0; k < node; k++) {
                    parents[minP][k] = child[i][k];

                }

                minP = 0;
                for (int k = 1; k < parentNum; k++) if (cost[minP] > cost[i]) minP = i;
            }
        }

    } while (countNum++ != 100); // 반복할 세대의 횟수



    // 세대 중에 제일 큰 값 찾기

    for (int i = 0; i < parentNum; i++) {
        if (cost[maxP] < cost[i]) maxP = i;
    }
    for (int k = 1; k <= node; k++) cout << parents[maxP][k] << " ";
    
    cout << endl;

    // 결과 출력

    for (int k = 1; k <= node; k++) {

        if (parents[maxP][k] == 1) fout << k << " ";
    }
    fout << cost[maxP] << endl;


    fp.close();
}


