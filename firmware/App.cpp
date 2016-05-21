
#include "App.h"

#include "Menu.h"
#include "Rainbow.h"
#include "Stars.h"
#include "SnakeGame.h"
#include "TetrisGame.h"
#include "Pong.h"
#include "BrickBreaker.h"
#include "SpectrumAnalyzer.h"

App::App(size_t width, size_t height) : width(width), height(height),ended(false)
{
  
}

void App::handleInput(Input& input) 
{
  
}

void App::run(unsigned long tick)
{
  
}

void App::render(Canvas& canvas)
{
  
}

void App::controlFlow(AppController& appController)
{
  if(ended) {
    appController.startMenu();
  }
}



App::~App()
{
  
}


AppController::AppController(size_t width, size_t height) : width(width), height(height), app(NULL) 
{
  
}

void AppController::process(unsigned long tick, Input& input, Canvas& canvas) 
{
  if(app) {
    app->handleInput(input);
    app->run(tick);
    app->render(canvas);
    app->controlFlow(*this);
  }
}

void AppController::startMenu()
{
  App* newApp = new Menu(width, height);
  setNewApp(newApp);
}

void AppController::startRainbow()
{
  App* newApp = new Rainbow(width, height);
  setNewApp(newApp);
}

void AppController::startStars()
{
  App* newApp = new Stars(width, height);
  setNewApp(newApp);
}

void AppController::startTetris()
{
  App* newApp = new TetrisGame(width, height);
  setNewApp(newApp);
}

void AppController::startSnake()
{
  App* newApp = new SnakeGame(width, height);
  setNewApp(newApp);
}

void AppController::startPong()
{
  App* newApp = new Pong(width, height);
  setNewApp(newApp);
}

void AppController::startBrickBreaker()
{
  App* newApp = new BrickBreaker(width, height);
  setNewApp(newApp);
}

void AppController::startSpectrumAnalyzer()
{
  App* newApp = new SpectrumAnalyzer(width, height);
  setNewApp(newApp);
}

void AppController::setNewApp(App* newApp)
{
  freeCurrentApp();
  app = newApp;
  Serial.print("Set new App");
  Serial.println((int)newApp);
}

void AppController::freeCurrentApp()
{
  if(app) {
    delete app;
    app = NULL;
  }
}

AppController::~AppController()
{
  freeCurrentApp();
}


