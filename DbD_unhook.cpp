#include <random>
#include <iostream>
#include <chrono>

constexpr auto number_of_simulations = 100000000; // Количество симуляций игр

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Идут вычисления, пожалуйста, подождите..." << endl << endl;

    random_device rd;   // недетерминированный генератор случайных чисел
    uniform_int_distribution<> dist(1, 100); // распределять результаты от 1 до 100 включительно
    unique_ptr<unsigned char[]> run(new unsigned char[number_of_simulations]);
    unsigned int game = 0;
    unsigned int runs = 0;
    unsigned int numbers_of_players = 4;
    unsigned int trying_unhooks = 3;
    unsigned int one_runs = 0;
    unsigned int two_runs = 0;
    unsigned int three_runs = 0;
    unsigned int four_runs = 0;
    double mean_runs = 0;
    
    auto start = std::chrono::steady_clock::now();

    // Симулируем попытки спрыгивания игроков с крюка
    while (game < number_of_simulations)
    {
        run[game] = 0;
        short player = 0;
        while (player++ < numbers_of_players)
        {
            for (unsigned int i = 0; i < trying_unhooks; ++i)
            {
                if (dist(rd) % 25 == 0)
                {
                    run[game] += 1;
                    break;
                }
            }
        }
        game++;
        player = 0;
    }
    // Конец симуляции

    auto end = std::chrono::steady_clock::now();

    // Время выполнения симуляций
    std::chrono::duration<double> elapsed_seconds = end - start;

    // Подсчёт статистики
    for (unsigned int i = 0; i < number_of_simulations; i++)
    {
        // Всего спрыгиваний
        runs += run[i];

        // Спрыгнул хотя бы 1 игрок
        one_runs += run[i] >= 1 ? 1 : 0;

        // Спрыгнуло хотя бы 2 игрока
        two_runs += run[i] >= 2 ? 1 : 0;

        // Спрыгнуло хотя бы 3 игрока
        three_runs += run[i] >= 3 ? 1 : 0;

        // Спрыгнуло хотя бы 4 игрока
        four_runs += run[i] >= 4 ? 1 : 0;
    }
        
    // Среднее количество спрыгиваний
    mean_runs = runs / double(number_of_simulations);
    
    // Вывод статистики
    cout << "Время выполнения симуляций игр:            " << format("{:9.4}", elapsed_seconds.count()) << " сек."   << endl;
    cout << "Проведено симуляций игр:                   " << format("{:9}", number_of_simulations)     << endl;
    cout << "Среднее количество спрыгиваний за 1 игру:  " << format("{:9.5}",mean_runs) << endl;
    cout << "Вероятность спрыгнуть конкретно 1 игроку:  " << format("{:9.5}", mean_runs / 4) << endl;
    cout << "Спрыгнул  хотя бы 1 игрок  за матч:        " << format("{:9}", one_runs)   << " Вероятность события: " << format("{:2.5}", one_runs / double(number_of_simulations))      << endl;
    cout << "Спрыгнуло хотя бы 2 игрока за матч:        " << format("{:9}", two_runs)   << " Вероятность события: " << format("{:2.5}", two_runs / double(number_of_simulations))      << endl;
    cout << "Спрыгнуло хотя бы 3 игрока за матч:        " << format("{:9}", three_runs) << " Вероятность события: " << format("{:2.5}", three_runs / double(number_of_simulations))    << endl;
    cout << "Спрыгнуло хотя бы 4 игрока за матч:        " << format("{:9}", four_runs)  << " Вероятность события: " << format("{:2.5}", four_runs / double(number_of_simulations))     << endl;

    cout << endl;
    system("pause");
    return(0);
}