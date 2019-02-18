#ifndef CURRENTRMS_API_OPPORTUNITY_HPP__
#define CURRENTRMS_API_OPPORTUNITY_HPP__

#include <string>
#include <vector>

namespace current_rms {

// Interestingly I'm starting to feel like its going to be best to describe
// these interfaces as interacting with basic string objects (and internally handle
// some conversion to Json objects where required) rather than creating an "official"
// structure directly and exposing that.
//
// Firstly because when dealing with the communication itself the current_rms
// server is expecting the object to be in Json and as currently listed there is
// alot of data in the object which honestly I'm not sure I want or need on the
// client side but it would be good to wrap this as an entry point to ensure
//
// that we have everything required for communication.
// Secondly, the conversion from and to json almost feels like it should be
// application domain knowledge as the format the information is stored locally
// honestly doesnt really matter to current-rms thus I'm not sure that I care to
// put it here.  This should simply be a means of communication to and from their
// server.

void Opportunities_GET( std::size_t page, std::size_t count,
  std::string* uri, std::vector<std::pair<std::string, std::string > >* options,
  std::string* body );

// void Opportunity_Post( std::size_t page, std::size_t count,
//   std::string* uri, OperationType* operation, ParameterList* options,
//   std::string* body ); ??

}

#endif //
