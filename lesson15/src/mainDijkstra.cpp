#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

int debugPoint(int line) {
    if (line < 0)
        return 0;

    // You can put breakpoint at the following line to catch any rassert failure:
    return line;
}

#define rassert(condition, message) if (!(condition)) { std::stringstream ss; (ss << "Assertion \"" << message << "\" failed at line " << debugPoint(__LINE__) << "!"); throw std::runtime_error(ss.str()); }
#define int long long

struct Edge {
    int u, v; // номера вершин которые это ребро соединяет
    int w; // длина ребра (т.е. насколько длинный путь предстоит преодолеть переходя по этому ребру между вершинами)

    Edge(int u, int v, int w) : u(u), v(v), w(w)
    {}
};

void run() {
    // https://codeforces.com/problemset/problem/20/C?locale=ru
    // Не требуется сделать оптимально быструю версию, поэтому если вы получили:
    //
    // Превышено ограничение времени на тесте 31
    //
    // То все замечательно и вы молодец.

    int nvertices, medges;
    std::cin >> nvertices;
    std::cin >> medges;

    std::vector<std::vector<Edge>> edges_by_vertex(nvertices);
    for (int i = 0; i < medges; ++i) {
        int ai, bi, w;
        std::cin >> ai >> bi >> w;
        rassert(ai >= 1 && ai <= nvertices, 23472894792020);
        rassert(bi >= 1 && bi <= nvertices, 23472894792021);

        ai -= 1;
        bi -= 1;
        rassert(ai >= 0 && ai < nvertices, 3472897424024);
        rassert(bi >= 0 && bi < nvertices, 3472897424025);

        Edge edgeAB(ai, bi, w);
        edges_by_vertex[ai].push_back(edgeAB);

        edges_by_vertex[bi].push_back(Edge(bi, ai, w)); // а тут - обратное ребро, можно конструировать объект прямо в той же строчке где он и потребовался
    }

    const int start = 0;
    const int finish = nvertices - 1;

    const int INF = std::numeric_limits<int>::max();

    std::vector<int> distances(nvertices, INF);
    distances[start] = 0;

    std::vector<bool> is_processed(nvertices, 0);
    is_processed[start] = 1;
    int last_processed = start;

    std:std::vector<int> from(nvertices,0);
    from[start]=-1;
    while(true){
        bool is_new = 0;
        for(auto E: edges_by_vertex[last_processed])
            if(!is_processed[E.v]){
                distances[E.v]=std::min(distances[E.v],distances[E.u]+E.w);
                is_new = 1;
                from[E.v] = E.u;
            }
        if(!is_new)
            break;
        int min_vert = finish;
        for(int v = 0;v<nvertices;v++)
            if(!is_processed[v])
                if(distances[min_vert]>distances[v])
                {
                    min_vert = v;
                }
        is_processed[min_vert] = 1;
        last_processed = min_vert;
    }
    if (distances[finish]!=INF) {
        std::vector<int> pass;
        pass.reserve(nvertices);
        int ptr = finish;
        while(ptr!=-1){
            pass.push_back(ptr);
            ptr = from[ptr];
        }
        std::reverse(pass.begin(), pass.end());
        for (auto v:pass) {
            std::cout << (v + 1) << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << -1 << std::endl;
    }
}

signed main() {
    try {
        run();

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
