# New Features added/developped by Easy Global Market, Nov 2015 abbas.ahmad@eglobalmark.com

To execute commands in this folder, please first make sure ../config.ini file is correct.

############################# IMPORTANT ###################################################
##																						 ##
##	This version uses pyhton 2.7. Python 3.x will not work.								 ##
##	The mandatory imports used are : 													 ##
##		- requests																		 ##
##		- json								 											 ##
##		- ConfigParser																	 ##
##		- io								 											 ##
##		- sys								 										     ##
##		- socket								 									     ##
##		- SocketServer								                                     ##
##		- time								 											 ##
##																						 ##
##  If you are missing one python module, let's take "requests" module fot example,		 ##
##  try the following command : pip install requests									 ##
## 								 														 ##
###########################################################################################
Particularly, note that all ContextBroker parameters including FIWARE service, HOST and PORT are up to date.


The scripts in this folder are a set of tools to interact with a FIWARE Orion ContextBroker. They are:

1. CheckVersion.py
A simple script to check connectivity to a ContextBroker.

2. CreateEntityWithOneAttribute.py  [ENTITY ID] [ENTITY TYPE] [ATTRIBUTE NAME] [ATTRIBUTE TYPE] [ATTRIBUTE VALUE]
Creates a new entity with ID, type and one attribute that has a name, type and value 

3. GetEntitiesByType.py [ENTITY TYPE]
It allows you to get a summary of all Entities of a specific ENTITY_TYPE (or all entities if ALL is provioded) in the specific FIWARE Service (if specified) or the general database (if no FIWARE Service specified).
At the end you will be prompted to print or not the full list of entities.
Known bug: the entities are printed in XML format and not JSON, this will be changed in a forthcomming version (as the code for summary calculation needs to be updated too).

4. GetEntityById.py [ENTITY_ID]
Gets the JSON representation of a specific entity.

5.  GetEntityAttribute.py [ENTITY_ID] [ATTRIBUTE NAME]
Gets the JSON representation of an attribute within a given entity.

6. UpdateEntityAttribute.py [ENTITY ID] [ENTITY TYPE] [ATTRIBUTE NAME] [ATTRIBUTE TYPE] [ATTRIBUTE VALUE]
Updates a specific attribute of a given entity. You may execute "GetEntityAttribute.py" before to get all parameters you need for this command.

7. DeleteEntityByIdAndType.py [ENTITY ID] [ENTITY TYPE]]
It allows you to delete an entity by it's ENTITY_ID and ENTITY_TYPE

8. SetSubscription.py [ENTITY ID] [ATTRIBUTE] [SERVER URL]
It configures an ONCHANGE subscription in the ContextBroker to be notified on chnages of the attribute [ATTRIBUTE] of a given entity [ENTITY_ID].
Before launching this command you should prepare a local server to handle incoming notifications at [SERVER URL] 
Such a server can be launched with the script "SubscriptionDaemon.py".

9. SubscriptionDaemon.py [PORT] [HANDLING_FILE_SCRIPT]
This script launches a local HTTP server at TCP port [PORT] to handle incoming notifications from a ContextBroker.
Everytime a JSON file is received is passed as an argument to [HANDLING_FILE_SCRIPT]. 
You can  always use "SubscriptionHandler.py" as an example of [HANDLING_FILE_SCRIPT] which will just print the output to the screen.

10. SubscriptionHandler.py 
Use this as example HANDLING_FILE_SCRIPT for "SubscriptionDaemon.py".

11. TestDaemon.py [PORT]
A simple client to test that your SubscriptionDaemon.py and handling script are working fine.






