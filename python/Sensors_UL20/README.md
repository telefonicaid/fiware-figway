
To execute commands in this folder, please first make sure ../config.ini file is correct.
Particularly, note that all IDAS parameters including service, APIKEY, HOST and PORTS are up to date.

After the config.ini file is ready it is easy to proceed:

1) First discover and explore all available Device Models 
> python DiscoverModels.py 

The command above will list all available Device Models.

2) You can use then GetModel.py to explore a specific Model. For instance, the "SENSOR_TEMP" model.
> python GetModel.py SENSOR_TEMP

2.1) If you need to create a new model then use CreateModel.py and check out the model examples at python/SensorUL20/models folder.

3) Then register (in the FIWARE IoT Cloud) your own Devices:
> RegisterDevice [DEV_TYPE] [DEV_NAME] [ASSET_NAME]

Where DEV_TYPE has to be one of the available Models discovered in (1) and [DEV_NAME] is the name we wish to give to the device itself.
The script will return the [DEV_ID], which is the actual Device identifier in the FIWARE Cloud.
DEV_ID is calculated as follows: [DEV_ID] = [host_id]:[DEV_NAME], being [host_id] the one specified in the ../config.ini file.
ASSET_ID is calculated as follows: [DEV_ID] = [host_id]:[DEV_NAME], being [host_id] the one specified in the ../config.ini file.

In the ContextBroker your device will appear with Entity ID = [DEV_TYPE].[ASSET_ID] and Entity Type = "Device".

Examples:
> python RegisterDevice ZWAVE_4IN1 4in1-1
> python RegisterDevice ZWAVE_4IN1 4in1-2
> python RegisterDevice ZWAVE_4IN1 4in1-EricRoom
> python RegisterDevice ZWAVE_4IN1 4in1-MartinRoom
> python RegisterDevice TEMP Temp-Livingroom
> python RegisterDevice DIMMER Light-Livingroom

4) You can use DiscoverDevices.py to know all registered devices and know their exact DEV_ID
> python ListDevices.py


5) Send the masurements of you Devices. 
> python SendObservation [DEV_NAME] [MEASUREMENT]

If your Device accepts several measurements, include the ones ready following this template:
> python SendObservation [DEV_NAME] [ALIAS1]=[MEASUREMENT1]|[ALIAS2]=[MEASUREMENT2]|[ALIAS3]=[MEASUREMENT3]|...

Examples
> python SendObservation Temp-LivingRoom 't|25'
> python SendObservation 4in1-EricRoom 't|25#h|65#l|250'
> python SendObservation 4in1-EricRoom 't|27#m|MOVE'
> python SendObservation 4in1-EricRoom 't|27.3#m|QUIET' 

6) Later on you may read the status of a sensor (last measurements and their time ocurrence):

> python ReadDeviceStatus.py [DEV_ID]

Example
> python ReadDeviceStatus.py RPI:79:ed:af:Hum-Test


WORK IN PROGRESS
------------------

7) Finally, we may continuosily receive commands related to one device and forward them to our own python script handler.
> python RegisterPushCommand Light-EricRoom u=http://1.2.3.4:8080
> python ListenCommands  1.2.3.4 8080 Light-EricRoom.py

Where 1.2.3.4 and 8080 are the IP adress and the PORT to listen for commands and "Light-EricRoom.py" is our own python script to handle commands.
