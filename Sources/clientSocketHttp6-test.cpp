
// Copyright 2013 Telefonica Investigación y Desarrollo, S.A.U

// This file is part of FIGWAY software (a machine2machine gateway for the Raspberry PI).

// FIGWAY is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// FIGWAY is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License along with FIGWARE. If not, see http://www.gnu.org/licenses/.

// For those usages not covered by the GNU Affero General Public License please contact with: Carlos Ralli Ucendo [ralli@tid.es] 

// Developed by Ken Gunnar Zangelin

#include <string>
#include <vector>

#include "clientSocketHttp6-test.h"
#include <iostream>

#include <sys/types.h>                          
#include <sys/socket.h>                         
#include <sys/un.h>                             
#include <netinet/in.h>                        
#include <netdb.h>                              
#include <arpa/inet.h>                          
#include <netinet/tcp.h>                        
#include <string>
#include <unistd.h>                             
#include <stdio.h>				
#include <errno.h>				


/* ****************************************************************************
*
* socketHttpConnect -
*/
int socketHttpConnect(std::string host, unsigned short port)
{
  int                 fd;
  int status;
  struct addrinfo hints;   
  struct addrinfo *peer; 
  char port_str[10];
 
 memset(&hints, 0, sizeof(hints)); 
 hints.ai_family = AF_UNSPEC;  
 hints.ai_socktype = SOCK_STREAM;
 hints.ai_protocol = 0;

 snprintf (port_str, sizeof(port_str), "%d" , port);

 if ((status = getaddrinfo(host.c_str(), port_str, &hints, &peer)) != 0) {
/*     LM_RE(-1, ("getaddrinfo('%s'): %s", host.c_str(), strerror(errno))); */   
}

  if ((fd = socket(peer->ai_family, peer->ai_socktype, peer->ai_protocol)) == -1) {
/*     LM_RE(-1, ("socket: %s", strerror(errno))); */
}

  if (connect(fd, peer->ai_addr, peer->ai_addrlen) == -1)
  {
    freeaddrinfo(peer);
    close(fd);
/*    LM_E(("connect(%s, %d): %s", host.c_str(), port, strerror(errno))); */
    return -1;
  }
  freeaddrinfo(peer);
  return fd;
}

/* ****************************************************************************
*
* sendHttpSocket -
*
*/
std::string sendHttpSocket( std::string ip,
                           unsigned short port,
                           std::string verb,
                           std::string resource,
                           std::string content_type,
                           std::string content,
                           bool waitForResponse)
{
  char         host[128];
  char         dataLen[32];
  char         buffer[TAM_BUF];
  char         response[TAM_BUF];
  std::string  msg;
  std::string  result;

 
  memset (buffer, 0, TAM_BUF);
  memset (response, 0, TAM_BUF);
 
  msg = verb;
  msg += " ";
  msg += resource;
  msg += " ";
  msg += "HTTP/1.1";   
  msg += "\n";        


  snprintf(host, sizeof(host), "Host: %s:%d\n", ip.c_str(), port);

  msg += "User-Agent: NGSI Rest Library\n";
  msg += host;
  msg += "Accept: */*\n";

  if ((!content_type.empty()) && (!content.empty()) ) 
  {
    snprintf(dataLen, sizeof(dataLen), "Content-Length: %zu\n", content.length() + 1);

    msg += "Content-Type: ";
    msg += content_type;
    msg += "\n";
    msg += dataLen;
    msg += "\n";
    msg += content;
    msg += "\n";
  }
  else
  {
    // It is mandatory to put \n, othrewise fails
    msg += "\n";
  }
  int fd = socketHttpConnect(ip, port); 

  int sz;
  sz = strlen(msg.c_str());
  int nb;
  nb = send(fd, msg.c_str(), sz, 0);

  if (waitForResponse) {
      nb = recv(fd,&buffer,TAM_BUF-1,0);

      if (nb == -1)
      {      }
      else if ( nb >= TAM_BUF)
      {      }
      else
      {
          memcpy (response, buffer, nb);
      }

      if (strlen(response) > 0)
      {
          result = response;
      }

  }
  else
  {
     result = "";
  }

  close(fd);
  return result;
}
