#ifndef SCHEDULE_CONVERT_HPP__
#define SCHDEULE_CONVERT_HPP__

#include <rapidjson/document.h>

struct Opportunity;
Opportunity convert( rapidjson::Value& v );

#endif // SCHEDULE_CONVERT_HPP__
