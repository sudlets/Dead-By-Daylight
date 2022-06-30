#include <random>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

void game_simulation(unsigned char* run, unsigned int number_of_simulations, unsigned int numbers_of_players, unsigned int trying_unhooks, unsigned int luck)
{
    unsigned int game = 0;
    random_device rd;   // недетерминированный генератор случайных чисел
    std::seed_seq seed2{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
    std::mt19937 entropy(seed2);
    uniform_int_distribution<> dist(1, 100); // распределять результаты от 1 до 100 включительно

    while (game < number_of_simulations)
    {
        run[game] = 0;
        short player = 0;
        while (player++ < numbers_of_players)
        {
            for (unsigned int i = 0; i < trying_unhooks; ++i)
            {
                if (dist(entropy) <= luck)
                {
                    run[game] += 1;
                    break;
                }
            }
        }
        game++;
        player = 0;
    }
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    unsigned long long runs = 0;
    unsigned int numbers_of_players = 4;
    unsigned int one_runs = 0;
    unsigned int two_runs = 0;
    unsigned int three_runs = 0;
    unsigned int four_runs = 0;
    double mean_runs = 0;
    unsigned int trying_unhooks = 3;
    unsigned int luck = 4;
    unsigned int number_of_simulations = 100000000; // Количество симуляций игр


    char key;
    char* arg;

    while (--argc > 0 && (*++argv)[0] == '-')
        if (key = *++argv[0])
            switch (key)
            {
            case('l'):
                if (--argc > 0)
                {
                    arg = *++argv;
                    luck = atoi(arg);

                    if (luck <= 0 || luck > 100)
                    {
                        cout << "Процент удачи должен находится в пределах (0 < luck < 100). Вы указали: \'-l " << arg << "\'" << endl;
                        cout << "Попробуйте ввести, например, \"-l 6\"" << endl;
                        return -1;
                    }
                }
                else
                {
                    cout << "После опции \"" << key << "\" должен быть указан процент удачи!" << endl;
                    cout << "Попробуйте ввести, например, \"-l 6\"" << endl;
                    return -1;
                }
                break;

            case('t'):
                if (--argc > 0)
                {
                    arg = *++argv;
                    trying_unhooks = atoi(arg);
                    if (trying_unhooks <= 0)
                    {
                        cout << "Количество спрыгиваний не может быть отрицательным или нулевым. Вы указали: \'-t " << arg << "\'" << endl;
                        cout << "Попробуйте ввести, например, \"-t 6\"" << endl;
                        return -1;
                    }
                }
                else
                {
                    cout << "После опции \"" << key << "\" должно быть указано количество попыток!" << endl;
                    cout << "Попробуйте ввести, например, \"-t 6\"" << endl;
                    return -1;
                }
                break;

            case('n'):
                if (--argc > 0)
                {
                    arg = *++argv;
                    number_of_simulations = atoi(arg);
                    if (number_of_simulations <= 0 || number_of_simulations >= UINT64_MAX)
                    {
                        cout << "Количество испытаний не может быть отрицательным или нулевым. Вы указали: \'-n " << arg << "\'" << endl;
                        cout << "Попробуйте ввести, например, \"-n 10000000\"" << endl;
                        return -1;
                    }
                }
                else
                {
                    cout << "После опции \"" << key << "\" должно быть указано количество испытаний!" << endl;
                    cout << "Попробуйте ввести, например, \"-n 100000000\"" << endl;
                    return -1;
                }
                break;

            case('h'):
                cout << "Справка" << endl;
                cout << "-l: процент удачи у выжившего.\n   Например, указать удачу 8%: -l 8\n   По умолчанию: 4\n\n";
                cout << "-t: количество попыток спрыгнуть с крюка.\n   Например, указать 6 попыток: -t 6\n   По умолчанию: 3\n\n";
                cout << "-n: количество испытаний.\n   Например, провести эксперимент в 100000 игр: -n 100000\n   По умолчанию: 100000000. Больше - точнее результаты\n\n";
                return 0;
                break;

            default:
                cout << "Обнаружена неизвестная опция \"" << key << endl;
                cout << "Справка: -h" << endl;
                return -2;
            }

    unique_ptr<unsigned char[]> run(new unsigned char[number_of_simulations]);

    cout << "Программа запущена с параметрами: " << "-l " << luck << " -t " << trying_unhooks << " -n " << number_of_simulations << endl;
    cout << "Идут вычисления, пожалуйста, подождите..." << endl << endl;




    auto start = std::chrono::steady_clock::now();

    // Симулируем попытки спрыгивания игроков с крюка

    unsigned int hw_threads = thread::hardware_concurrency();
    if (number_of_simulations < 10000)
        hw_threads = 1;

    vector<thread> threads;
    unsigned int simulations = number_of_simulations / hw_threads;
    unsigned int simulations_end = simulations + number_of_simulations % hw_threads;
    unsigned int move = 0;

    

    for (int i = 0; i < hw_threads; ++i)
    {
        threads.push_back(thread(game_simulation, run.get() + move, i < hw_threads ? simulations : simulations_end, numbers_of_players, trying_unhooks, luck));
        move += simulations;
    }

    for (auto& t : threads)
        t.join();

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
    cout << "Среднее количество спрыгиваний за 1 игру:  " << format("{:7.5}",mean_runs) << endl;
    cout << "Вероятность спрыгнуть конкретно 1 игроку:  " << format("{:6.5}", mean_runs / 4 * 100) << "%" << endl;
    cout << "Спрыгнул  хотя бы 1 игрок  за матч:        " << format("{:2.5}", one_runs / double(number_of_simulations) * 100) << "%" << endl;
    cout << "Спрыгнуло хотя бы 2 игрока за матч:        " << format("{:2.5}", two_runs / double(number_of_simulations) * 100) << "%" << endl;
    cout << "Спрыгнуло хотя бы 3 игрока за матч:        " << format("{:2.5}", three_runs / double(number_of_simulations) * 100) << "%" << endl;
    cout << "Спрыгнуло хотя бы 4 игрока за матч:        " << format("{:2.5}", four_runs / double(number_of_simulations) * 100) << "%" << endl;

    cout << endl;
    system("pause");
    return(0);
}
