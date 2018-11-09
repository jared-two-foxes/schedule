#ifndef SCHEDULE_AUTHENTICATE_HPP_
#define SCHEDULE_AUTHENTICATE_HPP_

#include <schedule/api/status.hpp>

#include <string>

namespace schedule {

// Could use an Expected<> or somethign here as a return.
googleapi::util::Status authenticate( /*transportlayer* transport,*/
  const std::string& client_secret_file/*, credential* credential*/  );

}

#endif // SCHEDULE_AUTHENTICATE_HPP_
