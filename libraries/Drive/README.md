# Drive Libraries

Expecting position and target_position objects.
Most, if not all of this should be re-written to the former design (not position-based).

Code compiles, but does not appear to function correctly.
Needs to be tested with working motors.
DriveMotor::update() may be the cause of issues (manually timed rather than setting a pwm frequency).

```C++
Position position;               // where am I?
Position_Target position_target; // where should I be?
Drive drive;
Gyro gyro1(0x00); // First I2C gyro
Gyro gyro2(0x01); // Second I2C gyro
Mouse mouse;
```

needs to have some sort of controllers written to determine power used for rotation and x/y movement
does not utilize mecanum wheels
