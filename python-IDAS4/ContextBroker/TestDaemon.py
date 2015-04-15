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
# Developed by Carlos Ralli Ucendo (@carlosralli), Nov 2014

import socket
import json
import sys

NUM_ARG=len(sys.argv)
COMMAND=sys.argv[0] 

if NUM_ARG==2:
   SERVER_PORT=eval(sys.argv[1])
else:
   print 'Usage: '+COMMAND+' [PORT]'
   print '  Where PORT = The TCP port the server is listening at.'
   print
   sys.exit(2)

data = {'message':'hello world!', 'test':123.4}

# Use this for IPv4
SERVER_ADDRESS = '127.0.0.1'
ADDRESS_FAMILY = 'AF_INET'

# Use this if your server is listening IPv6 connections.
#SERVER_ADDRESS = '::1'
#ADDRESS_FAMILY = 'AF_INET6'

SOCKET_FAMILY = 'socket.'+ADDRESS_FAMILY
SOCKET_TYPE = 'socket.SOCK_STREAM'
s = socket.socket(eval(SOCKET_FAMILY), eval(SOCKET_TYPE))
s.connect((SERVER_ADDRESS, SERVER_PORT))
s.send(json.dumps(data))
result = json.loads(s.recv(1024))
print result
s.close()
