#include "Event.hpp"

Event::Event(const int id, const std::chrono::seconds& time,
             const std::string& client_name,
             const std::optional<int> table_number)
    : id_(id),
      time_(time),
      client_name_(client_name),
      table_number_(table_number) {};

int Event::id() const {
  return id_;
}

std::chrono::seconds Event::time() const {
  return time_;
}

std::string Event::client_name() const {
  return client_name_;
}

std::optional<int> Event::table_number() const {
  return table_number_;
}