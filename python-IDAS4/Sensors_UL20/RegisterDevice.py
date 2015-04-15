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

if NUM_ARG==4:
   DEVICE_FILE=sys.argv[1]
   DEVICE_ID=sys.argv[2]
   ENTITY_ID=sys.argv[3]
else:
   print 'Usage: '+COMMAND+' [DEVICE_FILE] [DEVICE_ID] [ENTITY_ID]'
   print '  Where DEVICE_FILE is the name of the file in the devices folder containing the JSON description of a device model. See examples at .devices/ '
   sys.exit(2)

# Load the configuration file
with open(CONFIG_FILE,'r+') as f:
    sample_config = f.read()
config = ConfigParser.RawConfigParser(allow_no_value=True)
config.readfp(io.BytesIO(sample_config))

IDAS_HOST=config.get('idas', 'host')
IDAS_ADMIN_PORT=config.get('idas', 'adminport')
IDAS_UL20_PORT=config.get('idas', 'ul20port')
FIWARE_SERVICE=config.get('idas', 'fiware-service')
FIWARE_SERVICE_PATH=config.get('idas', 'fiware-service-path')
IDAS_AAA=config.get('idas', 'OAuth')
if IDAS_AAA == "yes":
   TOKEN=config.get('user', 'token')
   TOKEN_SHOW=TOKEN[1:5]+"**********************************************************************"+TOKEN[-5:]
else:
   TOKEN="NULL"
   TOKEN_SHOW="NULL"

HOST_ID=config.get('local', 'host_id')
f.close()

URL = "http://"+IDAS_HOST+":"+IDAS_ADMIN_PORT+'/iot/devices'

FILE="./devices/"+DEVICE_FILE
print "* opening: "+FILE
with open(FILE) as json_file:
    PAYLOAD_json = json.load(json_file)

PAYLOAD_json["devices"][0]["device_id"]=DEVICE_ID
PAYLOAD_json["devices"][0]["entity_name"]=ENTITY_ID
    
PAYLOAD = json.dumps(PAYLOAD_json, indent=4)

HEADERS = {'content-type': 'application/json' , 'X-Auth-Token' : TOKEN, 'Fiware-Service' : FIWARE_SERVICE, 'Fiware-ServicePath' : FIWARE_SERVICE_PATH }
HEADERS_SHOW = {'content-type': 'application/json' , 'X-Auth-Token' : TOKEN_SHOW, 'Fiware-Service' : FIWARE_SERVICE, 'Fiware-ServicePath' : FIWARE_SERVICE_PATH}

print "* Asking to "+URL
print "* Headers: "+str(HEADERS_SHOW)
print "* Headers: "+str(HEADERS)
print "* Sending PAYLOAD: "
print PAYLOAD
print
print "..."
r = requests.post(URL, data=PAYLOAD, headers=HEADERS)
print
print "* Status Code: "+str(r.status_code)
print "* Response: "
print r.text
print
