#pragma once

#include <chrono>
#include <map>
#include <set>
#include <string>

#include "Event.hpp"
#include "Table.hpp"

class ComputerClub {
 private:
  std::chrono::seconds opening_time_;
  std::chrono::seconds closing_time_;
  int price_per_hour_;
  std::vector<Table> tables_{};
  std::set<std::string> clients_{};
  std::queue<std::string> waiting_queue_{};
  std::map<std::string, int> client_to_table_{};

 public:
  ComputerClub(int number_of_tables, int price_per_hour,
               std::chrono::seconds opening_time,
               std::chrono::seconds closing_time);

  void handle_event(const Event& event);

  void close_club();

  void print_report() const;

  std::chrono::seconds opening_time() const;

  std::chrono::seconds closing_time() const;
};