#ifndef SCHEDULE_JSONHELPER_HPP__
#define SCHEDULE_JSONHELPER_HPP__

#include <schedule/DateTime.hpp>
#include <assert.h>


template <typename T >
T json_cast( rapidjson::Value& v );

template <>
bool json_cast( rapidjson::Value& v )
{
    assert( !v.IsNull() );
    assert( v.IsBool() );
    return v.GetBool();
}

template <>
int json_cast( rapidjson::Value& v )
{
    assert( !v.IsNull() );
    assert( v.IsInt() );
    return v.GetInt();
}

template <>
float json_cast( rapidjson::Value& v )
{
    assert( !v.IsNull() );
    assert( v.IsFloat() );
    return v.GetFloat();
}

template <>
const char* json_cast( rapidjson::Value& v )
{
    assert( !v.IsNull() );
    assert( v.IsString() );
    return v.GetString();
}

template <>
DateTime json_cast( rapidjson::Value& v )
{
    assert( !v.IsNull() );
    assert( v.IsString() );
    std::string str = v.GetString();
    return Parse( v.GetString(), "YYYY-MM-DDTHH:MM:SS" );
}


template <typename T >
T json_cast_with_default( rapidjson::Value& v, T defaultValue );

template <>
int json_cast_with_default( rapidjson::Value& v, int defaultValue )
{
    //assert( !v.IsNull() );
    if ( v.IsInt() )
    {
        return v.GetInt();
    }
    return defaultValue;
}

template <>
float json_cast_with_default( rapidjson::Value& v, float defaultValue )
{
    //assert( !v.IsNull() );
    if ( v.IsFloat() )
    {
        return v.GetFloat();
    }
    return defaultValue;
}

template <>
const char* json_cast_with_default( rapidjson::Value& v, const char* defaultValue )
{
    //assert( !v.IsNull() );
    if ( v.IsString() )
    {
        return v.GetString();
    }
    return defaultValue;
}

template <>
DateTime json_cast_with_default( rapidjson::Value& v, DateTime defaultValue )
{
    //assert( !v.IsNull() );
    if ( v.IsString() )
    {
        std::string str = v.GetString();
        return Parse( v.GetString(), "YYYY-MM-DDTHH:MM:SS" );
    }
    return defaultValue;
}

#endif // SCHEDULE_JSONHELPER_HPP__
