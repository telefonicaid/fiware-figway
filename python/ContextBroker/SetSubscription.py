# Copyright 2014 Telefonica Investigacion y Desarrollo, S.A.U
# 
# This file is part of FIGWAY software (a set of tools for FIWARE Orion ContextBroker and IDAS2.6).
#
# FIGWAY is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as 
# published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
# FIGWAY is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License along with FIGWARE. 
# If not, see http://www.gnu.org/licenses/
#
# For those usages not covered by the GNU Affero General Public License please contact with: Carlos Ralli Ucendo [ralli@tid.es] 
# Developed by Carlos Ralli Ucendo (@carlosralli), Nov 2014.

import requests, json, re 
from collections import Counter
import ConfigParser
import io
import sys

CONFIG_FILE = "../config.ini"

NUM_ARG=len(sys.argv)
COMMAND=sys.argv[0] 

if NUM_ARG==4:
   ENTITY_ID=sys.argv[1]
   ENTITY_ATTR=sys.argv[2]
   SERVER_URL=sys.argv[3]
else:
   print 'Usage: '+COMMAND+' [ENTITY ID] [ATTRIBUTE] [SERVER URL]'
   print '  ENTITY ID = Entity you want to watch for changes/updates.'
   print '  ATTRIBUTE = Attribute whose change will trigger notifications. In this example script only this attribute will be notified.'
   print '  SERVER URL = (Local) Server listening for notifications.Example: http://myserver.domain.com:10000' 
   print '    It has to be a reachable address:port for the ContextBroker. If you are behind a NAT/Firewall contact your network admin.'
   print
   sys.exit(2)


# Load the configuration file
with open(CONFIG_FILE,'r+') as f:
    sample_config = f.read()
config = ConfigParser.RawConfigParser(allow_no_value=True)
config.readfp(io.BytesIO(sample_config))

CB_HOST=config.get('contextbroker', 'host')
CB_PORT=config.get('contextbroker', 'port')
CB_FIWARE_SERVICE=config.get('contextbroker', 'fiware_service')
CB_AAA=config.get('contextbroker', 'OAuth')
if CB_AAA == "yes":
   TOKEN=config.get('user', 'token')
   TOKEN_SHOW=TOKEN[1:5]+"**********************************************************************"+TOKEN[-5:]
else:
   TOKEN="NULL"
   TOKEN_SHOW="NULL"

NODE_ID=config.get('local', 'host_id')
f.close()

CB_URL = "http://"+CB_HOST+":"+CB_PORT

MIN_INTERVAL = "PT5S"
DURATION = "P1M"
ENTITY_TYPE = ""
ENTITY_ATTR_WATCH = ENTITY_ATTR
ENTITY_ATTR_NOTIFY = ENTITY_ATTR

PAYLOAD = '{ \
    "entities": [ \
        { \
            "type": "'+ENTITY_TYPE+'", \
            "isPattern": "false", \
            "id": "'+ENTITY_ID+'" \
        } \
    ], \
    "attributes": [ \
        "'+ENTITY_ATTR_NOTIFY+'" \
    ], \
    "reference": "'+SERVER_URL+'", \
    "duration": "'+DURATION+'", \
    "notifyConditions": [ \
        { \
            "type": "ONCHANGE", \
            "condValues": [ \
                "'+ENTITY_ATTR_WATCH+'" \
            ] \
        } \
    ], \
    "throttling": "'+MIN_INTERVAL+'" \
}' 

HEADERS = {'content-type': 'application/json', 'accept': 'application/json' , 'Fiware-Service': CB_FIWARE_SERVICE ,'X-Auth-Token' : TOKEN}
#HEADERS = {'content-type': 'application/json', 'Fiware-Service': CB_FIWARE_SERVICE ,'X-Auth-Token' : TOKEN}
HEADERS_SHOW = {'content-type': 'application/json', 'accept': 'application/json' , 'Fiware-Service': CB_FIWARE_SERVICE , 'X-Auth-Token' : TOKEN_SHOW}
URL = CB_URL + '/v1/subscribeContext'

print "* Asking to "+URL
print "* Headers: "+str(HEADERS_SHOW)
print "* Sending PAYLOAD: "
print json.dumps(json.loads(PAYLOAD), indent=4)
print
print "..."
r = requests.post(URL, data=PAYLOAD, headers=HEADERS)
print
print "* Status Code: "+str(r.status_code)
print
print r.text
print
