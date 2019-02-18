#ifndef SCHEDULE_RENDERER_HPP__
#define SCHDEULE_RENDERER_HPP__

#include <terminal/terminal.hpp>
#include <vector>

struct Opportunity;

class Renderer
///
/// Is responsable for rendering the state of the application.
///
{
private:
  int32_t m_maxElements = 20;
  int32_t m_currentIndex = 0;

public:
  framework::terminal renderSingleFrame( framework::terminal const & terminal,
    std::vector<Opportunity > const & opportunities );

  void advanceElements( int32_t elements );
  void advanceElements();

};

#endif // SCHEDULE_RENDERER_HPP__
