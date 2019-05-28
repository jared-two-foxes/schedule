#ifndef SCHEDULE_JSONHELPER_HPP__
#define SCHEDULE_JSONHELPER_HPP__

#include <schedule/DateTime.hpp>
#include <rapidjson/document.h>
#include <assert.h>


template <typename T >
inline T json_cast( rapidjson::Value& v, const char* name );

template <>
inline bool json_cast( rapidjson::Value& v, const char* name )
{
    assert( v.HasMember( name ) );
    assert( v[name].IsBool() );
    return v[name].GetBool();
}

template <>
inline int json_cast( rapidjson::Value& v, const char* name )
{
    assert( v.HasMember( name ) );
    assert( v[name].IsInt() );
    return v[name].GetInt();
}

template <>
inline int64_t json_cast( rapidjson::Value& v, const char* name )
{
    assert( v.HasMember( name ) );
    assert( v[name].IsInt64() );
    return v[name].GetInt64();
}

template <>
inline float json_cast( rapidjson::Value& v, const char* name )
{
    assert( v.HasMember( name ) );
    assert( v[name].IsFloat() );
    return v[name].GetFloat();
}

template <>
inline const char* json_cast( rapidjson::Value& v, const char* name )
{
    assert( v.HasMember( name ) );
    assert( v[name].IsString() );
    return v[name].GetString();
}

template <>
inline DateTime json_cast( rapidjson::Value& v, const char* name )
{
    assert( v.HasMember( name ) );
    assert( v[name].IsString() );
    return Parse( v[name].GetString(), "YYYY-MM-DDTHH:MM:SS" );
}


template <typename T >
inline T json_cast_with_default( rapidjson::Value& v, const char* name, T defaultValue );

template <>
inline bool json_cast_with_default( rapidjson::Value& v, const char* name, bool defaultValue )
{
    if ( v.HasMember( name ) && v[name].IsBool() ) 
    {
        return v[name].GetBool();
    }
    return defaultValue;
}

template <>
inline int json_cast_with_default( rapidjson::Value& v, const char* name, int defaultValue )
{
    if ( v.HasMember( name ) && v[name].IsInt() ) 
    {
        return v[name].GetInt();
    }
    return defaultValue;
}

template <>
inline int64_t json_cast_with_default( rapidjson::Value& v, const char* name, int64_t defaultValue )
{
    if ( v.HasMember( name ) && v[name].IsInt64() ) 
    {
        return v[name].GetInt64();
    }
    return defaultValue;
}

template <>
inline float json_cast_with_default( rapidjson::Value& v, const char* name, float defaultValue )
{
    if ( v.HasMember( name ) && v[name].IsFloat() ) 
    {
        return v[name].GetFloat();
    }
    return defaultValue;
}

template <>
inline const char* json_cast_with_default( rapidjson::Value& v, const char* name, const char* defaultValue )
{
    if ( v.HasMember( name ) && v[name].IsString() ) 
    {
        return v[name].GetString();
    }
    return defaultValue;
}

template <>
inline DateTime json_cast_with_default( rapidjson::Value& v, const char* name, DateTime defaultValue )
{
    if ( v.HasMember( name ) && v[name].IsString() ) 
    {
        //@todo: Handle failure to parse...
        return Parse( v[name].GetString(), "YYYY-MM-DDTHH:MM:SS" );
    }
    return defaultValue;
}


#endif // SCHEDULE_JSONHELPER_HPP__
