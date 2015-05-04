#!/usr/bin/env python

# Copyright 2015 Telefonica Investigacion y Desarrollo, S.A.U
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
# Developed by Carlos Ralli Ucendo (@carlosralli), Apr 2015.


import requests, json
import ConfigParser
import io
import sys

CONFIG_FILE = "../config.ini"

NUM_ARG=len(sys.argv)
COMMAND=sys.argv[0] 
CB_IP='0.0.0.0'
CB_PORT='1026'

if NUM_ARG>=3:
   FIWARE_SERVICE=sys.argv[1]
   API_KEY=sys.argv[2]
else:
   print 'Usage: '+COMMAND+' [FIWARE_SERVICE] [API_KEY] [ContextBroker_IP] [ContextBroker_PORT]'
   print ' If you are using our ContextBroker IP = 0.0.0.0 and PORT = 1026, otherwise put your ContextBroker instance there'
   print
   sys.exit(2)

if NUM_ARG>=4:
   CB_IP=sys.argv[3]

if NUM_ARG>=5:
   CB_PORT=sys.argv[4]


# Load the configuration file
with open(CONFIG_FILE,'r+') as f:
    sample_config = f.read()
config = ConfigParser.RawConfigParser(allow_no_value=True)
config.readfp(io.BytesIO(sample_config))

IDAS_HOST=config.get('idas', 'host')
IDAS_ADMIN_PORT=config.get('idas', 'adminport')
IDAS_UL20_PORT=config.get('idas', 'ul20port')

FIWARE_SERVICE_PATH='/'

IDAS_AAA=config.get('idas', 'OAuth')
if IDAS_AAA == "yes":
   TOKEN=config.get('user', 'token')
   TOKEN_SHOW=TOKEN[1:5]+"**********************************************************************"+TOKEN[-5:]
else:
   TOKEN="NULL"
   TOKEN_SHOW="NULL"

HOST_ID=config.get('local', 'host_id')
f.close()

URL = "http://"+IDAS_HOST+":"+IDAS_ADMIN_PORT+'/iot/services'
PAYLOAD = '{   \
  "services": [  \
    {   \
      "apikey": "'+API_KEY+'", \
      "token": "token2", \
      "cbroker": "http://'+CB_IP+':'+CB_PORT+'", \
       "entity_type": "thing", \
      "resource": "/iot/d" \
    } \
  ] \
}'


HEADERS = {'content-type': 'application/json' , 'X-Auth-Token' : TOKEN, 'Fiware-Service' : FIWARE_SERVICE, 'Fiware-ServicePath' : FIWARE_SERVICE_PATH }
HEADERS_SHOW = {'content-type': 'application/json' , 'X-Auth-Token' : TOKEN_SHOW, 'Fiware-Service' : FIWARE_SERVICE, 'Fiware-ServicePath' : FIWARE_SERVICE_PATH}

print "* Asking to "+URL
print "* Headers: "+str(HEADERS_SHOW)
print "* Sending PAYLOAD: "
print json.dumps(json.loads(PAYLOAD), indent=4)
print
print "..."
r = requests.post(URL, data=PAYLOAD, headers=HEADERS)
print
print "* Status Code: "+str(r.status_code)
print "* Response: "
print r.text
print
