#ifndef CURRENTRMS_API_OPPORTUNITY_HPP__
#define CURRENTRMS_API_OPPORTUNITY_HPP__

#include <foundation/uuid.hpp>
#include <rapidjson/document.h>


struct opportunity;

uuid convert( rapidjson::Value& v, opportunity** op );


#endif //
