#!/usr/bin/python

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

import SocketServer, json, sys, os, time


NUM_ARG=len(sys.argv)
COMMAND=sys.argv[0] 

if NUM_ARG==3:
   SERVER_PORT=eval(sys.argv[1])
   HANDLING_SCRIPT=sys.argv[2]
else:
   print 'Usage: '+COMMAND+' [PORT] [HANDLING_FILE_SCRIPT]'
   print '  Where PORT = The TCP port the server will be listening at.'
   print '        HANDLING_FILE_SCRIPT = The script that will be called once the server receives something. Use your own or SubscriptionHandler.py'
   print
   sys.exit(2)

SERVER_OK_RESPONSE = {'return':'ok'}

# Use this for only-IPv4 incoming connections 
SERVER_ADDRESS='127.0.0.1'

# User this one for IPv4 or IPv6 connections.
# IMPORTANT: For this to work in Python 2.7 you need to edit "/usr/lib/python2.7/SocketServer.py" file
#               Change "address_family = socket.AF_INET" line by "address_family = socket.AF_INET6".
#            This is because SocketServer is buggy on this. Check this out at: http://bugs.python.org/issue20215 
#            For Python 3.5 there is a patch available at the link above.

#SERVER_ADDRESS='::'

class MyTCPServer(SocketServer.ThreadingTCPServer):
    allow_reuse_address = True

class MyTCPServerHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        try:
            data = json.loads(self.request.recv(1024).strip())
            # process the received data
            EXEC_FILE = 'python '+HANDLING_SCRIPT+' "'+str(data)+'"'
            localtime = time.asctime( time.localtime(time.time()) )
            print localtime+" - SubscriptionDaemon.py calling: ", EXEC_FILE
            os.system(EXEC_FILE)
            # send 'ok' back to client
            self.request.sendall(json.dumps(SERVER_OK_RESPONSE))
        except Exception, e:
            localtime = time.asctime( time.localtime(time.time()) )
            print localtime+" - Exception while receiving message: ", e

server = MyTCPServer((SERVER_ADDRESS, SERVER_PORT), MyTCPServerHandler)
server.serve_forever()
