#pragma once

#include <chrono>
#include <optional>
#include <regex>
#include <string>

class Event {
 private:
  int id_;
  std::chrono::seconds time_;
  std::string client_name_;
  std::optional<int> table_number_;

 public:
  Event(int id, const std::chrono::seconds& time,
        const std::string& client_name, std::optional<int> table_number);

  [[nodiscard]] int id() const;

  [[nodiscard]] std::chrono::seconds time() const;

  [[nodiscard]] std::string client_name() const;

  [[nodiscard]] std::optional<int> table_number() const;
};