#pragma once

#include <chrono>
#include <string>

std::chrono::seconds convert_string_to_time(const std::string& hours_str,
                                            const std::string& minutes_str);

std::string convert_time_to_string(const std::chrono::seconds& time);

void print_client_leave(const std::chrono::seconds& time,
                        const std::string& client_name);

void print_client_took_table(const std::chrono::seconds& time,
                             const std::string& client_name,
                             const int& table_id);

void print_error(const std::chrono::seconds& time, const std::string& message);