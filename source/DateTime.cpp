
#include <schedule/DateTime.hpp>
#include <string>
#include <iomanip>
#include <cstring> //< for memset


std::time_t makeTime( const DateTime& dateTime )
{
    // convert to time since epoch.
    tm tm;
    tm.tm_year = dateTime.YEAR - 1900;
    tm.tm_mon = dateTime.MONTH - 1;
    tm.tm_mday = dateTime.DAY;
    tm.tm_hour = dateTime.HOUR ;
    tm.tm_min = dateTime.MINUTE;
    tm.tm_sec = dateTime.SECOND;
    return mktime(&tm);
}

DateTime makeDateTime( const std::time_t& t )
{
    std::tm* tm = localtime( &t );
    DateTime dateTime;
    dateTime.YEAR = 1900 + tm->tm_year;
    dateTime.MONTH = 1 + tm->tm_mon;
    dateTime.DAY = tm->tm_mday;
    dateTime.HOUR = tm->tm_hour;
    dateTime.MINUTE = tm->tm_min;
    dateTime.SECOND = tm->tm_sec;
    dateTime.MILLISEC = 0;
    return dateTime;
}

DateTime Now()
{
    time_t now = time( nullptr );
    return makeDateTime( now );
}

int32_t weekDay( const DateTime& dateTime )
{
    std::time_t t = makeTime( dateTime );
    std::tm* tm = localtime(&t);
    return tm->tm_wday;
}

DateTime addDays( const DateTime& dateTime, const int32_t& days )
{
    DateTime out = dateTime;
    out.DAY += days;
    return out;
}

DateTime zeroTime( const DateTime& dateTime )
{
    DateTime out = dateTime;
    out.HOUR = out.MINUTE = out.SECOND = 0;
    return out;
}

DateTime Parse( const std::string& dateTimeStr, const std::string& formatStr )
{
    DateTime dateTime;
    memset( &dateTime, 0, sizeof(DateTime) );

    if ( formatStr == "YYYY-MM-DDTHH:MM:SS" )
    {
        sscanf( (char *)dateTimeStr.c_str(), "%hu-%hu-%huT%hu:%hu:%hu",
            &dateTime.YEAR, &dateTime.MONTH, &dateTime.DAY, &dateTime.HOUR,
            &dateTime.MINUTE,&dateTime.SECOND );
    }
    else if ( formatStr == "YYYYMMDDHHMMSS" )
    {
        sscanf( (char *)dateTimeStr.c_str(), "%4hu%2hu%2hu%2hu%2hu%2hu",
            &dateTime.YEAR, &dateTime.MONTH, &dateTime.DAY, &dateTime.HOUR,
            &dateTime.MINUTE, &dateTime.SECOND );
    }

    return dateTime;
}

std::string tostring( const DateTime& dateTime, const std::string formatStr )
{
    if ( formatStr == "YYYY-MM-DDTHH:MM:SS" )
    {
        char buffer[32];
        sprintf( buffer, "%hu-%hu-%huT%hu:%hu:%hu",
            dateTime.YEAR, dateTime.MONTH, dateTime.DAY, dateTime.HOUR,
            dateTime.MINUTE, dateTime.SECOND );
        return buffer;
    }

    return "";
}

bool operator <= ( const DateTime& lhs, const DateTime& rhs )
{
    return ( makeTime( lhs ) <= makeTime( rhs ) );
}

bool operator >= ( const DateTime& lhs, const DateTime& rhs )
{
    return ( makeTime( lhs ) >= makeTime( rhs ) );
}

bool operator < ( const DateTime& lhs, const DateTime& rhs )
{
    return ( makeTime( lhs ) < makeTime( rhs ) );
}

bool operator > ( const DateTime& lhs, const DateTime& rhs )
{
    return ( makeTime( lhs ) > makeTime( rhs ) );
}