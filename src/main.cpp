#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>

#include "ComputerClub.hpp"
#include "Event.hpp"
#include "utilities.hpp"

ComputerClub parse_computer_club(std::ifstream& input);

Event parse_event(const std::string& line);

int main(int argc, char** argv) {
  try {
    if (argc < 2) {
      throw std::runtime_error(std::format("Usage: {} <input_file>", argv[0]));
    }
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
      throw std::runtime_error("Error: Couldn't open input file");
    }

    auto computer_club = parse_computer_club(input);

    std::cout << std::format("{:%R}", computer_club.opening_time())
              << std::endl;

    std::string line;
    while (std::getline(input, line)) {
      std::cout << line << std::endl;

      const auto event = parse_event(line);
      computer_club.handle_event(event);
    }

    computer_club.close_club();

    std::cout << std::format("{:%R}", computer_club.closing_time())
              << std::endl;

    computer_club.print_report();
  } catch (std::exception& exception) {
    std::cout << exception.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

ComputerClub parse_computer_club(std::ifstream& input) {
  const std::regex price_per_hour_regex(R"(^\d+$)");

  std::string line;
  std::smatch match;

  // Read number of tables
  std::getline(input, line);
  if (!std::regex_match(line, match, std::regex((R"(^\d+$)")))) {
    throw std::runtime_error(line);
  }
  int number_of_tables = std::stoi(match[0]);
  if (number_of_tables < 1) {
    throw std::runtime_error(line);
  }

  // Read opening and closing times
  std::chrono::seconds opening_time;
  std::chrono::seconds closing_time;
  std::getline(input, line);
  if (!std::regex_match(line, match,
                        std::regex(R"(^(\d{2}):(\d{2}) (\d{2}):(\d{2})$)"))) {
    throw std::runtime_error(line);
  }
  try {
    opening_time = convert_string_to_time(match[1], match[2]);
    closing_time = convert_string_to_time(match[3], match[4]);
  } catch (...) {
    throw std::runtime_error(line);
  }
  if (opening_time >= closing_time) {
    throw std::runtime_error(line);
  }

  // Read price per hour
  std::getline(input, line);
  if (!std::regex_match(line, match, price_per_hour_regex)) {
    throw std::runtime_error(line);
  }
  int price_per_hour = std::stoi(line);
  if (price_per_hour < 1) {
    throw std::runtime_error(line);
  }

  return ComputerClub{number_of_tables, price_per_hour, opening_time,
                      closing_time};
};

Event parse_event(const std::string& line) {
  std::smatch match;
  if (!std::regex_match(
          line, match,
          std::regex(R"(^(\d{2}):(\d{2}) (\d+) (\S+)(?: (\S+))?$)"))) {
    throw std::runtime_error(line);
  }

  std::chrono::seconds time;
  int id;
  try {
    time = convert_string_to_time(match[1], match[2]);
    id = std::stoi(match[3]);
  } catch (...) {
    throw std::runtime_error(line);
  }
  if (id < 1 || id > 4) {
    throw std::runtime_error(line);
  }

  const auto client_name = match[4].str();

  std::optional<int> table_number{};
  if (id == 2) {
    if (!match[5].matched) {
      throw std::runtime_error(line);
    }

    table_number = std::stoi(match[5]);
  }

  return Event{id, time, client_name, table_number};
}