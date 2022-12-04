#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;
enum TrafficLightPhase
{
    red,
    green
};
// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. DONE
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. DONE
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. DONE
// Also, there should be an std::condition_variable as well as an std::mutex as private members. DONE

template <class T>
class MessageQueue
{
public:
    void send(T &&msg);
    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. DONE
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ DONE
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that DONE
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. DONE
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. DONE

class TrafficLight : public TrafficObject
{
public:
    // constructor / destructor
    TrafficLight();

    // getters / setters
    TrafficLightPhase getCurrentPhase();

    // typical behaviour methods
    void waitForGreen();
    void simulate();

private:
    // typical behaviour methods
    void cycleThroughPhases();
    TrafficLightPhase _currentPhase;
    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase DONE
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling DONE
    // send in conjunction with move semantics. DONE
    MessageQueue<TrafficLightPhase> _messsages;
    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif