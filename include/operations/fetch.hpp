#ifndef SCHEDULE_OPERATIONS_TRANSFORM_HPP__
#define SCHEDULE_OPERATIONS_TRANSFORM_HPP__

#include <string>
#include <vector>

struct Opportunity;

namespace schedule {
namespace operations {

//@todo: replace this args string with something actually meaningful.
std::vector<Opportunity > fetchFromCurrentRms(
  std::vector<std::string > const & args );

}
}

#endif // SCHEDULE_OPERATIONS_TRANSFORM_HPP__
