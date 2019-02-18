#ifndef SCHEDULE_OPERATIONS_TRANSFORMS_HPP__
#define SCHEDULE_OPERATIONS_TRANSFORMS_HPP__

#include <schedule/DateTime.hpp>
#include <string>
#include <vector>

struct Opportunity;

namespace schedule {
namespace operations {

std::vector<Opportunity > filterByWeek(
  std::vector<Opportunity > const & opportunities, DateTime const & dateTime );

}
}

#endif // SCHEDULE_OPERATIONS_TRANSFORMS_HPP__
