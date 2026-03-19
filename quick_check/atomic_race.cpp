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
    Shared s;

    auto t1 = std::thread([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        s.mtx.lock();
        if (s.state == 0) {
            if (s.flag.load()) {
                s.state = 1;
            }

            s.flag.store(true);
        }
        s.mtx.unlock();
    });

    auto t2 = std::thread([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        s.mtx.lock();
        if (!s.flag.load()) {
            s.state = 2;
        }

        s.flag.store(false);
        s.mtx.unlock();
    });

    auto t3 = std::thread([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        s.flag.store(false);
    });

    t1.join();
    t2.join();
    t3.join();

    return 0;
}

/*
Why This Code is Race-Free:
- Mutex protection: Any non-atomic shared data (s.state) is accessed with proper locking
- Atomic operations: The std::atomic_bool flag ensures race-free access even without mutexes
- No mixed access patterns: No thread accesses the same variable partly under lock and partly without lock

However, There May Still Be Logical Bugs:
While TSan reports no races, the code might have logic errors or unexpected 
behavior due to the complex interaction between threads. For example:

- Thread 1 and thread 2 both modify s.flag while holding the mutex, but thread 3 modifies it without the mutex
- The s.state value could end up in unexpected states due to interleaving
- The relationship between s.state and s.flag might not be consistent

TSan only detects data races (unsynchronized access to shared memory), not logical concurrency bugs.
*/