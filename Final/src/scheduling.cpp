#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <map>
#include <iterator>
#include <vector>

using namespace std;

pqueue_arrival read_workload(string filename) {
  pqueue_arrival workload;
  std::ifstream infile(filename);
  int arrival;
  int duration;
  int burst_time;
  while (infile >> arrival >> duration >> burst_time)
  {
    Process p = {arrival, -1, duration, -1, burst_time};
    workload.push(p);
  }
  return workload;
}

void show_workload(pqueue_arrival workload) {
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty()) {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes) {
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty()) {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion
         << endl;
    xs.pop_front();
  }
}

list<Process> fifo(pqueue_arrival workload) {
  list<Process> complete;
  int time = 0;
  while(!workload.empty()){
    Process p = workload.top();
    if(p.arrival > time){
      time = p.arrival;
    }
    p.first_run = time;
    time = time + p.duration;
    p.completion = time;
    complete.push_back(p);
    workload.pop();
  }
  return complete;
}

list<Process> sjf(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_duration wLoad;
  int time = workload.top().arrival;
  while(!workload.empty()){
    Process p = workload.top();
    if(p.arrival <= time){
      wLoad.push(p);
      workload.pop();
    // if a process starts after time then we run the shortest job
    // in the duration queue
    }else{
        if(!wLoad.empty()){
          Process pReady = wLoad.top();
          wLoad.pop();
          pReady.first_run = time;
          time = time + pReady.duration;
          pReady.completion = time;
          complete.push_back(pReady);
        }else{
          time++;
        }
    }
    // after running the job we check time again
  }
  while(!wLoad.empty()){
    Process pReady = wLoad.top();
    wLoad.pop();
    pReady.first_run = time;
    time = time + pReady.duration;
    pReady.completion = time;
    complete.push_back(pReady);
  }
  return complete;
}

list<Process> stcf(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_duration wLoad;
  std::map<int, int> durations;
  int time = 0;
  while(!workload.empty()){
    Process p = workload.top();
    if(p.arrival <= time){
      wLoad.push(p);
      workload.pop();
    // if a process starts after time then we run the shortest job
    // in the duration queue
    }else{
        if(!wLoad.empty()){
          Process pReady = wLoad.top();
          wLoad.pop();
          if(pReady.first_run == -1){
            pReady.first_run = time;
            durations[pReady.first_run] = pReady.duration; 
          }
          time++;
          pReady.duration--;
          if(pReady.duration <= 0){
            // need to save original duration
            pReady.duration = durations[pReady.first_run];
            pReady.completion = time;
            complete.push_back(pReady);
          }else{
            wLoad.push(pReady);
          }
        }else{
          time++;
        }
    }
    // after running the job we check time again
  }
  // workload is empty once time is past latest arrival time so anything finished after that time must come after
  while(!wLoad.empty()){
        Process pReady = wLoad.top();
        wLoad.pop();
        if(pReady.first_run == -1){
          pReady.first_run = time;
          durations[pReady.first_run] = pReady.duration; 
        }
        time++;
        pReady.duration--;
        if(pReady.duration <= 0){
          // need to save original duration
          pReady.duration = durations[pReady.first_run];
          pReady.completion = time;
          complete.push_back(pReady);
        }else{
          wLoad.push(pReady);
        }
  } 
  return complete;
}

list<Process> rr(pqueue_arrival workload) {
  list<Process> complete;
  list<Process> arrived;
  std::map<int, int> durations;
  int time;
  list<Process>::iterator i;
  if(!workload.empty()){
    time = workload.top().arrival;
    arrived.push_back(workload.top());
    workload.pop();
    i = arrived.begin();
  }
  while(!workload.empty()){
    Process p = workload.top();
    if(p.arrival <= time){
      arrived.push_back(p);
      workload.pop();
      continue;
    }
    Process pReady = *i;
    i = arrived.erase(i);
    if(pReady.first_run == -1){
      pReady.first_run = time;
      durations[time] = pReady.duration;
    }
    pReady.duration--;
    time++;
    if(pReady.duration <= 0){
      pReady.completion = time;
      pReady.duration = durations[pReady.first_run];
      complete.push_back(pReady);
    }else{
      arrived.insert(i, pReady);
    }
    // i++;/
  }
  while(!arrived.empty()){
    // problem i = end
    while(i != arrived.end()){
        Process pReady = *i;
        i = arrived.erase(i);
        // cout << pReady.arrival;
        if(pReady.first_run == -1){
          pReady.first_run = time;
          durations[time] = pReady.duration;
        }
        pReady.duration--;
        time++;
        if(pReady.duration <= 0){
          pReady.completion = time;
          pReady.duration = durations[pReady.first_run];
          complete.push_back(pReady);
        }else{
          arrived.insert(i, pReady);
        }
    }
    i = arrived.begin();
  }
  return complete;
}

float avg_turnaround(list<Process> processes) {
  int count = 0;
  float total = 0;
  for(auto const& p : processes){
    total += p.completion - p.arrival;
    count++;
  }
  float avg = -1;
  if(count > 0){
    avg = total / count;
  }
  return avg;
}

float avg_response(list<Process> processes) {
  int count = 0;
  float total = 0;
  for(auto const& p : processes){
    total += p.first_run - p.arrival;
    count++;
  }
  float avg = -1;
  if(count > 0){
    avg = total / count;
  }
  return avg;
}

void show_metrics(list<Process> processes) {
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_t << endl;
  cout << "Average Response Time:   " << avg_r << endl;
}


// void priorityBoost(*list<Process> q1, *list<Process> q2, *list<Process> q3, *list<Process> q4){
//   for(std::list<Process>::iterator it = *q2.begin(); it != *q2.end(); it++){
//     *q1.push_back(*q2(it));
//     *q2.pop_front();
//   }
//   for(std::list<Process>::iterator it = *q3.begin(); it != *q3.end(); it++){
//     *q1.push_back(*q3(it));
//     *q3.pop_front();
//   }
//   for(std::list<Process>::iterator it = *q4.begin(); it != *q4.end(); it++){
//     *q1.push_back(*q4(it));
//     *q4.pop_front();
//   }
  // reset time allotments
// }
list<Process> MLFQ(pqueue_arrival workload) {
  list<Process> q1;
  list<Process> q2;
  list<Process> q3;
  list<Process> q4;
  list<Process> complete;
  std::map<int, int> durations;
  int time;
  list<Process>::iterator i;
  std::map<int, int> usedTime;
  int limit1 = 100;
  int limit2 = 10;
  int limit3 = 15;
  int firstArrival;
  // priority queues
  // if multiple jobs have same priority run rr on them
  // new jobs placed in highest priority
  // // if job uses an entire time slice when running reduce priority
  // // if it givs up CPU before time slice ends stays in same priority
  // once a job uses up its time allotment at a lvel priority is reduced 
  // after some timeframe move all jobs to topmost queue
  // 
  if(!workload.empty()){
    time = workload.top().arrival;
    firstArrival = time;
    q1.push_back(workload.top());
    workload.pop();
  }
  while(!workload.empty() || !q1.empty() || !q2.empty() || !q3.empty() || !q4.empty()){
    if((time - firstArrival) % 50){
      // priorityBoost(&q1,&q2,&q3,&q4);
      while(!q2.empty()){
        Process p = q2.front();
        usedTime[p.first_run] = 0;
        q1.push_back(p);
        q2.pop_front();
      }
      while(!q3.empty()){
        Process p = q3.front();
        usedTime[p.first_run] = 0;
        q1.push_back(p);
        q3.pop_front();
      }
      while(!q4.empty()){
        Process p = q4.front();
        usedTime[p.first_run] = 0;
        q1.push_back(p);
        q4.pop_front();
      }
    }
    if(!workload.empty()){
      if(time >= workload.top().arrival){
        q1.push_back(workload.top());
        workload.pop();
      }
    }
    if(!q1.empty()){
      Process pReady = q1.front();
      q1.pop_front();
      if(pReady.first_run == -1){
        pReady.first_run = time;
        usedTime[time] = 0;
      }
      pReady.duration--;
      usedTime[pReady.first_run]++;
      if(pReady.duration > 0){
      // decrement time allotment
        if(usedTime[pReady.first_run] > limit1){
          usedTime[pReady.first_run] = 0;
          q2.push_back(pReady);
        }else{
          q1.push_back(pReady);
        }       
      }else{
        pReady.completion = time;
        complete.push_back(pReady);
      }

    }else if(!q2.empty()){
      Process pReady = q2.front();
      q2.pop_front();
      if(pReady.first_run == -1){
        pReady.first_run = time;
        usedTime[time] = 0;
      }
      pReady.duration--;
      usedTime[pReady.first_run]++;
      if(pReady.duration > 0){
      // decrement time allotment
        if(usedTime[pReady.first_run] > limit2){
          usedTime[pReady.first_run] = 0;
          q3.push_back(pReady);
        }else{
          q2.push_back(pReady);
        }       
      }else{
        pReady.completion = time;
        complete.push_back(pReady);
      }

    }else if(!q3.empty()){
      Process pReady = q3.front();
      q3.pop_front();
      if(pReady.first_run == -1){
        pReady.first_run = time;
        usedTime[time] = 0;
      }
      pReady.duration--;
      usedTime[pReady.first_run]++;
      if(pReady.duration > 0){
      // decrement time allotment
        if(usedTime[pReady.first_run] > limit3){
          usedTime[pReady.first_run] = 0;
          q4.push_back(pReady);
        }else{
          q3.push_back(pReady);
        }       
      }else{
        pReady.completion = time;
        complete.push_back(pReady);
      }
    }else{
      Process pReady = q4.front();
      q4.pop_front();
      if(pReady.first_run == -1){
        pReady.first_run = time;
        usedTime[time] = 0;
      }
      pReady.duration--;
      if(pReady.duration > 0){
      // decrement time allotment
        q4.push_back(pReady);       
      }else{
        pReady.completion = time;
        complete.push_back(pReady);
      }
    }
    time++;
  }
  return complete;
}
