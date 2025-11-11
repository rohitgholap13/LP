#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <condition_variable>

using namespace std;
mutex mtx;          // Mutex object
queue<int> buffer;
condition_variable cv;

sem_t empty_slots;
sem_t full_slots;

void producer_mutex(){
    for(int i=1; i<=10; i++){
        lock_guard<mutex> lock(mtx);
        buffer.push(i);
        cout << "Produced " << i << endl;
        cv.notify_one();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void consumer_mutex(){
    while(1){
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] {return !buffer.empty();});
        int data = buffer.front();
        buffer.pop();
        cout << "Consumed " << data << endl;
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void producer_semaphore(){
    for(int i = 1; i <= 5; ++i){
      sem_wait(&empty_slots);
      buffer.push(i);
      cout << "Produced: " << i << endl;
      sem_post(&full_slots);
      this_thread::sleep_for(chrono::milliseconds(500));
   }
}

void consumer_semaphore(){
    while(1){
      sem_wait(&full_slots);
      int data = buffer.front();
      buffer.pop();
      cout << "Consumed: " << data << endl;
      sem_post(&empty_slots);
      this_thread::sleep_for(chrono::milliseconds(500));
   }
}

int main(){
    // thread producerThread(producer_mutex);
    // thread consumerThread(consumer_mutex);

    // producerThread.join();
    // consumerThread.join();

    sem_init(&empty_slots, 0, 10);
    sem_init(&full_slots, 0, 0);

    thread producerThread(producer_semaphore);
    thread consumerThread(consumer_semaphore);

    producerThread.join();
    consumerThread.join();
    
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    return 0;
}