#ifndef SCHEDULE_FUNCTIONAL_HPP__
#define SCHEDULE_FUNCTIONAL_HPP__

template <typename T, typename Function >
inline
T filter( T& xs, Function fn )
{
    T out;
    for ( auto& op : xs )
    {
        if ( fn( op ) )
        {
            out.push_back( op );
        }
    }
    return out;
}

#endif // SCHEDULE_FUNCTIONAL_HPP__
