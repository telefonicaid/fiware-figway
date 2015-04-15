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

import requests, json
import ConfigParser
import io
import sys

CONFIG_FILE = "../config.ini"

# Load the configuration file
with open(CONFIG_FILE,'r+') as f:
    sample_config = f.read()
config = ConfigParser.RawConfigParser(allow_no_value=True)
config.readfp(io.BytesIO(sample_config))

CB_HOST=config.get('contextbroker', 'host')
CB_PORT=config.get('contextbroker', 'port')
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
PAYLOAD = {'some' : 'data'}
HEADERS = {'content-type': 'application/json' , 'X-Auth-Token' : TOKEN}
HEADERS_SHOW = {'content-type': 'application/json', 'accept': 'application/json' , 'X-Auth-Token' : TOKEN_SHOW}

URL = CB_URL + '/version'

print "* Asking to "+URL
print "* Headers: "+str(HEADERS_SHOW)
print "..."
r = requests.get(URL, data=json.dumps(PAYLOAD), headers=HEADERS)
print
print "* Status Code: "+str(r.status_code)
print "* Response: "
print r.text
print
