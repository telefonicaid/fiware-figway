
// Copyright 2013 Telefonica Investigación y Desarrollo, S.A.U

// This file is part of FIGWAY software (a machine2machine gateway for the Raspberry PI).

// FIGWAY is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// FIGWAY is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License along with FIGWARE. If not, see http://www.gnu.org/licenses/.

// For those usages not covered by the GNU Affero General Public License please contact with: Carlos Ralli Ucendo [ralli@tid.es] 

// Developed by Ken Gunnar Zangelin

#ifndef CLIENT_SOCKET_HTTP_H
#define CLIENT_SOCKET_HTTP_H

#include <string>
#include <vector>

#define  TAM_BUF        8192


/***************************************************************************
*
* socketHttpConnect - 
*/
extern int socketHttpConnect(std::string host, unsigned short port);



/* ****************************************************************************
*
* sendHttpSocket - 
*/
extern std::string sendHttpSocket( std::string ip,
                                   unsigned short port, 
                                   std::string verb,
                                   std::string resource, 
                                   std::string content_type, 
                                   std::string content,
                                   bool waitForResponse = true
                                   );

#endif
