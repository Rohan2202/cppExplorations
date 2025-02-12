#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>
#include <memory>

class MutexDS
{
public:
    std::mutex m;
    int x;
    int y;
    int z;
    std::shared_ptr<int> p;

    MutexDS() : x(0), y(0), z(1) {}

    void increment_and_compute()
    {
        std::lock_guard<std::mutex> lock(m);
        if (x < 0)
        {
            x = x + 1;
            y = y + 2;
            z = z + 3;
        }
    }

    void decrement_and_compute()
    {
        std::lock_guard<std::mutex> lock(m);
        if (z > 0)
        {
            x = x - 1;
            y = y - 3;
            z = z - 5;
        }
    }
};

class AtomicDS
{
public:
    std::atomic<int> x{0}, y{1}, z{2};
    void increment_and_compute()
    {
        if (x.load() < 0)
        {
            x.store(x.load() + 1);
            y.store(y.load() + 3);
            z.store(z.load() + 1);
        }
    }

    void decrement_and_compute()
    {
        if (z.load() > 0)
        {
            x.store(x.load() - 1);
            y.store(y.load() - 3);
            z.store(z.load() - 5);
        }
    }
};

int main()
{
    MutexDS mds;
    AtomicDS ads;
    const int n = 100000;

    auto f1 = [&mds, &n]()
    { for (int i = 0; i < n; i++) mds.increment_and_compute(); };
    auto f2 = [&mds, &n]()
    { for (int i = 0; i < n; i++) mds.decrement_and_compute(); };

    auto f3 = [&ads, &n]()
    { for (int i = 0; i < n; i++) ads.increment_and_compute(); };
    auto f4 = [&ads, &n]()
    { for (int i = 0; i < n; i++) ads.decrement_and_compute(); };

    // Start timer for MutexDS
    auto start_mutex = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> mds_v;
    for (int i = 0; i < 100; i++)
    {
        if (i % 2 == 0)
        {
            mds_v.emplace_back(f1);
        }
        else
        {
            mds_v.emplace_back(f2);
        }
    }

    for (auto &t : mds_v)
    {
        t.join();
    }

    auto end_mutex = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> mutex_duration = end_mutex - start_mutex;
    std::cout << "Time taken by MutexDS: " << mutex_duration.count() << " seconds" << std::endl;
    std::cout << mds.x << ' ' << mds.y << " " << mds.z << std::endl;
    // Start timer for AtomicDS
    auto start_atomic = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> mda_v;
    for (int i = 0; i < 100; i++)
    {
        if (i % 2 == 0)
        {
            mda_v.emplace_back(f3);
        }
        else
        {
            mda_v.emplace_back(f4);
        }
    }

    for (auto &t : mda_v)
    {
        t.join();
    }

    auto end_atomic = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> atomic_duration = end_atomic - start_atomic;
    std::cout << "Time taken by AtomicDS: " << atomic_duration.count() << " seconds" << std::endl;
    std::cout << ads.x.load() << ' ' << ads.y.load() << " " << ads.z.load() << std::endl;
    return 0;
}
