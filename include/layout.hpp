#ifndef SCHEDULE_SHOW_HPP__
#define SCHEDULE_SHOW_HPP__

#include <schedule/opportunity.hpp>
#include <terminal/components/component.hpp>
#include <terminal/components/stacklayout.hpp>
#include <terminal/components/flowlayout.hpp>
#include <foundation/base/functional.hpp>
#include <foundation/strings/utils.hpp>

template <typename T >
framework::Component render( T const & );

template <typename... Elements >
framework::Component stack( Elements... el ) {
    return framework::StackLayout<> {
        render( el )...,
    };
}

template <typename... Elements >
framework::Component flow( Elements... el ) {
    return framework::FlowLayout<> {
        render( el )...,
    };
}

template <typename T >
framework::Component render( T const & o ) {
  return framework::Text { foundation::toString( o ) };
}

template <>
framework::Component render( Opportunity const & op ) {
  return flow(
    op.subject,
    std::string(", "),
    op.state_name,
    std::string(", "),
    op.number,
    std::string(", "),
    op.starts_at,
    std::string(", "),
    op.ends_at
  );
}

template <>
framework::Component render( DateTime const & datetime ) {
  return framework::Text( tostring(datetime) );
}

template <typename Iterator >
framework::Component render( Iterator start, Iterator end ) {
  std::vector<framework::Component> out;
  std::transform( start, end,
    std::back_inserter(out),
    [](auto const & o){ return render( o ); });
  return framework::StackLayout<>(out);
}


#endif // SCHEDULE_SHOW_HPP__
