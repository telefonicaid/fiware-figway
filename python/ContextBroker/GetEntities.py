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

if NUM_ARG==2:
   ENTITY_TYPE=sys.argv[1]
else:
   print 'Usage: '+COMMAND+' [ENTITY_TYPE] '
   print '  You may use ENTITY_TYPE: ALL to list all Entities belonging to all Entity types.'
   sys.exit(2)

if ENTITY_TYPE == "ALL":
   ENTITY_TYPE = ""

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
PAYLOAD = '{                \
    "entities": [           \
    {                        \
        "type": "'+ENTITY_TYPE+'",   \
        "isPattern": "true", \
        "id": ".*"  \
    }  \
    ],  \
    "attributes" : [ ] \
}'


#HEADERS = {'content-type': 'application/json', 'accept': 'application/json' , 'X-Auth-Token' : TOKEN}
HEADERS = {'content-type': 'application/json', 'Fiware-Service': CB_FIWARE_SERVICE ,'X-Auth-Token' : TOKEN}
HEADERS_SHOW = {'content-type': 'application/json', 'accept': 'application/json' , 'Fiware-Service': CB_FIWARE_SERVICE , 'X-Auth-Token' : TOKEN_SHOW}
URL = CB_URL + '/ngsi10/queryContext'

print "* Asking to "+URL
print "* Headers: "+str(HEADERS_SHOW)
print "* Sending PAYLOAD: "
print json.dumps(json.loads(PAYLOAD), indent=4)
print
print "..."
r = requests.post(URL, data=PAYLOAD, headers=HEADERS)
print
print "* Status Code: "+str(r.status_code)

list_id = re.findall(r'<id>[\w]*.', r.text)
list_type = re.findall('<entityId type=\S+ isPattern=\S+>', r.text)
Num_id=len(list_id)
Num_type=len(list_type)
cnt_id=Counter(list_id)
cnt_type=Counter(list_type)

print "***** Number of Entity Types: "+str(Num_type)
print
print "***** List of Entity Types"
for x in cnt_type.most_common():
    print '%s : %d' % x
print
print "**** Number of Entity IDs: "+str(Num_id)
print
print "**** List of Entity IDs"
for x in cnt_id.most_common():
    print '%s : %d' % x
print

ASK = str(raw_input("Do you want me to print all Entities? (yes/no)"))
if ASK == "yes":
  print r.text

print
