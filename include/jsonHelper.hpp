#ifndef SCHEDULE_JSONHELPER_HPP__
#define SCHEDULE_JSONHELPER_HPP__

#include <assert.h>


template <typename T >
T json_cast( rapidjson::Value& v );

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

#endif // SCHEDULE_JSONHELPER_HPP__
