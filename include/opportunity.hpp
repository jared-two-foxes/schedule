#ifndef SCHEDULE_OPPORTUNITY_HPP__
#define SCHEDULE_OPPORTUNITY_HPP__

#include <schedule/DateTime.hpp>
#include <string>

struct Opportunity
{
  int id;
  std::string subject;
  std::string description;
  std::string number;
  std::string state_name;
  std::string status_name;
  std::string reference;
  DateTime created_at;
  DateTime updated_at;
  DateTime starts_at;
  DateTime ends_at;
  bool customer_collecting;
  bool customer_returning;
  std::string delivery_instructions;
  DateTime deliver_starts_at;
  DateTime deliver_ends_at;
  DateTime collect_starts_at;
  DateTime collect_ends_at;
};

#endif // SCHEDULE_OPPORTUNITY_HPP__
