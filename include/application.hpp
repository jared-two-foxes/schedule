#ifndef SCHEDULE_APPLICATION_HPP__
#define SCHEDULE_APPLICATION_HPP__

#include <schedule/renderer.hpp>
#include <terminal/terminal.hpp>
#include <foundation/dispatcher.hpp>
#include <vector>

struct Opportunity;

class Application
{
private:
  using CallbackSignature = void (const std::vector<std::string >& );

private:
  // Component/Systems
  framework::terminal terminal_;
  // Controllers
  framework::CommandDispatcher<std::string, CallbackSignature > dispatcher_;
  Renderer renderer_;
  // Data
  std::vector<Opportunity > masterOpportunitiesList_; // Extract out to a DataCache like object?
  std::vector<Opportunity > displayList_;
  bool quit_;

public:
  Application();
  ~Application();

  void setup();
  void processCmd( std::string const& cmd, std::vector<std::string > const& args );
  void run();

private:
  void initDispatcher();
  void prepareGoogleCredential(); //?

  // void announceAutheticationRequest();
  // void obtainAuthorizationToken( std::string const & authorization_code );
  // void pushToGoogleSheet();

};

#endif // SCHEDULE_APPLICATION_HPP__
