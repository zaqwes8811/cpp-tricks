// (c) from Sutter

#ifndef S_ACTORS_CC11_H_
#define S_ACTORS_CC11_H_

#include <memory>
#include <string>
#include <thread>

#include "safe_queue_cc11.h"

namespace cc11 {
// Example 2: Active helper, in idiomatic C++(0x)
//
class Actior {
public:
    typedef std::function<void()> Message;

    Actior() : done(false) {
        thd = std::unique_ptr<std::thread>(new std::thread([=] { this->Run(); }));
    }

    ~Actior() {
        Send([&] { done = true; });
        ;
        thd->join();
    }

    void Send(Message m) { mq.enqueue(m); }

private:
    Actior(const Actior &);          // no copying
    void operator=(const Actior &);  // no copying

    bool done;                             // le flag
    concurent::message_queue<Message> mq;  // le queue
    std::unique_ptr<std::thread> thd;      // le thread

    void Run() {
        while (!done) {
            Message msg = mq.dequeue();
            msg();  // execute message
        }           // note: last message sets done to true
    }
};
}  // namespace cc11

#endif