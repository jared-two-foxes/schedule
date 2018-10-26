#ifndef __DATETIME_HPP__
#define __DATETIME_HPP__

#include <string>
#include <cstdint>
#include <ctime>

struct DateTime
{
    uint16_t YEAR;
    uint16_t MONTH;
    uint16_t DAY;
    uint16_t HOUR;
    uint16_t MINUTE;
    uint16_t SECOND;
    uint16_t MILLISEC;
};


std::time_t makeTime( const DateTime& dateTime );
DateTime makeDateTime( const std::time_t& t );
DateTime Now();

int32_t weekDay( const DateTime& dateTime );
DateTime addDays( const DateTime& dateTime, const int32_t& days );
DateTime zeroTime( const DateTime& dateTime );

DateTime Parse( const std::string& dateTimeStr, const std::string& formatStr );

std::string tostring( const DateTime& dt, const std::string format = "YYYY-MM-DDTHH:MM:SS" );

bool operator <= ( const DateTime& lhs, const DateTime& rhs );
bool operator >= ( const DateTime& lhs, const DateTime& rhs );
bool operator < ( const DateTime& lhs, const DateTime& rhs );
bool operator > ( const DateTime& lhs, const DateTime& rhs );

#endif // __DATETIME_HPP__
