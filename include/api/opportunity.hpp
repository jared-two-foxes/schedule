#ifndef CURRENTRMS_API_OPPORTUNITY_HPP__
#define CURRENTRMS_API_OPPORTUNITY_HPP__

#include <foundation/uuid.hpp>
#include <rapidjson/document.h>


namespace current_rms {

struct opportunity;

void clearOpportunities();

std::uint32_t getOpportunityCount();

uuid convert( rapidjson::Value& v, opportunity** op );

}

#endif //
