#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "hough.h"

#include <opencv2/imgproc.hpp>
std::string pass = "C:/Users/Ratkovva.22/CLionProjects/CPPExercises2021/lesson05/data/";
void test(std::string name) {
    std::string full_path = pass + name + ".jpg";
    cv::Mat img = cv::imread(full_path);
    rassert(!img.empty(), 238982391080010);
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // преобразуем в оттенки серого

    cv::Mat grad_x, grad_y; // в этих двух картинках мы получим производную (градиент=gradient) по оси x и y
    // для этого достаточно дважды применить оператор Собеля (реализованный в OpenCV)
    cv::Sobel(img, grad_x, CV_32FC1, 1, 0);
    cv::Sobel(img, grad_y, CV_32FC1, 0, 1);
    rassert(!grad_x.empty(), 234892748239070017);
    rassert(!grad_y.empty(), 234892748239070018);

    // сохраняем оба результата в картинку на диск, чтобы проверить что результат выглядит разумно
    cv::imwrite(pass + name + "_1_sobel_x.png", grad_x);
    cv::imwrite(pass + name + "_2_sobel_y.png", grad_y);

    // замечаем что мы ведь забыли взять абсолютное значение градиента!
    // TODO посмотрите на картинки на диске, какая из картинок это явно показывает?
    // давайте это исправим:
    grad_x = cv::abs(grad_x);
    grad_y = cv::abs(grad_y);
    cv::imwrite(pass+ name + "_3_sobel_x.png", grad_x);
    cv::imwrite(pass + name + "_4_sobel_y.png", grad_y);

    cv::Mat sobel_strength(img.rows, img.cols, CV_32FC1, 0.0f);
    // теперь хочется заполнить sobel_strength силой градиента с учетом обеих осей, т.е. что-то вроде sobel_strength=sqrt(grad_x^2+grad_x^2):
    for (int j = 0; j < sobel_strength.rows; ++j) {
        for (int i = 0; i < sobel_strength.cols; ++i) {
            float dx = grad_x.at<float>(j, i);
            float dy = grad_y.at<float>(j, i);
            float gradient_strength = sqrt(dx*dx+dy*dy);
            sobel_strength.at<float>(j, i) = gradient_strength;
        }
    }
    cv::imwrite(pass + name + "_5_sobel_strength.png", sobel_strength);

    cv::Mat hough = buildHough(sobel_strength);

    cv::imwrite(pass + name + "_6_hough.png", hough);
    float max_accumulated = 0.0f;
    for (int j = 0; j < hough.rows; ++j) {
        for (int i = 0; i < hough.cols; ++i){
            max_accumulated = fmax(hough.at<float>(j, i),max_accumulated);
         }
     }
    for (int j = 0; j < hough.rows; ++j) {
        for (int i = 0; i < hough.cols; ++i){
            float t = hough.at<float>(j, i)/max_accumulated*255;
            hough.at<float>(j, i) = t;
        }
    }
    cv::imwrite(pass + name + "_7_hough_normalized.png", hough);
}


int main() {
    try {
        // TODO посмотрите на результат (аккумулятор-пространство Хафа) на всех этих картинках (раскомментируя их одну за другой)
        // TODO подумайте и напишите здесь оветы на вопросы:
        test("line01");
        // 1) Какие координаты примерно должны бы быть у самой яркой точки в картинке line01_7_hough_normalized.png?
        // ответ:

//        test("line02");
//        // 2) Какие координаты примерно должны бы быть у самой яркой точки в картинке line02_7_hough_normalized.png?
//        // ответ:
//
//        test("line11");
//        // 3) Чем должно бы принципиально отличаться пространство Хафа относительно случая line01?
//        // ответ:
//
//        test("line12");
//        // 4) Зная правильный ответ из предыдущего случая line11 - как найти правильнйы ответ для line12?
//        // ответ:
//
//        test("line21_water_horizont");
//        // 5) Сколько должно бы быть ярких точек?
//        // ответ:
//
//        test("multiline1_paper_on_table");
//        // 6) Сколько должно бы быть ярких точек? Сколько вы насчитали в пространстве Хафа?
//        // ответ:
//
//        test("multiline2_paper_on_table");
//        // 7) Сколько должно бы быть ярких точек? Сколько вы насчитали в пространстве Хафа? Есть ли интересные наблюдения относительно предыдущего случая?
//        // ответ:
//
//        test("valve");
//        // 8) Какие-нибудь мысли?

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}

