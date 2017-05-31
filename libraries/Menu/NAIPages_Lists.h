#ifndef Pages_Lists_h
#define Pages_Lists_h
#include "NAIMenu.h"

/*
Collection of pages and pre-made lists for inclusion into a MenuClass object.
*/
pages ack,
accept,//finish adjustment, pass data, do not change menu
home,//startup
	enabled,//running
	style,//set style of mecanum (RC or VideoGame)
	config,//configuration page
		configsticks,//configure joysticks
			xstick,//set x axis stick
			ystick,//set y axis stick
			zstick,//set z axis stick
			wstick,//set w axis stick
		configdrive,//configure drive mode
		configknobs,//configure knobs
			configspeed,//set speed knob
			configrotation,//set rotation knob
			configother1,
			configother2,
		configbuttons,//config buttons
		configswitches,
	calibrate,//calibration page
		calarm,//calibrate arm page
			calall,
			callower,
			calupper,
			calwrist,
			calgripper,
		caldrive,//calibrate drive page
			calm,//double check you want to calibrate motors
		calknobs,//calibrate knobs page
		calsticks,//calibrate sticks page
			calsticks2,
		calgyro,//calibrate gyro
	resetcontroller;


_oplists blanklist,knoblist,stickoptions,knoboptions,buttonoptions;

#endif