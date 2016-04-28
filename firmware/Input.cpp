#include "Input.h"

//USB and Xbox gamepad library
#include <XBOXUSB.h>
#include <usbhub.h>
#include <spi4teensy3.h>

USB Usb;
USBHub USBHub(&Usb);

XBOXUSB xboxController[CONCURRENT_APPS] = {
  XBOXUSB(&Usb),
#if CONCURRENT_APPS > 1
  XBOXUSB(&Usb),
#endif
#if CONCURRENT_APPS > 2
  XBOXUSB(&Usb),
#endif
#if CONCURRENT_APPS > 3
  XBOXUSB(&Usb),
#endif
};

#define MAX_LEDS 4
LEDEnum ledEnums[MAX_LEDS] = {
  LED1,
  LED2, 
  LED4,
  LED3,
};

boolean usbInitialized = false;

void initializeUsb()
{
  #ifndef DISABLE_USB

  Serial.println("Initialize USB");
  //Init USB
  if (Usb.Init() == -1) {
    Serial.println(F("\r\nOSC did not start"));
    while (1); //halt
  } else { 
    Serial.println(F("\r\nUSB init successfull"));
  }
  usbInitialized = true;
  
  #endif
}

void UsbTask(){
  #ifndef DISABLE_USB
  Usb.Task();
  #endif
}

Input::Input(int controllerIndex) : controllerIndex(controllerIndex), prevControl(BTN_NONE), curControl(BTN_NONE)
{
}

void Input::init()
{
  if(!usbInitialized) 
  {
    initializeUsb(); 
  }
}

uint8_t Input::readReal(){
  /* Return the pressed button even if it's held down
   * The exception is the start button, because it's only used to return
   * to the menu
   */
  xboxController[controllerIndex].setLedOn(ledEnums[controllerIndex % MAX_LEDS]);
  curControl = BTN_NONE;
  if (xboxController[controllerIndex].Xbox360Connected) {
    if (xboxController[controllerIndex].getButtonPress(B) || xboxController[controllerIndex].getButtonPress(RIGHT)) 
    {
      curControl = BTN_RIGHT;
    }
    else if (xboxController[controllerIndex].getButtonPress(X) || xboxController[controllerIndex].getButtonPress(LEFT))
    {
      curControl = BTN_LEFT;
    }
    else if (xboxController[controllerIndex].getButtonPress(Y) || xboxController[controllerIndex].getButtonPress(UP))
    {
      curControl = BTN_UP;
    }
    else if (xboxController[controllerIndex].getButtonPress(A) || xboxController[controllerIndex].getButtonPress(DOWN))
    {
      curControl = BTN_DOWN;
    }
    else if (xboxController[controllerIndex].getButtonPress(START) || xboxController[controllerIndex].getButtonPress(XBOX))
    {
      curControl = BTN_START;
    }
      
    if (curControl == prevControl && curControl == BTN_START)
      curControl = BTN_NONE;
    else
      prevControl = curControl;
  }
  return curControl;
}

uint8_t Input::read(){
  xboxController[controllerIndex].setLedOn(ledEnums[controllerIndex % MAX_LEDS]);
  if (xboxController[controllerIndex].Xbox360Connected) {
    if (xboxController[controllerIndex].getButtonPress(B) || xboxController[controllerIndex].getButtonPress(RIGHT)) 
    {
      curControl = BTN_RIGHT;
    }
    else if (xboxController[controllerIndex].getButtonPress(X) || xboxController[controllerIndex].getButtonPress(LEFT))
    {
      curControl = BTN_LEFT;
    }
    else if (xboxController[controllerIndex].getButtonPress(Y) || xboxController[controllerIndex].getButtonPress(UP))
    {
      curControl = BTN_UP;
    }
    else if (xboxController[controllerIndex].getButtonPress(A) || xboxController[controllerIndex].getButtonPress(DOWN))
    {
      curControl = BTN_DOWN;
    }
    else if (xboxController[controllerIndex].getButtonPress(START) || xboxController[controllerIndex].getButtonPress(XBOX))
    {
      curControl = BTN_START;
    }
      
    if (curControl == prevControl)
      curControl = BTN_NONE;
    else
      prevControl = curControl;
  }
  return curControl;
}

boolean Input::getL1R1Status(){
  if (xboxController[controllerIndex].Xbox360Connected) {
     return xboxController[controllerIndex].getButtonPress(L1) && xboxController[controllerIndex].getButtonPress(R1);
  }
  return false;
}

void Input::startRumble()
{
   xboxController[controllerIndex].setRumbleOn(0, 255);
}

void Input::stopRumble()
{
   xboxController[controllerIndex].setRumbleOn(0, 0);
}

