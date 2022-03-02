#include <vector>
#include <sstream>
#include <iostream>
#include <set>
#include <stdexcept>

#include <libutils/rasserts.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>


struct Edge {
    int u, v; // номера вершин которые это ребро соединяет
    int w; // длина ребра (т.е. насколько длинный путь предстоит преодолеть переходя по этому ребру между вершинами)

    Edge(int u, int v, int w) : u(u), v(v), w(w)
    {}
};

// Эта биективная функция по координате пикселя (строчка и столбик) + размерам картинки = выдает номер вершины
int encodeVertex(int row, int column, int nrows, int ncolumns) {
    rassert(row < nrows, 348723894723980017);
    rassert(column < ncolumns, 347823974239870018);
    int vertexId = row * ncolumns + column;
    return vertexId;
}

// Эта биективная функция по номеру вершины говорит какой пиксель этой вершине соовтетствует (эта функция должна быть симметрична предыдущей!)
cv::Point2i decodeVertex(int vertexId, int nrows, int ncolumns) {

    // TODO: придумайте как найти номер строки и столбика пикселю по номеру вершины (просто поймите предыдущую функцию и эта функция не будет казаться сложной)
    int row = -1;
    int column = -1;

    // сверим что функция симметрично сработала:
    rassert(encodeVertex(row, column, nrows, ncolumns) == vertexId, 34782974923035);

    rassert(row < nrows, 34723894720027);
    rassert(column < ncolumns, 3824598237592030);
    return cv::Point2i(column, row);
}

void run(int mazeNumber) {
    cv::Mat maze = cv::imread("H:\\CPPExercises2021\\lesson15\\data\\mazesImages\\maze" + std::to_string(mazeNumber) + ".png");
    rassert(!maze.empty(), 324783479230019);
    rassert(maze.type() == CV_8UC3, 3447928472389020);
    std::cout << "Maze resolution: " << maze.cols << "x" << maze.rows << std::endl;

    int nvertices = maze.cols*maze.rows; // TODO

    std::vector<std::vector<Edge>> edges_by_vertex(nvertices);
    for (int j = 0; j < maze.rows; ++j) {
        for (int i = 0; i < maze.cols; ++i) {
            cv::Vec3b color = maze.at<cv::Vec3b>(j, i);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];

            int v = encodeVertex(j, i, maze.rows, maze.cols);
            for(int di = -1; di<2; di++){
                for(int dj = -1; dj<2; dj++){
                    if(i+di>=0 && j+dj>=0 && i+di<maze.cols && j+dj<maze.rows && !(di==0 && dj==0)){
                        cv::Vec3b _color = maze.at<cv::Vec3b>(j, i);
                        unsigned char _blue = _color[0];
                        unsigned char _green = _color[1];
                        unsigned char _red = _color[2];
                        int diff = (_blue-blue)*(_blue-blue)+(_red-red)*(_red-red)+(_green-green)*(_green-green);
                        int u = encodeVertex(j+dj, i+di, maze.rows, maze.cols);
                        edges_by_vertex[v].push_back(Edge(v, u, diff));
                    }
                }
            }
        }
    }

    int start, finish;
    if (mazeNumber >= 1 && mazeNumber <= 3) { // Первые три лабиринта очень похожи но кое чем отличаются...
        start = encodeVertex(300, 300, maze.rows, maze.cols);
        finish = encodeVertex(0, 305, maze.rows, maze.cols);
    } else if (mazeNumber == 4) {
        start = encodeVertex(154, 312, maze.rows, maze.cols);
        finish = encodeVertex(477, 312, maze.rows, maze.cols);
    } else if (mazeNumber == 5) { // Лабиринт в большом разрешении, добровольный (на случай если вы реализовали быструю Дейкстру с приоритетной очередью)
        start = encodeVertex(1200, 1200, maze.rows, maze.cols);
        finish = encodeVertex(1200, 1200, maze.rows, maze.cols);
    } else {
        rassert(false, 324289347238920081);
    }

    const int INF = std::numeric_limits<int>::max();

    cv::Mat window = maze.clone(); // на этой картинке будем визуализировать до куда сейчас дошла прокладка маршрута

    std::vector<int> distances(nvertices, INF);


    int* q = new int[nvertices];
    int * prev = new int[nvertices];

    std::set<std::pair<int, int>> now;
    q[start] = 0;
    prev[start] = -1;
    now.insert({q[start], start});
    for(int i = 0; i<nvertices; i++){
        if(i!=start){
            q[i] = 10000000000000ll;
            now.insert({q[i], i});
            prev[i] = -1;
        }
    }

    while(!now.empty()){
        int v = (*now.begin()).second;
        int len = (*now.begin()).first;
        now.erase(now.begin());
        for(auto z:edges_by_vertex[v]){
            if(len+z.w<q[z.v]){
                now.erase({q[z.v], z.v});
                q[z.v] = len+z.w;
                prev[z.v] = v;
                now.insert({q[z.v], z.v});
            }

        }
    }

    if (prev[finish]!=-1) {
        int now = finish;
        std::vector<int> ans;
        while(now!=-1) {
            ans.push_back(now+1);
            now = prev[now];
        }
        reverse(ans.begin(), ans.end());
        for(auto z:ans){
            maze.at<cv::Vec3b>(j, i);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];
            std::cout << z << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << -1 << std::endl;
    }

    // TODO в момент когда вершина становится обработанной - красьте ее на картинке window в зеленый цвет и показывайте картинку:
    //    cv::Point2i p = decodeVertex(the_chosen_one, maze.rows, maze.cols);
    //    window.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(0, 255, 0);
    //    cv::imshow("Maze", window);
    //    cv::waitKey(1);
    // TODO это может тормозить, в таком случае показывайте window только после обработки каждой сотой вершины

    // TODO обозначьте найденный маршрут красными пикселями

    // TODO сохраните картинку window на диск

    std::cout << "Finished!" << std::endl;

    // Показываем результат пока пользователь не насладиться до конца и не нажмет Escape
    while (cv::waitKey(10) != 27) {
        cv::imshow("Maze", window);
    }
}

int main() {
    try {
        int mazeNumber = 1;
        run(mazeNumber);

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
