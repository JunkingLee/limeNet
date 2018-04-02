//
// Created by tony on 18-3-26.
//

#ifndef LIMENET_THREADLOCK_HPP
#define LIMENET_THREADLOCK_HPP

#include <thread>
#include <condition_variable>
#include <memory>
using namespace std::chrono;
class ThreadLock{
public:
    ThreadLock():_finished(false),_isStart(false){}
    void Start(){
        //_thread=new std::thread();
        _thread=make_unique<std::thread>(&ThreadLock::entry,this);
        _tid=_thread->get_id();
        _thread->detach();
        _isStart=true;
    }

    bool wait(milliseconds waitTime){
        bool ret=true;
        if(!_isStart)
            return ret;
        std::unique_lock<std::mutex> locker(_mutex);
        while(!_finished){
            if(_condition.wait_for(locker,waitTime)== std::cv_status::timeout){
                ret=false;
                break;
            }
        }
        return ret;
    }

    void wait(){
        if(!_isStart)
            return;
        std::unique_lock<std::mutex> locker(_mutex);
        while(!_finished)
            _condition.wait(locker);
    }

    std::thread::id getTid() const{
        return _tid;
    }

    virtual void run()=0;

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

protected:
    ThreadLock(const ThreadLock&)=delete;
    ThreadLock& operator=(const ThreadLock&)=delete;
private:
    void entry(){
            this->run();
            _isStart=false;
            std::unique_lock<std::mutex> locker(_mutex);
            _finished=true;
            _condition.notify_all();

    }
private:
    std::mutex _mutex;
    std::condition_variable _condition;
    bool _finished;
    bool _isStart;
    std::thread::id _tid;
    std::unique_ptr<std::thread> _thread;
};
#endif //LIMENET_THREADLOCK_HPP
