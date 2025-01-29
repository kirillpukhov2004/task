#include "utilities.hpp"

#include <iostream>

std::chrono::seconds convert_string_to_time(const std::string& hours_str,
                                            const std::string& minutes_str) {
  const int hours_int = std::stoi(hours_str);
  const int minutes_int = std::stoi(minutes_str);
  if (hours_int < 0 || hours_int > 23 || minutes_int < 0 || minutes_int > 59) {
    throw std::invalid_argument("Invalid hours or minutes");
  }
  return std::chrono::hours(hours_int) + std::chrono::minutes(minutes_int);
}

std::string convert_time_to_string(const std::chrono::seconds& time) {
  const auto adjusted_time = time % std::chrono::hours(24);
  return std::format("{:%R}", std::chrono::hh_mm_ss(adjusted_time));
}

void print_client_leave(const std::chrono::seconds& time,
                        const std::string& client_name) {
  std::cout << std::format("{} 11 {}", convert_time_to_string(time),
                           client_name)
            << std::endl;
}

void print_client_took_table(const std::chrono::seconds& time,
                             const std::string& client_name,
                             const int& table_id) {
  std::cout << std::format("{} 12 {} {}", convert_time_to_string(time),
                           client_name, table_id)
            << std::endl;
}

void print_error(const std::chrono::seconds& time, const std::string& message) {
  std::cout << std::format("{} 13 {}", convert_time_to_string(time), message)
            << std::endl;
}