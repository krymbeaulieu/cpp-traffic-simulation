#include <iostream>
#include <random>
//#include <chrono>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() DONE
    // to wait for and receive new messages and pull them from the queue using move semantics. DONE
    // The received object should then be returned by the receive function. DONE
    std::unique_lock<std::mutex> unq_lock(_mutex);
    _condition.wait(unq_lock, [this]() {return !_queue.empty();});
    T msg = std::move(_queue.front()); //queue FIFO
    _queue.pop_front();
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> DONE
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification. DONE
    std::lock_guard<std::mutex> guard(_mutex);
    _queue.emplace_back(std::move(msg));
    _condition.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop DONE
    // runs and repeatedly calls the receive function on the message queue. DONE
    // Once it receives TrafficLightPhase::green, the method returns. DONE
    while(true){
        
        TrafficLightPhase light = std::move(_messsages.receive());
        if(TrafficLightPhase::green == light){
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.  DONE
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
    // threads.emplace_back(std::move(std::thread(&TrafficLight::cycleThroughPhases, this)));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles DONE
    // and toggles the current phase of the traffic light between red and green and sends an update method DONE
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. DONE
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. DONE

    // set up random cycle duration
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<int> dur(4, 6);
    // first duration
    int rand_dur = dur(engine);

    //startup vals
    auto start = std::chrono::steady_clock::now();

    while (true)
    {
        const auto curr_time = std::chrono::steady_clock::now();
        const auto curr_dur = std::chrono::duration_cast<std::chrono::seconds>(curr_time - start).count();
        if (rand_dur <= curr_dur)
        {
            start = std::chrono::steady_clock::now();
            // switch light
            switch (_currentPhase)
            {
            case green:
                _currentPhase = red;
                break;
            default:
                _currentPhase = green;
                break;
            }
            // update message
            TrafficLightPhase msg = _currentPhase;
            _messsages.send(std::move(msg));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
