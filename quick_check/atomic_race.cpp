#include <atomic>
#include <thread>
#include <mutex>
#include <chrono>

class Shared {
public:
    int state = 0;
    std::mutex mtx;
    std::atomic_bool flag{false};
};

int main()
{
    for (int iter = 0; iter < 1000; iter++) {
        Shared s;

        auto t1 = std::thread([&] {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            for (int i = 0; i < 100; i++) {  // Loop to increase race window
                s.mtx.lock();
                if (s.state == 0) {
                    if (s.flag.load()) {
                        s.state = 1;
                    }

                    s.flag.store((i+1) % 2);
                }
                s.mtx.unlock();
            }
        });

        auto t2 = std::thread([&] {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            for (int i = 0; i < 100; i++) {  // Loop to increase race window
                s.mtx.lock();
                if (!s.flag.load()) {
                    s.state = 2;
                }

                s.flag.store(i % 2);
                s.mtx.unlock();
            }
        });

        auto t3 = std::thread([&] {
            for (int i = 0; i < 100; i++) {
                //std::this_thread::sleep_for(std::chrono::milliseconds(1));
                s.flag.store(i %2 );
            }
        });

        t1.join();
        t2.join();
        t3.join();
    }
    return 0;
}
