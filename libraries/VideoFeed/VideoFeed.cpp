#include "Videofeed.h"

VideoFeed Cam1(30,31);  //create the objects and set pin #s to chip
VideoFeed Cam2(32,33);

//SetFeed(cam) sets the VideoFeed object to source "cam"(0-3) camera
void VideoFeed::SetFeed(int cam)
{
    switch(cam){
        case 0:
            digitalWrite(pin1,HIGH);
            digitalWrite(pin2,HIGH);
        break;
        case 1:
            digitalWrite(pin1,LOW);
            digitalWrite(pin2,HIGH);
        break;
        case 2:
            digitalWrite(pin1,LOW);
            digitalWrite(pin2,LOW);
        break;
        case 3:
            digitalWrite(pin1,HIGH);
            digitalWrite(pin2,LOW);
        break;
    }
    
    camera = cam;   //update camera status variable
    
    //print the change on Serial
    Serial.print(Name);
    Serial.print(" set to:\t");
    Serial.println(camera);
}

void VideoFeed::Init(const char* name, int start)
{
    strcpy(Name,name);			//display name for feed
    
    pinMode(pin1,OUTPUT);       //set pin to output
    digitalWrite(pin1,HIGH);    //write it default HIGH
    
    pinMode(pin2,OUTPUT);       //same as above
    digitalWrite(pin2,HIGH);
    
    SetFeed(start);             //set startup camera
}

VideoFeed::VideoFeed(int _pin1, int _pin2)
{
    pin1 = _pin1;
    pin2 = _pin2;
}

//NextFeed() cycles to the next camera, looping back to 0 after 3
void VideoFeed::NextFeed()
{
    SetFeed((camera+1)%4);
}

//PrevFeed() cycles to the previous camrea, looping back to 3 after 0
void VideoFeed::PrevFeed()
{
    SetFeed((camera-1)%4);
}