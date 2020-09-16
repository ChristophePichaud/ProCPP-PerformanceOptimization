// Apress - ProCppPerfOptim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

void MonitoringServer(const string& server)
{
    // ...
    cout << "Monitoring Server..." << server << endl;
}

void Routine1()
{
    for (int i = 0; i < 1024; ++i)
    {
        MonitoringServer("SERVER01");
        this_thread::sleep_for(60s);
    }
}

void Routine2(const vector<string>& servers)
{
    for_each(servers.begin(), servers.end(), [](const string& name) {
        MonitoringServer(name);
        this_thread::sleep_for(1s);
        });
}

class BackgroundTask
{
public:
    BackgroundTask(const std::string& name, const int& age) : _name(name), _age(age) {}

    double GetResult() const
    {
        return _res;
    }

    void operator()()
    {
        std::cout << "Name:" << _name
            << ", Age:" << _age << std::endl;
        _res = _age * 10.52;
    }

private:
    std::string _name;
    int _age = 0;
    double _res = 0.0;
};

int _counter = 0;
string _name = "";
mutex _mutex;
shared_mutex _smutex;

void RoutineModifyMembers()
{
    for (int i = 0; i < 10000; ++i)
    {
        _mutex.lock();
        ++_counter;
        _name += to_string(i);
        _mutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(10));

    }
}

void RoutinePrintMembers()
{
    for (int i = 0; i < 10000; ++i)
    {
        _mutex.lock();
        //cout << "Name: " << _name << ", Counter: " << _counter << endl;
        cout << "Counter: " << _counter << endl;
        _mutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(5));

    }
}

void RoutineModifyMembers2()
{
    for (int i = 0; i < 10000; ++i)
    {
        std::scoped_lock lock(_mutex);

        ++_counter;
        _name += to_string(i);

        this_thread::sleep_for(chrono::milliseconds(10));

    }
}

void RoutinePrintMembers2()
{
    for (int i = 0; i < 10000; ++i)
    {
        std::scoped_lock lock(_mutex);

        cout << "Counter: " << _counter << endl;

        this_thread::sleep_for(chrono::milliseconds(5));

    }
}

void RoutineModifyMembers3()
{
    for (int i = 0; i < 10000; ++i)
    {
        _smutex.lock_shared();
        ++_counter;
        _name += to_string(i);
        _smutex.unlock_shared();
        this_thread::sleep_for(chrono::milliseconds(10));

    }
}

void RoutinePrintMembers3()
{
    for (int i = 0; i < 10000; ++i)
    {
        _smutex.lock_shared();
        cout << "Counter: " << _counter << endl;
        _smutex.unlock_shared();
        this_thread::sleep_for(chrono::milliseconds(5));

    }
}

void funcAsync()
{
    future<int> f1 = async([]() {
        int data = 0;
        for (int i = 0; i < 100; ++i)
        {
            data += i;
        }
        return data;
        });

    int data = f1.get();
    cout << "Task returned : " << data << endl;
}

atomic_int _count = 0;

void funcAtomic()
{
    thread t1([=]() { for (int i = 0; i < 10000; ++i) {
        _count = _count + 1;
        this_thread::sleep_for(chrono::milliseconds(25));
    }
        });

    thread t2([=]() { for (int i = 0; i < 10000; ++i) {
        _count = _count + 10;
        this_thread::sleep_for(chrono::milliseconds(25));
    }
        });

    thread t3([=]() { for (int i = 0; i < 10000; ++i) {
        cout << "Count: " << _count << endl;
        this_thread::sleep_for(chrono::milliseconds(25));
    }
        });

    t1.join();
    t2.join();
    t3.join();
}


typedef std::shared_mutex Lock;
typedef std::unique_lock<Lock>  WriterLock;
typedef std::shared_lock<Lock>  ReaderLock;
Lock myLock;

void ReaderFunction()
{
    ReaderLock r_lock(myLock);
    // Make some read operations
}

void WriterFunction()
{
    WriterLock w_lock(myLock);
    // Make some write operations
}

void funcRWLock()
{
    thread t1([=]() { for (int i = 0; i < 10000; ++i) {
        WriterLock w_lock(myLock);
        _count = _count + 1;
        this_thread::sleep_for(chrono::milliseconds(25));
    }
        });

    thread t2([=]() { for (int i = 0; i < 10000; ++i) {
        WriterLock w_lock(myLock);
        _count = _count + 10;
        this_thread::sleep_for(chrono::milliseconds(25));
    }
        });

    thread t3([=]() { for (int i = 0; i < 10000; ++i) {
        ReaderLock r_lock(myLock);
        cout << "Count: " << _count << endl;
        this_thread::sleep_for(chrono::milliseconds(25));
    }
        });

    t1.join();
    t2.join();
    t3.join();
}

condition_variable _cv;
mutex _mutex1;
int _counter2 = 0;

void funcConditionVariables()
{
    thread t1([=]() {
        unique_lock<mutex> lock(_mutex1);
        _cv.wait(lock);
        cout << "Counter: " << _counter2 << endl;
        });

    thread t2([=]() { 
        unique_lock<mutex> lock(_mutex1);
        for (int i = 0; i < 100; ++i) {
        _counter2 = _counter2 + 10;
        cout << i;
        this_thread::sleep_for(chrono::milliseconds(25));
    }
        lock.unlock();
        _cv.notify_one();
        });

    t1.join();
    t2.join();
}

class Rand_int {
public:
    Rand_int(int low, int high) :dist{ low,high } { }
    int operator()() { return dist(re); } // draw an int
    void seed(int s) { re.seed(s); } // choose new random engine seed
private:
    default_random_engine re;
    uniform_int_distribution<> dist;
};

void funcAlgorithms()
{
    //vector<int> data;
    //for (int i = 0; i < 100; ++i) {
    //    default_random_engine dre;
    //    uniform_int_distribution uid(1, 100);
    //    auto die = [=]() { return uid(dre); }; // make a generator
    //    int value = die(); // roll the die: x becomes a value in [1:6]
    //    cout << value << " ";
    //}

    //using my_engine = default_random_engine; // type of engine
    //using my_distribution = uniform_int_distribution<>; // type of distribution
    //my_engine re{}; // the default engine
    //my_distribution one_to_six{ 1,6 }; // distr ibution that maps to the ints 1..6
    //auto die = [=]() { return one_to_six(re); }; // make a generator
    //int x = die(); // roll the die: x becomes a value in [1:6]
    ////Thanks to its uncompromising

    constexpr int max = 9;
    Rand_int rnd{ 0,max }; // make a unifor m random number generator
    vector<int> histogram(max + 1); // make a vector of appropriate size
    for (int i = 0; i != 200; ++i)
        ++histogram[rnd()]; // fill histogram with the frequencies of numbers [0:max]
    for (int i = 0; i != histogram.size(); ++i) { // wr ite out a bar graph
        cout << i << '\t';
        for (int j = 0; j != histogram[i]; ++j) cout << '∗';
        cout << endl;
    }
}

int main()
{
    funcAlgorithms();
    return 0;
    funcConditionVariables();
    return 0;
    funcRWLock();
    return 0;
    funcAtomic();
    funcAsync();
    return 0;

    thread t4(RoutineModifyMembers3);
    thread t5(RoutinePrintMembers3);
    t4.join();
    t5.join();
    return 0;

    BackgroundTask task("Lisa", 14);
    std::thread t3(ref(task));
    t3.join();
    std::cout << "Result: " << task.GetResult() << endl;

    vector<string> servers{ "SERVER01", "SERVER02", "SERVER03", "SERVER04", "SERVER05", "SERVER06" };
    thread t2(Routine2, ref(servers));
    t2.join();

    thread t1(Routine1);
    t1.join();
    return 0;
}

