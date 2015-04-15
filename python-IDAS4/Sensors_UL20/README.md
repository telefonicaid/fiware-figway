
To execute commands in this folder, please first make sure ../config.ini file is correct.
Particularly, note that all IDAS parameters including Fiware service, Fiware path, API_KEY, HOST and PORTS are up to date.

After the config.ini file is ready it is easy to proceed:


0) Create an Iot Service (only for private IDAS instances where you are Admin).

Python CreateService.py [FIWARE_SERVICE]  [API_KEY]     

Example:
> Python CreateService OpenIoT.py 4jggokgpepnvsb2uv4s40d59ov


1) Register a Sensor or actuator:

SensorsUL20/> python RegisterDevice.py  [DEV_FILE] [DEV_ID] [ENTITY_ID]

Example:
> python RegisterDevice.py SENSOR_TEMP Sensor1 Temp-Madrid28001


Example of SENSOR_TEMP file:
{
 "devices": [
    { "device_id": "DEV_ID",
      "entity_name": "ENTITY_ID",
      "entity_type": "thing",
      "timezone": "Europe/Madrid",
"attributes": [
        { "object_id": "t",
          "name": "temperature",
          "type": "int"
        } ],
 "static_attributes": [
        { "name": "att_name",
          "type": "string",
          "value": "value"
        }
       ]
      }]} 


Example of actuator SWITCH file:
{
 "devices": [
    { "device_id": "DEV_ID",
      "entity_name": "ENTITY_ID",
      "entity_type": "thing",
      "timezone": "Europe/Madrid",
      "commands": [
        { "name": "RawCommand",
          "type": "command",
          "value": ”RawCommand|%s"
        } ],

"attributes": [
        { "object_id": "s",
          "name": "status",
          "type": "string"
        } ],
 "static_attributes": [
        { "name": "att_name",
          "type": "string",
          "value": "value"
        }]}]}


2) List Devices:
SensorsUL20/> python ListDevices.py    



3) Send observations of a sensor:

SensorsUL20/> python SendObservation.py  [DEV_ID] ‘[alias1|value1]’ 


Example:
> python SendObservation.py Sensor1 't|34'


4) read Observations (at the ContextBroker where ytour IDAS service is sending updates)

ContextBroker/> python GetEntity.py  [ENTITY_ID]  

Example:
> python GetEntity.py Temp-Madrid28001



5) Simulate a command to an actuator (the COntextBroker update is buggy at this time, but you can use this script even when it works for your tests).

python SimulateCommand.py [ENTITY_ID]  ‘[COMMAND]‘     

Example:
> python SimulateCommand.py dev_11 actuator


5) Get commands from your device:
python GetPoolingCommands.py  [DEV_ID]

Example:
> python GetPoolingCommands.py dev_11


