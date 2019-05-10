#ifndef SCHEDULE_RENDERER_HPP__
#define SCHDEULE_RENDERER_HPP__

#include <terminal/terminal.hpp>
#include <terminal/components/component.hpp>
#include <vector>

struct Opportunity;

class Renderer
///
/// Is responsable for rendering the state of the application.
///
{
private:
  const int32_t m_maxElements = 20;

  framework::Component m_layout;
  int32_t m_currentIndex = 0;

public:
  Renderer();

  void clear();

  void addLine( std::string const & line );
  void addOpportunities( std::vector<Opportunity > const & opportunities );

  void advanceElements( int32_t elements );
  void advanceElements();

  framework::terminal present( framework::terminal const & terminal );

};

#endif // SCHEDULE_RENDERER_HPP__
