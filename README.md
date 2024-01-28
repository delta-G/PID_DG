# PID_DG

This is an improved PID library for Arduino.  The basic calculation is based on the PID library by Brett Beauregard.  Additional math for using real time deltas was taken from modifications to that library by @zhomeslice (forum.arduino.cc).

I encapsulated the controller into a class for the functions and a struct for the settings.  This library works in a very different way to the original and may not be quite as suited to beginners.  The library calculates the derivative and integrals based on actual time deltas.  It does not have a setting for a time interval.  Unlike the PID library where you would call compute as often as possible, with this version the caller is responsible for the timing.  With this version, compute should be called from a blink-without-delay style timer or from a timer interrupt.  


The class constructor must be called with a reference to a PID_Settings struct.  That struct holds the K values, the setpoint, direction, and output limits.  Define the members like this:

```
PID_Settings pidSettings = {
  .setpoint = 0,
  .Kp = 72.0,
  .Ki = 0.8,
  .Kd = 10.0,
  .outputMax = 5000,
  .outputMin = -5000,
  .direction = DIRECT
};
```

Then you can call the constructor to create the instance:

`PID_Class myPID(pidSettings);`

There are two methods to run the calculations. 

`double PID_Class::compute(double input)`  The compute method has changed.  You now pass the input as a parameter.  Additionally, it returns the output.  This is the biggest change from the old library. 

`void PID_Class::bumplessStart(double currentInput, double currentOutput, int skips)`  The bumpless start method allows for starting and stopping the controller.  Since the caller is responsible for the timing, there are no methods to enable or disable output.  If you have not called compute in a long time or have drastically changed settings, or if you are starting the controller in a non-idle state, then this function allows you to re-initialize the controller.  It takes arguments for the initial input and output values as well as a skips argument.  If skips is greater than zero, then the PID controller will revert to P only for that many iterations.  This allows for a bit of a soft start to the controller.  

There are no methods to set or check the settings.  Since they are in a public struct that belongs to the calling code, you can alter the values directly in the struct.  For instance to change the Kp value in the example struct above:

`pidSettings.Kp = someNewValue;`

The changes will be recognized on the next call of either compute or bumlessStart.  


