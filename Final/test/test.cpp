#include <gtest/gtest.h>
#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <iterator>
#include <list>
#include <queue>

using namespace std;

TEST(SchedulingTest, SchedulingTest1) {
  pqueue_arrival pq = read_workload("workloads/workload_01.txt");
  EXPECT_EQ(pq.size(), 3);
}

TEST(SchedulingTest, SchedulingTest2) {
  pqueue_arrival pq = read_workload("workloads/not_a_file.txt");
  EXPECT_EQ(pq.size(), 0);
}

TEST(SchedulingTest, SchedulingTest3) {
  pqueue_arrival pq = read_workload("workloads/workload_01.txt");

  ASSERT_FALSE(pq.empty());
  Process x1 = pq.top();
  pq.pop();
  EXPECT_EQ(x1.arrival, 0);

  ASSERT_FALSE(pq.empty());
  Process x2 = pq.top();
  pq.pop();
  EXPECT_EQ(x2.arrival, 0);

  ASSERT_FALSE(pq.empty());
  Process x3 = pq.top();
  pq.pop();
  EXPECT_EQ(x3.arrival, 0);
}

TEST(SchedulingTest, SchedulingTest4) {
  pqueue_arrival pq = read_workload("workloads/workload_01.txt");

  ASSERT_FALSE(pq.empty());
  Process x1 = pq.top();
  pq.pop();
  EXPECT_EQ(x1.duration, 10);

  ASSERT_FALSE(pq.empty());
  Process x2 = pq.top();
  pq.pop();
  EXPECT_EQ(x2.duration, 10);

  ASSERT_FALSE(pq.empty());
  Process x3 = pq.top();
  pq.pop();
  EXPECT_EQ(x3.duration, 10);
}

TEST(SchedulingTest, SchedulingTest5) {
  pqueue_arrival pq = read_workload("workloads/workload_05.txt");
  show_workload(pq);
  list<Process> l = fifo(pq);
  for(auto const &v : l){
    cout << "_____" << "\n";
    cout << v.arrival << "\n";
    cout << v.first_run << "\n";
    cout << v.duration << "\n";
    cout << v.completion << "\n";
    cout << "_____" << "\n";
  }
  }

int main(int argc, char **argv) {
  pqueue_arrival pq = read_workload("workloads/workload_05.txt");
  // show_workload(pq);
  cout << "workload_05.txt" << "\n";
  cout << "rr" << "\n";
  list<Process> l = rr(pq);
  for(auto const &v : l){
    cout << "_____" << "\n";
    cout << v.arrival << "\n";
    cout << v.first_run << "\n";
    cout << v.duration << "\n";
    cout << v.completion << "\n";
    cout << "_____" << "\n";
  }
  cout << "MLFQ" << "\n";
  list<Process> e = MLFQ(pq);
  for(auto const &v : e){
    cout << "_____" << "\n";
    cout << v.arrival << "\n";
    cout << v.first_run << "\n";
    cout << v.duration << "\n";
    cout << v.completion << "\n";
    cout << "_____" << "\n";
  }
  pqueue_arrival pq1 = read_workload("workloads/workload_mlfq.txt");
  // show_workload(pq);
  cout << "workload_mlfq.txt" << "\n";
  cout << "rr" << "\n";
  list<Process> l1 = rr(pq1);
  for(auto const &v : l1){
    cout << "_____" << "\n";
    cout << v.arrival << "\n";
    cout << v.first_run << "\n";
    cout << v.duration << "\n";
    cout << v.completion << "\n";
    cout << "_____" << "\n";
  }
  cout << "MLFQ" << "\n";
  list<Process> e1 = MLFQ(pq1);
  for(auto const &v : e1){
    cout << "_____" << "\n";
    cout << v.arrival << "\n";
    cout << v.first_run << "\n";
    cout << v.duration << "\n";
    cout << v.completion << "\n";
    cout << "_____" << "\n";
  }
}
