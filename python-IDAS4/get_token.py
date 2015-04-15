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
# A simple script to get a valid Oauth2.0 token for FIWARE Lab GEs access.

import requests, json, getpass

TOKENS_URL = "http://cloud.lab.fi-ware.org:4730/v2.0/tokens"

print
print "Now you will be prompted for your user/password within FIWARE Lab Oauth2.0 Authentication system"
print "If you didn't go and register first at http://cloud.fiware.org"
print
USER = raw_input("FIWARE Lab Username: ")
PASSWORD = getpass.getpass("FIWARE Lab Password: ") 
PAYLOAD = "{\"auth\": {\"passwordCredentials\": {\"username\":\""+USER+"\", \"password\":\""+PASSWORD+"\"}}}"
HEADERS =  {'content-type': 'application/json'} 
URL = TOKENS_URL

RESP = requests.post(URL, data=PAYLOAD, headers=HEADERS)
print
print "FIWARE Oauth2.0 Token: "+RESP.json()["access"]["token"]["id"]
print
print "Token expires: "+RESP.json()["access"]["token"]["expires"]
print
