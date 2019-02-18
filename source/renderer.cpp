
#include <schedule/layout.hpp>
#include <schedule/opportunity.hpp>
#include <schedule/renderer.hpp>

#include <terminal/components/component.hpp>
#include <terminal/components/stacklayout.hpp>
#include <terminal/components/text.hpp>

framework::Component
renderOpportunities( std::vector<Opportunity > const & xs, int32_t index, int32_t maxElements )
{
  //assert( xs.size() > index );
  auto i = xs.begin() + index;
  bool needsElipses = std::distance(i, xs.end()) > maxElements;
  auto end = i + (needsElipses ? maxElements : std::distance(i, xs.end()));

  return framework::StackLayout<> {
    framework::Text( "Opportunities: " ),
    render( i, end ),
    needsElipses ? framework::Text( "..." ) : framework::Text( "" ),
    framework::Text( " " ),
    framework::Text( "Please enter a command:" )
  };
}

framework::terminal
Renderer::renderSingleFrame( framework::terminal const & terminal,
  std::vector<Opportunity > const & opportunities )
{
  Log( 0, "renderSingleFrame\n" );

  //framework::Component layout = state_ == 0 ? renderOpportunities() : renderAuthorization();
  framework::Component layout = renderOpportunities( opportunities, m_currentIndex, m_maxElements );
  std::string const next = layout.render(100).toString();
  std::string const prompt( ">> " );

  // Show the current state.
  return terminal.flip( next + prompt );
}

void
Renderer::advanceElements( int32_t elements )
{
  m_currentIndex += elements;
  if (m_currentIndex < 0)  {
    m_currentIndex == 0;
  }
}

void
Renderer::advanceElements()
{
  advanceElements(m_maxElements);
}
