#ifndef _APP_H_
#define _APP_H_

#include <Arduino.h>

#include "Canvas.h"
#include "Input.h"

class AppController;

class App {
  
protected:
  const size_t width;
  const size_t height;
  boolean ended;
  App(size_t width, size_t height);
public:
  virtual ~App();
  virtual void handleInput(Input& input);
  virtual void run(unsigned long tick);
  virtual void render(Canvas& canvas);
  virtual void controlFlow(AppController& appController);
};

class AppController {
private:
  const size_t width;
  const size_t height;
  App* app;
  void setNewApp(App* app);
  void freeCurrentApp();
public:
  AppController(size_t width, size_t height);
  virtual ~AppController();
  void process(unsigned long tick, Input& input, Canvas& canvas);
  void startMenu();
  void startRainbow();
  void startStars();
  void startTetris();
  void startSnake();
  void startPong();
  void startBrickBreaker();
};

#endif
