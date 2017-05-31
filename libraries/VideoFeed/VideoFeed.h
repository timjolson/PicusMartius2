#ifndef VIDEOFEED_H
#define VIDEOFEED_H

#include <Arduino.h>

class VideoFeed {
    byte camera;    //which camera is being displayed
    int pin1, pin2; //pins to control IC
    
    char Name[10];  //name for printing
    
public:
    void NextFeed();    //increment displayed feed
    void PrevFeed();    //decrement displayed feed
    
    void SetFeed(int cameraNum);    //set specific feed
    byte GetFeed(){return camera;}; //get current selected feed
    
    void Init(const char* Name, int startupCamera);//give the feed a name and default camera#
    
    //constructor
    VideoFeed(int pin1, int pin2);//set the two control pins for the chip
    
};

extern VideoFeed Cam1;  //objects created in cpp, declared here so other files can access
extern VideoFeed Cam2;

#endif