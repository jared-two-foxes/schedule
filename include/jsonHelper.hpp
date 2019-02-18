#ifndef SCHEDULE_JSONHELPER_HPP__
#define SCHEDULE_JSONHELPER_HPP__

#include <schedule/DateTime.hpp>
#include <rapidjson/document.h>
#include <assert.h>


template <typename T >
inline T json_cast( rapidjson::Value& v );

template <>
inline bool json_cast( rapidjson::Value& v )
{
    assert( !v.IsNull() );
    assert( v.IsBool() );
    return v.GetBool();
}

template <>
inline int json_cast( rapidjson::Value& v )
{
    assert( !v.IsNull() );
    assert( v.IsInt() );
    return v.GetInt();
}

template <>
inline float json_cast( rapidjson::Value& v )
{
    assert( !v.IsNull() );
    assert( v.IsFloat() );
    return v.GetFloat();
}

template <>
inline const char* json_cast( rapidjson::Value& v )
{
    assert( !v.IsNull() );
    assert( v.IsString() );
    return v.GetString();
}

template <>
inline DateTime json_cast( rapidjson::Value& v )
{
    assert( !v.IsNull() );
    assert( v.IsString() );
    std::string str = v.GetString();
    return Parse( v.GetString(), "YYYY-MM-DDTHH:MM:SS" );
}


template <typename T >
inline T json_cast_with_default( rapidjson::Value& v, T defaultValue );

template <>
inline int json_cast_with_default( rapidjson::Value& v, int defaultValue )
{
    //assert( !v.IsNull() );
    if ( v.IsInt() )
    {
        return v.GetInt();
    }
    return defaultValue;
}

template <>
inline float json_cast_with_default( rapidjson::Value& v, float defaultValue )
{
    //assert( !v.IsNull() );
    if ( v.IsFloat() )
    {
        return v.GetFloat();
    }
    return defaultValue;
}

template <>
inline const char* json_cast_with_default( rapidjson::Value& v, const char* defaultValue )
{
    //assert( !v.IsNull() );
    if ( v.IsString() )
    {
        return v.GetString();
    }
    return defaultValue;
}

template <>
inline DateTime json_cast_with_default( rapidjson::Value& v, DateTime defaultValue )
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
