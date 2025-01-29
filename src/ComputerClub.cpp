#include "ComputerClub.hpp"

#include <iostream>

#include "utilities.hpp"

ComputerClub::ComputerClub(const int number_of_tables, const int price_per_hour,
                           const std::chrono::seconds opening_time,
                           const std::chrono::seconds closing_time)
    : opening_time_(opening_time),
      closing_time_(closing_time),
      price_per_hour_(price_per_hour) {
  tables_.reserve(number_of_tables);
  for (size_t i = 0; i < number_of_tables; i++) {
    tables_.emplace_back(i + 1, price_per_hour_);
  }
}

void ComputerClub::handle_event(const Event& event) {
  switch (event.id()) {
    case 1: {
      if (event.time() < opening_time_ || event.time() >= closing_time_) {
        print_error(event.time(), "NotOpenYet");
        break;
      }

      if (clients_.contains(event.client_name())) {
        print_error(event.time(), "YouShallNotPass");
        break;
      }

      clients_.insert(event.client_name());

      break;
    }
    case 2: {
      if (!event.table_number().has_value()) {
        throw;
      }

      const int table_number = event.table_number().value();
      if (table_number < 1 || table_number > tables_.size()) {
        throw;
      }

      if (!clients_.contains(event.client_name())) {
        print_error(event.time(), "ClientUnknown");
        break;
      }

      Table& new_table = tables_[table_number - 1];
      if (new_table.isOccupied()) {
        print_error(event.time(), "PlaceIsBusy");
        break;
      }

      if (client_to_table_.contains(event.client_name())) {
        // Moving client from old table to the new one
        int old_table_number = client_to_table_[event.client_name()];
        Table& old_table = tables_[old_table_number - 1];
        new_table.transferSession(old_table);
      } else {
        new_table.startSession(event.client_name(), event.time());
      }

      // Updating client to table connections
      client_to_table_[event.client_name()] = table_number;

      break;
    }
    case 3: {
      if (!clients_.contains(event.client_name())) {
        print_error(event.time(), "ClientUnknown");
        break;
      }

      int number_of_occupied_tables = 0;
      for (const auto& table : tables_) {
        if (table.isOccupied()) {
          number_of_occupied_tables++;
        }
      }

      if (number_of_occupied_tables < tables_.size()) {
        print_error(event.time(), "ICanWaitNoLonger!");
      }

      if (waiting_queue_.size() >= tables_.size()) {
        print_client_leave(event.time(), event.client_name());
        clients_.erase(event.client_name());
      } else {
        waiting_queue_.push(event.client_name());
      }

      break;
    }
    case 4: {
      if (!clients_.contains(event.client_name())) {
        print_error(event.time(), "ClientUnknown");
        break;
      }

      clients_.erase(event.client_name());
      if (client_to_table_.contains(event.client_name())) {
        int table_number = client_to_table_[event.client_name()];
        client_to_table_.erase(event.client_name());
        Table& table = tables_[table_number - 1];
        table.endSession(event.time());

        if (!waiting_queue_.empty()) {
          std::string next_client = waiting_queue_.front();
          waiting_queue_.pop();

          client_to_table_[next_client] = table_number;
          table.startSession(next_client, event.time());
          print_client_took_table(event.time(), next_client,
                                  table_number);
        }
      }

      break;
    }
    default:
      break;
  }
}
void ComputerClub::close_club() {
  for (auto& client_name : clients_) {
    if (client_to_table_.contains(client_name)) {
      const int table_number = client_to_table_[client_name];
      tables_[table_number - 1].endSession(closing_time_);
    }

    print_client_leave(closing_time_, client_name);
  }
}

void ComputerClub::print_report() const {
  for (auto& table : tables_) {
    std::cout << std::format("{} {} {:%R}", table.id(), table.totalRevenue(),
                             table.totalTime())
              << std::endl;
  }
}
std::chrono::seconds ComputerClub::opening_time() const {
  return opening_time_;
}

std::chrono::seconds ComputerClub::closing_time() const {
  return closing_time_;
}
