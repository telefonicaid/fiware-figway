Description
-------------

- figway is an open implementation of a very basic M2M gateway for RaspberryPI to talk to FI-WARE IoT backend.

- figway is a set of commands coded in C for the Raspbeery PI platform enabling your PI to become an M2M gateway connecting your
  favourite sensors/actuators to any M2M Platform providing an HTTP POST API based on SensorML standard (SML) or OMA NGSI9/10

- This package provides also "fizway_resgister" & "fizway" scripts to interconnect some specific z-wave sensors, assumming there is a
 razbian GPIO hardware module and the z-way server software installed in your PI.
 

How to start
-------------

0) You need a RaspberryPI with a Razbian card plugged in its GPIO interface & some z-wave sensors/actuators (see suported HW below).
   Do not forget to install the z-way server software as described in the Razbian card manual.

1) First you need to include your z-wave devices into your z-wave network by accessing the z-way server at http://[YOUR_PI]:8083
Do not forget to get the Device_ID numbers assigned to your sensors in the z-way server environment.

2) Edit "fizway_resgister" and "fizway" scripts to update the Device_ID number of your sensors.
   You may also need to uncomment lines/sections if you have more z-wave devices than the pre-configured pack:
     - a 4in1 multisensor (AEON), a 4-Button Key Fob (zwave.me) and a ON-OFF/DIMMER switch (Everspring).
 
   The software also supports Fibaro RGB Switch, Fibaro Power Mmanagement switch & Garage Sensor (detects horizontal/vertical)  
   However, adding new other devices should't be a hard task cheking existing ones and z-way server logs.

3) Edit the file SensorML/Register_SWITCH and update the line containing the callback URL:
  "<swe:value>http://1.0.0.1:9999</swe:value>"

   -> Instead of 1.0.0.1 put the RaspberryPI public IP address 
   -> Instead of 9999 set the port the switch daemon will be listening to (in fizway script configuration, normally 7777)
   
   Note: If you are in a NATed environment use the IP:Port of the WAN router instead and map that external IP:port in 
         the router to the right pair.

4) Check & Edit -if necessary- the figway "Config" file (normally, modifications aren't needed)
   A Config.example file is provided and comments should make this task really easy.

5) Register all your working sensors by executing "./fizway_register" 

6) Launch the script to interconnect your sensors to FI-WARE IoT Backend: "./fizway &"


figway commands details & configuration
----------------------------------------

- 3 commands are provided within figway package:

* registerDevice:    it enables you to register a device in a M2M platform (via SensorML or NGSI9/10).
                     you are expected to provide a DEVICE_ID and a DEVICE_TYPE.
                     devices need to be registered in the M2M platform prior to send/receive any data.

* addObservation:    once you collect an observation from a sensor you can store this in the RPI with this command.

* sendObservations:  once you have collected all observations from a device you can send all them in a single packet with this command.
                     It will generate an error if no observations are added or not all observation types have been updated.

Before executing these commands you may need to check and edit the figway "Config" file. 
An example "Config.example" file is provided.


fizway Supported Hardware (z-wave sensors/actuators)
-------------------------------------------------------

The following z-wave sensors are currently supported:
- AEON 4in1 Multisensor
- Key Fob (z-wave.me) 4-Buttons remote
- Fibaro ON/OFF Switch
- Fibaro Dimmer 
- Everspring Door/Window Sensor
- Garage Sensor

figway Extensibility
-----------------------

More sensors can be easily added by providing new SensorML (or NGSI).
Checking the existing template files is the best way to understand how to write new ones.


Using/Compiling fizway daemons
---------------------------------
- One device-type specific fizway daemon is launched for each actuator you wish to send commands to (currently only over SensorML,
in the future NGSI will be also supported to send commands to actuators).

- For your convenience, "fizway" script launches all daemons related to actuators configured in that file.

- Source code is provided, if you modify it you may use the following commands to recompile all:
  (change to the ./Sources directory, compile and copy the binaries back to its location)

->  Specific daemon for SWITCHES/DIMMERS. 
  The expected command from M2M platform is "FIZCOMMAND [VALUE] ", where [VALUE] is an integer from 0 to 255.
  In switches 0 = off and 255 = on while dimmers admit any integer within the range.
 
g++ -o fizway_switchd fizway_switchd.cpp    



Compiling figway commands
--------------------------

- Source code is provided, if you modify it you may use the following commands to recompile all:
  (change to the ./Sources directory, compile and copy the binaries back to its location)
 
 g++ -o registerDevice registerDevice.cpp  clientSocketHttp6-test.cpp clientSocketHttp6-test.h 
 g++ -o sendObservations sendObservations.cpp  clientSocketHttp6-test.cpp clientSocketHttp6-test.h
 g++ -o addObservation addObservation.cpp  


Credits
------------
- Developed by Carlos Ralli Ucendo (@carlosralli). Jul 2013.
- Thanks to Rafael Baños Lopez for his great support for the SML/NGSI template files & lots of debugging efforts!
