
import pygame, Buttons, os, time, spinbox, operator, math, string
from pygame.locals import *
from UDP_Library import *
from joystick_Library import *
from spinbox import *

#Initialize pygame
pygame.init()

#camera names
c1n = "Front"
c2n = "Bottom"
c3n = "Gripper"
c4n = "Rear"

#colors
#http://www.rapidtables.com/web/color/RGB_Color.htm
white = (255, 255, 255)
black = (0, 0, 0)
red = (255, 0, 0)
uglygreen = (107,142,35)
blue = (0,26,255)
good = (0, 255, 0)
bad = (255, 0, 0)

#sizes
scrsz = (800,600) #screen size
cw = 60 #camera button width
ch = 45 #camera button height

#screen sub-locations
starttopleftx = 10
starttoplefty = 10
starttoprightx = 400
starttoprighty = 10
startbottomrightx = 400
startbottomrighty = 180
startcx = 85
startcy = 135
xpadding = 10
ypadding = 10

true = 1
false = 0

freq = 10 #update rate (Hz)
deadband = 3 #joystick deadband around center

address = "192.168.1.100"
port = 5000

raspberry_pi = UDP_Library( address, port)

joy1 = joystick_Library(0)
joy2 = joystick_Library(1)

joy1.start_Joystick()
joy2.start_Joystick()

try:
    verify = int(joy1.numAxes)

    if verify != 3:
        joy1 = joystick_Library(1)
        joy2 = joystick_Library(0)

        joy1.start_Joystick()
        joy2.start_Joystick()
except:
    print("\n..\nJoysticks not Initialized Properly\n..\n")

class Bott_Stuff:
    def __init__(self):
        self.main()
    
    #Create a display
    def display(self):
        self.screen = pygame.display.set_mode(scrsz,0,32)
        icon = pygame.image.load("robotics logo.png")
        pygame.display.set_icon(icon)
        pygame.display.set_caption("NIU Robotics ROV")
        GUI_start_time = time.clock()
        Joystick_send_time = time.clock()

#
#
#Update the display and show the buttons
#
#
    def update_display(self):
        self.topleftx = starttopleftx
        self.toplefty = starttoplefty
        self.toprightx = starttoprightx
        self.toprighty = starttoprighty
        self.bottomrightx = startbottomrightx
        self.bottomrighty = startbottomrighty
        self.cx = startcx
        self.cy = startcy

        #set background color
        self.screen.fill((30, 144, 255))

        #Parameters:               surface,      color,       x,   y,   length, height, width,    text,      text_color
    # TOP LEFT DISPLAY SECTION
        self.IMUCal.create_button(self.screen, uglygreen, self.topleftx, self.toplefty, 50, 35, 0, "IMU Cal", white, 18)
        self.PIDAdj.create_button(self.screen, uglygreen, self.TOPLEFTX(50), self.toplefty+35+ypadding, 50, 35, 0, "Adjust", white, 18)
        self.DepthAuto.create_button(self.screen, blue if self.depthAuto else uglygreen, self.topleftx, self.TOPLEFTY(20),50,20,0,"Auto",white, 18)
        self.DepthPIDStat.create_button(self.screen, blue if self.depthPIDon else uglygreen, self.topleftx+60,starttoplefty, 100, 20, 0,"Depth " + ("Enabled" if self.depthPIDon == 1 else "Disabled"), white, 18)
        self.TiltAuto.create_button(self.screen, blue if self.tiltAuto else uglygreen, self.topleftx, self.TOPLEFTY(20),50,20,0,"Auto",white)
        self.TiltPIDStat.create_button(self.screen, blue if self.tiltPIDon else uglygreen, self.topleftx+60,starttoplefty + 30, 100, 20, 0,"Tilt " + ("Enabled" if self.tiltPIDon == 1 else "Disabled"), white, 18)
        self.YawAuto.create_button(self.screen, blue if self.yawAuto else uglygreen, self.topleftx, self.TOPLEFTY(20),50, 20, 0, "Auto", white)
        self.YawPIDStat.create_button(self.screen, blue if self.yawPIDon else uglygreen, self.topleftx+60, starttoplefty+60, 100, 20, 0, "Yaw " + ("Enabled" if self.yawPIDon == 1 else "Disabled"), white, 18)

    # CAM BUTTONS
        self.write_text(self.screen, "Cameras", white, 80, 105, 90, 30, 18)
        self.write_text(self.screen, "Screen 1", white, 0, 135, 90, 30, 18)
        self.write_text(self.screen, "Screen 2", white, 0, 175, 90, 30, 18)
        self.Feed1C1.create_button(self.screen, blue if self.cam1 == 1 else uglygreen, self.CX(), self.cy, cw, ch, 0, c1n, white,18)
        self.Feed1C2.create_button(self.screen, blue if self.cam1 == 2 else uglygreen, self.CX(), self.cy, cw, ch, 0, c2n, white,18)
        self.Feed1C3.create_button(self.screen, blue if self.cam1 == 3 else uglygreen, self.CX(), self.cy, cw, ch, 0, c3n, white,18)
        self.Feed1C4.create_button(self.screen, blue if self.cam1 == 4 else uglygreen, self.CX(), self.cy, cw, ch, 0, c4n, white,18)
        self.CNL() # new camera button line
        self.Feed2C1.create_button(self.screen, blue if self.cam2 == 1 else uglygreen, self.CX(), self.cy, cw, ch, 0, c1n, white,18)
        self.Feed2C2.create_button(self.screen, blue if self.cam2 == 2 else uglygreen, self.CX(), self.cy, cw, ch, 0, c2n, white,18)
        self.Feed2C3.create_button(self.screen, blue if self.cam2 == 3 else uglygreen, self.CX(), self.cy, cw, ch, 0, c3n, white,18)
        self.Feed2C4.create_button(self.screen, blue if self.cam2 == 4 else uglygreen, self.CX(), self.cy, cw, ch, 0, c4n, white,18)

    # TOP RIGHT DISPLAY SECTION
        self.TempRecord.create_button(self.screen, uglygreen, starttoprightx - 50, starttoprighty, 30, 30, 0, "Record",white,18)
        self.TempUnits.create_button(self.screen, uglygreen, self.toprightx, self.toprighty,   80,  30,     0, "Degrees ("+self.tempUnits+"):",  white, 18)
        self.Temperature()

        self.DepthUnits.create_button(self.screen, uglygreen, self.toprightx, self.toprighty,  80,  30,     0, "Depth (" + self.depthUnits + "):", white, 18)
        self.Depth()
        self.DepthCal.create_button(self.screen, uglygreen, self.toprightx + 150, self.TOPRIGHTY(30), 45, 30, 0, "Depth Cal", white, 18)
        self.PressUnits.create_button(self.screen, uglygreen, self.toprightx, self.toprighty,  80,  30,     0, "Pressure(" + self.pressUnits + "):", white, 18)
        self.Pressure()

    # BOTTOM RIGHT DISPLAY SECTION
        #get status dependent color adjustment
        self.colorchange = (self.regulator1+self.regulator2-60)*1.4+((self.currentDraw-2000)/21000)*170 #21 amps max
        self.colorchange += 255*self.alarmTripped
        if self.colorchange < 0: self.colorchange = 0   #limit
        if self.colorchange > 255: self.colorchange = 255#limit
        self.ccolorchange = (self.colorchange, -1*self.colorchange, 0)  #corrected color change

        #draw system monitor button
        self.SysData.create_button(self.screen, tuple(map(operator.add,good,self.ccolorchange)), self.bottomrightx, self.bottomrighty, 160, 180, 0, " ", white, 18)
        self.SystemData()
        self.AlarmReset.create_button(self.screen, red if self.alarmTripped else uglygreen, self.bottomrightx+20, self.BOTRIGHTY(45), 80, 30, 0, "RESET ALARMS", white, 10)
        
        #draw surface safety override button
        self.SurfaceOverride.create_button(self.screen, bad if self.surfaceOverride else good, startbottomrightx + 50,startbottomrighty - 30, 60, 20, 0, "OVERRIDE", white, 18)

    #write alerts at bottom of screen
        self.write_text(self.screen, self.Alert, bad, self.bottomrightx+25, self.bottomrighty, 60, 20, 25)
        
    # BOTTOM LEFT DISPLAY SECTION
        self.AttitudeIndicator()
        self.Bot_Placeholder()

    # PID ADJUST PAGE
        if self.adjust == "open":
            self.topleftx = starttopleftx + 55
            self.toplefty = starttoplefty - 3
            #spinPD = spinBox.make((self.TOPLEFTX(48),self.toplefty),0.001)
            spinPD = spinBox.make(self.spinPD, (self.TOPLEFTX(48),self.toplefty))
            self.spinPD.draw(self.screen)
            spinID = spinBox.make(self.spinID,(self.TOPLEFTX(48),self.toplefty))
            self.spinID.draw(self.screen)
            spinDD = spinBox.make(self.spinDD,(self.TOPLEFTX(48),self.toplefty))
            self.spinDD.draw(self.screen)
            self.write_text(self.screen,"Depth",white,self.topleftx,self.toplefty,50,30)

            self.topleftx = starttopleftx + 55
            self.TOPLEFTY(20)
            spinPT = spinBox.make(self.spinPT,(self.TOPLEFTX(48),self.toplefty))
            self.spinPT.draw(self.screen)
            spinIT = spinBox.make(self.spinIT,(self.TOPLEFTX(48),self.toplefty))
            self.spinIT.draw(self.screen)
            spinDT = spinBox.make(self.spinDT,(self.TOPLEFTX(48),self.toplefty))
            self.spinDT.draw(self.screen)
            self.write_text(self.screen, "Tilt", white, self.topleftx, self.toplefty, 50, 30)

            self.topleftx = starttopleftx + 55
            self.TOPLEFTY(20)
            spinPY = spinBox.make(self.spinPY,(self.TOPLEFTX(48),self.toplefty))
            self.spinPY.draw(self.screen)
            spinIY = spinBox.make(self.spinIY,(self.TOPLEFTX(48),self.toplefty))
            self.spinIY.draw(self.screen)
            spinDY = spinBox.make(self.spinDY,(self.TOPLEFTX(48),self.toplefty))
            self.spinDY.draw(self.screen)
            self.write_text(self.screen, "Yaw", white, self.topleftx, self.toplefty, 50, 30)
            self.TOPLEFTY(20)

            self.topleftx = starttopleftx +50
            self.write_text(self.screen,"P",white,self.TOPLEFTX(48),self.toplefty,58,15)
            self.write_text(self.screen,"I",white,self.TOPLEFTX(48),self.toplefty,58,15)
            self.write_text(self.screen, "D", white,self.TOPLEFTX(48), self.toplefty,58,15)


    # DIRECT ENTRY FORM
        self.TxTextEntry.create_button(self.screen, white, 180,340,180,20,0," ",white)
        self.write_text(self.screen, ">>"+self.message, black, 170, 325, 200, 50, 15)

    #DIRECT DISPLAY FORM
        self.RxText.create_button(self.screen, white, 180, 370, 180, 20, 0, " ", white)
        self.write_text(self.screen, "<<"+self.rxmessage, black, 170, 325, 200, 110, 15)

        pygame.display.flip()
#
#
#END UPDATE_DISPLAY
#
#


    #METHOD DEFINITIONS
    def write_text(self, surface, text, text_color, x, y, length, height, fontsize=0):
        if fontsize!=0:
            font_size = fontsize
        else:
            font_size = int(length//len(text)) if len(text) >8 else 14
        myFont = pygame.font.SysFont("Calibri", font_size)
        myText = myFont.render(text, 1, text_color)
        surface.blit(myText, ((x+length/2) - myText.get_width()/2, (y+height/2) - myText.get_height()/2))
        return surface

    #external temperature display
    def Temperature(self):
        if self.tempUnits == 'F':
            temp = self.ConvTemp(self.temp)
        else:
            temp = round(self.temp,1)
        self.write_text(self.screen, str(temp), white, self.toprightx+100, self.TOPRIGHTY(30), 40, 30)


    def ConvTemp(self,temp):
        if self.tempUnits=="F":
            return round(temp * (9/5) +32, 1)
        elif self.tempUnits=="C":
            return round(temp,1)

    #depth display
    def Depth(self):
        # convert mbar to m H2O
        self.depth = (self.pressure - self.zeroDepthPressure) * 10.1972 / 1000
        if self.depthUnits == 'm':
            d = round(self.depth,2)
        else:
            #convert m to ft
            d = round(self.depth * 3.28084,2)
        self.write_text(self.screen, str(d), white, self.toprightx+100, self.toprighty, 40, 30, 12)

    #pressure display
    def Pressure(self):
        #convert mbar to kPa
        p = self.pressure * 0.1
        if self.pressUnits == 'kPa':
            p = round(p,2)
        else:
            #convert kPa to psi
            p = round(p * 0.145038, 2)
        self.write_text(self.screen, str(p), white, self.toprightx + 100, self.TOPRIGHTY(30), 40, 30, 12)

    #assorted labels
    def OtherTexts(self):
        self.write_text(self.screen, "Cameras", white, 80, 105, 90, 30)
        self.write_text(self.screen, "Screen 1", white, 0, 135, 90, 30)
        self.write_text(self.screen, "Screen 2", white, 0, 175, 90, 30)

    #system data display
    def SystemData(self):
        self.write_text(self.screen, "System Monitor", white, self.bottomrightx-20, self.BOTRIGHTY(15), 200, 30)
        self.write_text(self.screen, "Regulator Temp", white, self.BOTRIGHTX(10), self.BOTRIGHTY(10), 160, 20)
        self.write_text(self.screen, "1("+self.tempUnits+"):", white, self.bottomrightx, self.bottomrighty, 80, 20)
        self.write_text(self.screen, str(self.ConvTemp(self.regulator1)), white, self.bottomrightx+60, self.BOTRIGHTY(10), 50, 20)
        self.write_text(self.screen, "2("+self.tempUnits+"):", white, self.bottomrightx, self.bottomrighty, 80, 20)
        self.write_text(self.screen, str(self.ConvTemp(self.regulator2)), white, self.bottomrightx+60, self.BOTRIGHTY(10), 50, 20)
        self.write_text(self.screen, "Current Draw:", white, self.bottomrightx-50, self.bottomrighty, 160, 20)
        self.write_text(self.screen, str(round(self.currentDraw/1000,1))+" A", white, self.bottomrightx+60, self.BOTRIGHTY(15), 80, 20)
        self.write_text(self.screen, "Comm refresh:", white, self.bottomrightx - 50, self.bottomrighty, 160, 20)
        self.write_text(self.screen, str(round(self.timeout,1)) + " s", white, self.bottomrightx +60, self.BOTRIGHTY(20), 80, 20)

    #attitude indicator
    def AttitudeIndicator(self):
        self.write_text(self.screen, "Horizon Placeholder", red, 60, 150, 100, 200, 18)

    #bot display
    def Bot_Placeholder(self):
        self.write_text(self.screen, "Bot Placeholder", red, 250, 150, 100, 200, 18)
        self.write_text(self.screen, "Arm 1", red, 175, 275, 100, 100)
        
        if self.arm1 == True:
            self.write_text(self.screen, "out", red, 250, 300, 50, 50)
        if self.arm1 == False:
            self.write_text(self.screen, "IN", red, 250, 300, 50, 50)

        self.write_text(self.screen, "Arm 2", red, 275, 275, 100, 100)
        if self.arm2 == True:
            self.write_text(self.screen, "out", red, 350, 300, 50, 50)
        if self.arm2 == False:
            self.write_text(self.screen, "IN", red, 350, 300, 50, 50)

    #These functions increment object positions
    #They return the position for the current button, text, etc. and increment
    #the position variable for the next object in line, with padding
    #USE: Pass the height or width of the current object as the parameter
    def TOPRIGHTX(self,width):
        t = self.toprightx
        global xpadding
        self.toprightx += width + xpadding
        return t

    def TOPRIGHTY(self,height):
        t = self.toprighty
        global ypadding
        self.toprighty += height + ypadding
        return t

    def BOTRIGHTX(self,width):
        t = self.bottomrightx
        global xpadding
        self.bottomrightx += width + xpadding
        return t

    def BOTRIGHTY(self,height):
        t = self.bottomrighty
        global ypadding
        self.bottomrighty += height + ypadding
        return t

    def TOPLEFTX(self,width):
        t = self.topleftx
        global xpadding
        self.topleftx += width + xpadding
        return t

    def TOPLEFTY(self,height):
        t = self.toplefty
        global ypadding
        self.toplefty += height + ypadding
        return t

    def CX(self):
        t = self.cx
        global xpadding, cw
        self.cx += cw + xpadding
        return t

    def CY(self):
        t = self.cy
        global ypadding, ch
        self.cy += ch + ypadding
        return t

    def CNL(self):
        global startcx
        self.cx = startcx
        self.CY()

    #Run the loop
    def main(self):

        #CREATE ALL THE OBJECTS, then in update_display() we will assign text, positions, color, etc.

        #TOP RIGHT
        self.TempUnits = Buttons.Button()
        self.TempRecord = Buttons.Button()
        self.DepthUnits = Buttons.Button()
        self.PressUnits = Buttons.Button()
        self.SurfaceOverride = Buttons.Button()
        self.DepthCal = Buttons.Button()

        #TOP LEFT
        self.IMUCal = Buttons.Button()
        self.PIDAdj = Buttons.Button()
        self.DepthAuto = Buttons.Button()
        self.TiltAuto = Buttons.Button()
        self.YawAuto = Buttons.Button()
        self.DepthPIDStat = Buttons.Button()
        self.TiltPIDStat = Buttons.Button()
        self.YawPIDStat = Buttons.Button()
        self.spinPD = spinbox.spinBox(2)
        self.spinID = spinbox.spinBox(.001)
        self.spinDD = spinbox.spinBox(.001)
        self.spinPT = spinbox.spinBox(2)
        self.spinIT = spinbox.spinBox(.001)
        self.spinDT = spinbox.spinBox(.001)
        self.spinPY = spinbox.spinBox(1000)
        self.spinIY = spinbox.spinBox(.001)
        self.spinDY = spinbox.spinBox(.001)

        #BOTTOM RIGHT
        self.SysData = Buttons.Button()
        self.SysData = Buttons.Button()
        self.AlarmReset = Buttons.Button()

        #BOTTOM LEFT
        self.TxTextEntry = Buttons.Button()
        self.RxText = Buttons.Button()

        #CAMERA BUTTONS
        self.Feed1C1 = Buttons.Button()
        self.Feed1C2 = Buttons.Button()
        self.Feed1C3 = Buttons.Button()
        self.Feed1C4 = Buttons.Button()
        self.Feed2C1 = Buttons.Button()
        self.Feed2C2 = Buttons.Button()
        self.Feed2C3 = Buttons.Button()
        self.Feed2C4 = Buttons.Button()

        #timers
        self.ROVtimer = 0.0
        self.PID_send_time = 0

        #STATUS VARIABLES
        self.temp = 100
        self.depth = 0.0
        self.zeroDepthPressure = 986.92 #mbar at atmospheric
        self.pressure = 1200  #mbar reported
        self.regulator1 = 25
        self.regulator2 = 25
        self.currentDraw = 2000
        self.Alert = "Clear Alarms to Operate"

        self.alarmTripped = True
        self.atSurface = True
        self.surfaceOverride = False
        self.underCurrent = 0
        self.overCurrent = 0
        self.timeout = 0

        self.gyro = ( 10, 20, 30 )

        self.arm1 = True
        self.arm2 = True

        self.pumpOn = False

        self.cam1 = 1 # update these from feedback, use to change color of buttons
        self.cam2 = 2

        self.adjust = "closed"
        self.depthPIDon = False
        self.depthAuto = False
        self.tiltPIDon = False
        self.tiltAuto = False
        self.yawPIDon = False
        self.yawAuto = False
        self.PID_rotator = 0

        self.message = ""
        self.rxmessage = ""
        self.ROV_inString = ""

        self.GUI_start_time = 0
        self.Joystick_send_time = 0
        self.start_record_time = 0
        self.depthUnits = 'ft'
        self.tempUnits = 'F'
        self.pressUnits = 'psi'
        self.tempReadout = True

        #UPDATE THE DISPLAY WITH BUTTONS AND TEXT, ETC.
        self.display()

        while True:
            # MANAGE GUI INPUTS
            self.update_display()
            self.ROV_inString = raspberry_pi.receiveUDP()
            
            while self.ROV_inString != 0 and len(self.ROV_inString):
                #print("S:" + self.ROV_inString)
                if self.ROV_inString[0]+self.ROV_inString[1] == "Dp":
                    for e in range(len(self.ROV_inString),2,-1):
                        try:
                            self.spinPD.setState(int(self.ROV_inString[2:e])/1000.0)
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] + self.ROV_inString[1] == "Di":
                    for e in range(len(self.ROV_inString), 2,-1):
                        try:
                            self.spinID.setState(int(self.ROV_inString[2:e])/1000.0)
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] + self.ROV_inString[1] == "Dd":
                    for e in range(len(self.ROV_inString), 2,-1):
                        try:
                            self.spinDD.setState(int(self.ROV_inString[2:e])/1000.0)
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] + self.ROV_inString[1] == "Tp":
                    for e in range(len(self.ROV_inString), 2,-1):
                        try:
                            self.spinPT.setState(int(self.ROV_inString[2:e])/1000.0)
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] + self.ROV_inString[1] == "Ti":
                    for e in range(len(self.ROV_inString), 2,-1):
                        try:
                            self.spinIT.setState(int(self.ROV_inString[2:e])/1000.0)
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] + self.ROV_inString[1] == "Td":
                    for e in range(len(self.ROV_inString), 2,-1):
                        try:
                            self.spinDT.setState(int(self.ROV_inString[2:e])/1000.0)
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] + self.ROV_inString[1] == "Yp":
                    for e in range(len(self.ROV_inString), 2,-1):
                        try:
                            self.spinPY.setState(int(self.ROV_inString[2:e])/1000.0)
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] + self.ROV_inString[1] == "Yi":
                    for e in range(len(self.ROV_inString), 2,-1):
                        try:
                            self.spinIY.setState(int(self.ROV_inString[2:e])/1000.0)
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] + self.ROV_inString[1] == "Yd":
                    for e in range(len(self.ROV_inString), 2,-1):
                        try:
                            self.spinDY.setState(int(self.ROV_inString[2:e])/1000.0)
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass

                elif self.ROV_inString[0] == 'X':
                    for e in range(len(self.ROV_inString), 1,-1):
                        try:
                            self.gyro = ( int(self.ROV_inString[1:e]), self.gyro[1], self.gyro[2] )
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] == 'Y':
                    for e in range(len(self.ROV_inString), 1,-1):
                        try:
                            self.gyro = (self.gyro[0], int(self.ROV_inString[1:e]), self.gyro[2] )
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] == 'Z':
                    for e in range(len(self.ROV_inString), 1,-1):
                        try:
                            self.gyro = ( self.gyro[0], self.gyro[1], int(self.ROV_inString[1:e]))
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] == 'T':
                    for e in range(len(self.ROV_inString), 1,-1):
                        try:
                            self.regulator1 = int(self.ROV_inString[1:e])/1000.0
                            self.ROV_inString = self.ROV_inString[e:]
                            #print("S:" + self.ROV_inString)
                            if self.ROV_inString[0] == ",":
                                for e2 in range(len(self.ROV_inString), 1, -1):
                                    try:
                                        self.regulator2 = int(self.ROV_inString[1:e2])/1000.0
                                        self.ROV_inString = self.ROV_inString[e2:]
                                        break
                                    except:
                                        pass
                            break
                        except:
                            pass
                elif self.ROV_inString[0] == 'P':
                    try:
                        self.pumpOn = int(self.ROV_inString[i + 1])
                        self.ROV_inString = self.ROV_inString[1:]
                    except:
                        pass
                elif self.ROV_inString[0] == 't':
                    for e in range(len(self.ROV_inString), 1, -1):
                        try:
                            self.temp = int(self.ROV_inString[1:e])/1000
                            self.ROV_inString = self.ROV_inString[e:]
                            #print("S:" + self.ROV_inString)
                            if self.ROV_inString[0] == ",":
                                for e2 in range(len(self.ROV_inString),1,-1):
                                    try:
                                        self.tempTime = int(self.ROV_inString[1:e2])
                                        self.ROV_inString = self.ROV_inString[e2:]
                                        #print("S:" + self.ROV_inString)
                                        break
                                    except:
                                        pass
                            break
                        except:
                            pass
                elif self.ROV_inString[0] == 'd':
                    for e in range(len(self.ROV_inString), 0, -1):
                        try:
                            self.pressure = int(self.ROV_inString[1:e])/1000.0
                            self.ROV_inString = self.ROV_inString[e:]
                        except:
                            pass
                elif self.ROV_inString[0] == 'C':
                    try:
                        self.cam1 = int(self.ROV_inString[1])
                        self.ROV_inString = self.ROV_inString[2:]
                        #print("S:" + self.ROV_inString)
                        if self.ROV_inString[0] == ",":
                            try:
                                self.cam2 = int(self.ROV_inString[1])
                                self.ROV_inString = self.ROV_inString[2:]
                                #print("S:"+self.ROV_inString)
                            except:
                                pass
                    except:
                        pass
                elif self.ROV_inString[0] == 'M':
                    try:
                        self.overCurrent = int(self.ROV_inString[1])
                        self.ROV_inString = self.ROV_inString[2:]
                    except:
                        pass
                elif self.ROV_inString[0] == 'm':
                    try:
                        self.underCurrent = int(self.ROV_inString[1])
                        self.ROV_inString = self.ROV_inString[2:]
                    except:
                        pass
                elif self.ROV_inString[0] == "c":
                    for e in range(len(self.ROV_inString), 0, -1):
                        try:
                            self.currentDraw = int(self.ROV_inString[1:e])/1000.0
                            self.ROV_inString = self.ROV_inString[e:]
                            break
                        except:
                            pass
                elif self.ROV_inString[0] == "!":
                    self.rxmessage = self.ROV_inString[1:]
                elif self.ROV_inString[0] == ",":
                    self.ROV_inString = self.ROV_inString[1:]
                else:
                    self.ROV_inString = self.ROV_inString[1:]
                self.ROVtimer = time.clock()

            # TODO: do safety checks
            #   monitor temperature

            #   monitor current
            if self.currentDraw > 18000:
                self.alarmTripped = 1
                print("TOTAL CURRENT DRAW")
                self.Alert = "TOTAL CURRENT DRAW"
                #print("TOTAL CURRENT DRAW")
            if self.underCurrent != 0 or self.overCurrent !=0:
                self.alarmTripped = 1
                print("THRUSTER OVERCURRENT")
                self.Alert = "THRUSTER OVERCURRENT"
                #print("THRUSTER OVERCURRENT")

            #   monitor timeout
            self.timeout = time.clock() - self.ROVtimer
            if self.timeout > 3000:
                self.alarmTripped = 1
                #TODO: send stop signal
                print("ROV TIMEOUT")
                self.Alert ="ROV TIMEOUT"
                #print("ROV TIMEOUT")

            #monitor depth
            if (self.zeroDepthPressure - self.pressure) > -10:
                if (self.zeroDepthPressure - self.pressure) > 10:
                    #TODO: we are out of water
                    print("ROV out of water")
                    self.Alert ="ROV out of water"
                    #print("ROV out of water")
                    if not self.surfaceOverride:
                        raspberry_pi.sendUDP("S")
                        print("Shutting down motion")
                        self.alarmTripped = 1
                        #print("Shutting down motion")
                else:
                    #TODO: we are at surface
                    print("ROV at surface")
                    self.Alert ="ROV at surface"
                    #print("ROV at surface")
                    if not self.surfaceOverride:
                        raspberry_pi.sendUDP("s")
                        print ("Shutting down top thrusters")
                        #print ("Shutting down top thrusters")

            if self.alarmTripped == 0:
                self.Alert = ""

            #watch GUI
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                elif event.type == MOUSEBUTTONDOWN:
                    mpos = pygame.mouse.get_pos() #mouse position
                    if self.TempUnits.pressed(mpos):  #change temperature display units
                        if self.tempUnits == 'F':
                            self.tempUnits = 'C'
                        elif self.tempUnits =='C':
                            self.tempUnits = 'F'
                        print("Temp in " + self.tempUnits)
                    elif self.PIDAdj.pressed(mpos):   #open/close PID page
                        if self.adjust == "open":
                            raspberry_pi.sendUDP("%")
                            self.adjust = "closed"
                        elif self.adjust == "closed":
                            self.adjust = "open"
                        print("PID adjust "+self.adjust)
                    elif self.IMUCal.pressed(mpos):     #IMU calibrate
                        raspberry_pi.sendUDP("&")
                        print("IMU Calibrating")
                    elif self.TempRecord.pressed(mpos):  #record
                        self.start_record_time = time.clock()
                        print(round(self.start_record_time - self.GUI_start_time,3))
                        print("record")
                    elif self.DepthUnits.pressed(mpos):  #change depth units
                        if self.depthUnits == "ft":
                            self.depthUnits = "m"
                        elif self.depthUnits == "m":
                            self.depthUnits = "ft"
                        print("Depth in " + self.depthUnits)
                    elif self.DepthCal.pressed(mpos):   #calibrate depth pressure at surface
                        self.zeroDepthPressure = self.pressure
                        print("Surface pressure calibrated at " + str(round(self.pressure,2)) + " mbar")
                    elif self.PressUnits.pressed(mpos):    #change pressure units
                        if self.pressUnits == 'psi':
                            self.pressUnits = 'kPa'
                        elif self.pressUnits == 'kPa':
                            self.pressUnits = 'psi'
                        print ("Pressure in " + self.pressUnits)
                    elif self.Feed1C1.pressed(mpos):  #feed 1 camera 1
                        raspberry_pi.sendUDP("C1")
                        print("Feed 1 "+c1n)
                    elif self.Feed1C2.pressed(mpos):  # feed 1 camera 2
                        raspberry_pi.sendUDP("C2")
                        print("Feed 1 " + c2n)
                    elif self.Feed1C3.pressed(mpos):  # feed 1 camera 3
                        raspberry_pi.sendUDP("C3")
                        print("Feed 1 " + c3n)
                    elif self.Feed1C4.pressed(mpos):  # feed 1 camera 4
                        raspberry_pi.sendUDP("C4")
                        print("Feed 1 " + c4n)
                    elif self.Feed2C1.pressed(mpos):  # feed 2 camera 1
                        raspberry_pi.sendUDP("c1")
                        print("Feed 2 " + c1n)
                    elif self.Feed2C2.pressed(mpos):  # feed 2 camera 2
                        raspberry_pi.sendUDP("c2")
                        print("Feed 2 " + c2n)
                    elif self.Feed2C3.pressed(mpos):  # feed 2 camera 3
                        raspberry_pi.sendUDP("c3")
                        print("Feed 2 " + c3n)
                    elif self.Feed2C4.pressed(mpos):  # feed 2 camera 4
                        raspberry_pi.sendUDP("c4")
                        print("Feed 2 " + c4n)
                    elif self.TxTextEntry.pressed(mpos):  #direct command entry
                        if pygame.mouse.get_pressed()[0]:   #left click sends
                            if not self.message == "":  # not blank
                                print("Sending:: " + self.message)
                                raspberry_pi.sendUDP(self.message)
                            else:
                                print("No message to send")
                        elif pygame.mouse.get_pressed()[2]: #right click clears
                            self.message = ""
                    elif self.RxText.pressed(mpos):  # direct message printing
                        if pygame.mouse.get_pressed()[0]:  # left click clears
                            self.rxmessage = ""
                    elif self.AlarmReset.pressed(mpos): #reset alarms button
                        if pygame.mouse.get_pressed()[2]: #right click to clear
                            #TODO: do things
                            #TODO: send reset for whatever alarms are tripped
                            self.underCurrent = 0
                            self.overCurrent = 0
                            self.alarmTripped = False
                            raspberry_pi.sendUDP("M") #reset flags
                            print("reset alarms")
                    elif self.SurfaceOverride.pressed(mpos): #override button
                        self.surfaceOverride = not self.surfaceOverride
                        if self.surfaceOverride == True:
                            raspberry_pi.sendUDP("m")
                        print("Surface Override "+str(self.surfaceOverride))

                    if self.adjust=="closed":
                        #TODO: this section, status changes should come from ROV in
                        #response to commands sent
                        #TODO: auto on/off is control station operated, sending enable/disable
                        #commands dependant on controller deadbands and a short timeout for each
                        #PID type
                        if self.DepthPIDStat.pressed(mpos):  # depth pid
                            self.depthPIDon = not self.depthPIDon
                            raspberry_pi.sendUDP("O"+str(self.depthPIDon))
                            print("enable/disable depth pid")
                        elif self.DepthAuto.pressed(mpos):  # auto depth pid
                            self.depthAuto = not self.depthAuto
                            print("enable/disable auto depth")
                        elif self.TiltPIDStat.pressed(mpos):  # tilt pid
                            self.tiltPIDon = not self.tiltPIDon
                            raspberry_pi.sendUDP("O"+str(self.tiltPIDon))
                            print("enable/disable tilt pid")
                        elif self.TiltAuto.pressed(mpos):  # auto tilt pid
                            self.tiltAuto = not self.tiltAuto
                            print("enable/disable auto tilt")
                        elif self.YawPIDStat.pressed(mpos):  # yaw pid
                            self.yawPIDon = not self.yawPIDon
                            raspberry_pi.sendUDP("O"+str(self.yawPIDon))
                            print("enable/disable yaw pid")
                        elif self.YawAuto.pressed(mpos):  # auto yaw pid
                            self.yawAuto = not self.yawAuto
                            print("enable/disable auto yaw")
                    elif self.adjust=="open":
                        #watch button presses
                        self.spinPD(mpos)
                        self.spinID(mpos)
                        self.spinDD(mpos)

                        self.spinPT(mpos)
                        self.spinIT(mpos)
                        self.spinDT(mpos)

                        self.spinPY(mpos)
                        self.spinIY(mpos)
                        self.spinDY(mpos)

                elif event.type == KEYDOWN:
                    keys = pygame.key.get_pressed()
                    inkey = event.key
                    if keys[K_ESCAPE]:
                        if keys[K_RSHIFT] or keys[K_LSHIFT]:
                            #TODO: re-run initialization
                            #Reset to default config and things
                            print("reset")
                            break
                        else:
                            print("CLOSING")
                            time.sleep(.2)
                            pygame.quit()
                    #elif keys[K_RETURN]:
                        #if self.message != "": #not blank
                            #print("Sending:: " + self.message)
                            #self.ROV_inString = self.message
                            #raspberry_pi.sendUDP(self.message)
                        #else:
                            #print("No message to send")
                    elif keys[K_LCTRL] or keys[K_RCTRL]:
                        if keys[K_c]:
                            self.message = ""
                    elif keys[K_BACKSPACE]:
                        self.message = self.message[0:-1]
                    elif inkey <= 127:
                        if keys[K_RSHIFT] or keys[K_LSHIFT]:
                            if ord('a') <= inkey <= ord('z'):
                                self.message = self.message + chr(inkey).upper()
                            else:
                                symbol_lookup = {'1': '!', '2': '@', '3': '#', '4': '$', '5': '%', '6': '^', '7': '&', '8': '*', '9': '(', '0': ')','-':'_','=':'+','`':'~'}
                                if chr(inkey) in symbol_lookup:
                                    self.message = self.message + symbol_lookup[chr(inkey)]
                                else:
                                    print("Key not in dictionary")
                        elif set(chr(inkey)).issubset(set(string.printable)):
                            self.message = self.message + chr(inkey)

            #still inside if there is an event

        #still inside while loop
        #read joysticks and format joystick input
            if ((time.clock() - self.Joystick_send_time) > (1/(freq))):
                self.Joystick_send_time = time.clock()
                joy1_str = joy1.get_Joystick_Str()
                raspberry_pi.sendUDP(joy1_str)

                joy2_str = joy2.get_Controller_Str()
                raspberry_pi.sendUDP(joy2_str)
                #print(joy1_str+joy2_str)
                
        #still inside the while loop
        # TODO: Manage regular interval stuff
            if self.depthAuto:
               if joy1.get_joystick_db_time("z") > 1.0:
                   if not self.depthPIDon:
                        raspberry_pi.sendUDP("D1")
                        print("depth on")
               else:
                   if self.depthPIDon:
                        raspberry_pi.sendUDP("D0")
                        print("depth off")
            if self.tiltAuto:
               if (joy1.get_joystick_db_time("y") + joy1.get_joystick_db_time("x")) > 1.0:
                   if not self.tiltPIDon:
                        raspberry_pi.sendUDP("O1")
                        print("tilt on")
               else:
                   if self.tiltPIDon:
                        raspberry_pi.sendUDP("O0")
                        print("tilt off")
            if self.yawAuto:
               if joy2.get_joystick_db_time("w") > 0.8:
                   if not self.yawPIDon:
                        raspberry_pi.sendUDP("Y1")
                        print("yaw on")
               else:
                   if self.yawPIDon:
                        raspberry_pi.sendUDP("Y0")
                        print("yaw off")

            #TODO: send rotating PID values every ~80ms
            #combine and send two per message (or three if we upgrade ROV receiving function)
            if self.adjust == "open": #if we're adjusting PIDs
                if ((time.clock() - self.PID_send_time) > ( 0.8/9.0 ) ): #send each of 9 messages every 0.8 seconds
                    self.PID_send_time = time.clock()
                    if self.PID_rotator == 0:
                        raspberry_pi.sendUDP("Dp" + str(round( self.spinPD.getState() *1000)))
                    elif self.PID_rotator == 1:
                        raspberry_pi.sendUDP("Di" + str(round( self.spinID.getState() * 1000)))
                    elif self.PID_rotator == 2:
                        raspberry_pi.sendUDP("Dd" + str(round( self.spinDD.getState() * 1000)))
                    elif self.PID_rotator == 3:
                        raspberry_pi.sendUDP("Tp" + str(round( self.spinPT.getState() * 1000)))
                    elif self.PID_rotator == 4:
                        raspberry_pi.sendUDP("Ti" + str(round( self.spinIT.getState() * 1000)))
                    elif self.PID_rotator == 5:
                        raspberry_pi.sendUDP("Td" + str(round( self.spinDT.getState() * 1000)))
                    elif self.PID_rotator == 6:
                        raspberry_pi.sendUDP("Yp" + str(round( self.spinPY.getState() * 1000)))
                    elif self.PID_rotator == 7:
                        raspberry_pi.sendUDP("Yi" + str(round( self.spinIY.getState() * 1000)))
                    elif self.PID_rotator == 8:
                        raspberry_pi.sendUDP("Yd" + str(round( self.spinDY.getState() * 1000)))
                    self.PID_rotator = (self.PID_rotator + 1) % 9


if __name__ == '__main__':
    obj = Bott_Stuff()
