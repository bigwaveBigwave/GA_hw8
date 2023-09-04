# 유전 알고리즘 

방향성이 없는 간선에 가중치가 부여된 단순 그래프(simple graph) G = (V, E)가 주어진다. V와 E는
각각 정점 집합과 간선 집합이다. 정점 집합 V를 두 개의 집합 S와 S’(=V-S)로 나누는 방법은
여러 종류가 있는데, 여러분은 S와 S’을 잇는 간선의 가중치의 총합을 최대화하도록 나눠야 한다.
그림 1은 10개의 정점과 14개의 간선으로 이뤄진 단순 그래프의 한 인스턴스다. 각 간선의
가중치는 1 또는 -1로 주어져있다. 그림 2는 그림 1의 그래프의 정점 집합을 둘로 나누는 방법
중 하나로, S = {1, 6, 8, 9}, S’ = {2, 3, 4, 5, 7, 10}로 나눈 것이다. S와 S’에 속한 정점들을 각각
검은색과 흰색으로 표시하였다. 굵게 표시된 간선은 S와 S’을 잇는 간선으로 가중치의 총합은 8이
된다. 그림 3은 정점 집합을 둘로 나누는 다른 방법으로, 두 집합을 잇는 간선의 가중치의 총합은
7이다.

주어진 단순 그래프 G에서 정점 집합을 두 집합 S와 S’(= V-S)로 나눌 때, 이 둘을 잇는 간선의
가중치의 총합을 제한 시간 내에 가능한 최대화하는 Genetic Algorithm을 작성하라.
- 인스턴스의 크기: 정점의 개수 |V|≤500, 간선의 개수 |E| ≤ 5000
- 제한 시간: 한 인스턴스 당 180초

[입력]
입력 파일의 이름은 maxcut.in이다. 입력 파일의 각 항목은 공백과 줄바꿈으로 구분된다. 첫
줄에는 정점의 개수 |V|와 간선의 개수 |E|가 주어진다. 둘째 줄부터 차례대로 각 간선의 양 끝
정점의 번호와 간선의 가중치가 주어진다. 정점의 번호는 1 이상 |V| 이하의 정수로 주어지고,
간선의 가중치는 정수로 주어진다. 시작점과 끝점이 같은 간선(self-loop)이나 동일한 두 정점을
잇는 복수 개의 간선(multiple edges)는 주어지지 않는다.
[출력]
출력 파일의 이름은 maxcut.out이다. 여러분이 구한 최적 집합 S 내의 모든 정점 번호를 한 줄에
공백으로 구분하여 출력한다. 출력 파일에 가중치 총합 수치를 포함하지 않는다