#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

// �޸� �ϸ� cost�� ����ϴ� �Լ�. �ڽ� �ظ� ����� �� ����. local search�� ���� �޸� �� �ʿ䰡 �ֱ� ����. 
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


// �޸� ���� �ʰ� cost�� ��ͷ� ���ϴ� �Լ�. k���� �ϳ��� �پ��ٰ� end�� �Ǹ� ����. 
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


// �ϳ��� �޶����� ���� ǥ���� cost. memo�� ��� ����ؼ� ���ϱ� 
// r�� �׷��� �ٲ�� �����. n�� ��ü ��� ��
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


        cerr << "���� ���� ����" << endl; return -1;
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

    // �θ� 2���� ����. ��ʸ�Ʈ ������� ��. 

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

            // �ݹ��ؼ� �ڽ� �����
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

            // ������� ������ cost ���ϱ� -> �Լ��� ���� ����. ��ͷ� ����
           // memoization �迭 memo[]


            vector<int> memo(node + 1);
            for (int i = 0; i < memo.size(); i++) memo[i] = 0;

            childCost[a] = calcMemCost(node, memo, child[a], v);



            // ���� ����ȭ 
            
            // ��� ũ�⸸ŭ�� ������ ���� �����


            vector<int> rand;
            for (int i = 1; i <= node; ++i) {


                rand.push_back(i);
            }
            random_shuffle(rand.begin(), rand.end());


            // pesudo code 2����� 
            // 
            // i�� ������ ���� rand�� ��ȸ 


            vector<int> candidate(child[a]);
            int candiCost = childCost[a];
            bool improved = true;
            while (improved) {
                improved = false;
                for (int i = 0; i < node; i++) {
                    // ��� rand[i]�� �ݴ� �׷����� �ٲ� ���ο� �� �����

                    vector<int> newX(child[a]);
                    int newC = 0;
                    newX[rand[i]] = 1 - newX[rand[i]];

                    //���ο� ���� cost ���ϱ�
                    int newCost = calcLocal(node, newX, rand[i], memo, v);

                    // ���ο� �ذ� �� ������ �ٲٱ�
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

    } while (countNum++ != 100); // �ݺ��� ������ Ƚ��



    // ���� �߿� ���� ū �� ã��

    for (int i = 0; i < parentNum; i++) {
        if (cost[maxP] < cost[i]) maxP = i;
    }
    for (int k = 1; k <= node; k++) cout << parents[maxP][k] << " ";
    
    cout << endl;

    // ��� ���

    for (int k = 1; k <= node; k++) {

        if (parents[maxP][k] == 1) fout << k << " ";
    }
    fout << cost[maxP] << endl;


    fp.close();
}


