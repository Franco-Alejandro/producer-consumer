#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex m;
std::condition_variable cv;
std::queue<int> q;

void producer() {
    for (int i = 0; i < 10; ++i) {
        std::unique_lock<std::mutex> lock(m);
        q.push(i);
        lock.unlock();
        cv.notify_one();
    }
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [] { return !q.empty(); });
        int val = q.front();
        q.pop();
        lock.unlock();
        std::cout << "Consumer received: " << val << std::endl;
        if (val == 9) {
            break;
        }
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}
