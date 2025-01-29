#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

struct Session {
  std::string client_name;
  std::chrono::seconds start_time;
  std::chrono::seconds end_time;

  [[nodiscard]] std::chrono::seconds totalTime() const;
};

class Table {
private:
  int id_;
  int price_per_hour_;
  std::optional<Session> current_session_{};
  std::vector<Session> sessions_{};

public:
  Table(int id, int price_per_hour);

  void startSession(const std::string& client_name, const std::chrono::seconds& start_time);

  void endSession(const std::chrono::seconds& end_time);

  void transferSession(Table& old_table);

  [[nodiscard]] bool isOccupied() const;

  [[nodiscard]] std::chrono::seconds totalTime() const;

  [[nodiscard]] int totalRevenue() const;

  [[nodiscard]] int id() const;
};