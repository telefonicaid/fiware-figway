
// Copyright 2013 Telefónica Investigación y Desarrollo, S.A.U

// This file is part of FIGWAY software (a machine2machine gateway for the Raspberry PI).

// FIGWAY is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// FIGWAY is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License along with FIGWARE. If not, see http://www.gnu.org/licenses/.

// For those usages not covered by the GNU Affero General Public License please contact with: Carlos Ralli Ucendo [ralli@tid.es] 


// Developed by Carlos Ralli Ucendo (@carlosralli), Aug 2013.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h> 
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFSIZE 1024
#define BACKLOG 10    
#define MAX_HTTPRESP_BUFF 10000
#define MAX_STR_COMP 80
#define MAX_LINE 128


/* ************************************************************************************************************
 * 
 * FindReplaceFile - This function finds a string in a file and replaces it with another string in a new file
 *
 **************************************************************************************************************/ 

int FindReplaceFile(std::string str2find, std::string str2repl, std::string fileOrig, std::string fileRepl) {
	
    char text2find[MAX_STR_COMP];
    char text2repl[MAX_STR_COMP];
    memcpy(text2find,str2find.c_str(),str2find.size()+1);
    memcpy(text2repl,str2repl.c_str(),str2repl.size()+1);
	
    char buffer[MAX_LINE+2];
    char *buff_ptr, *find_ptr;
    FILE *fp1, *fp2;
    size_t find_len = strlen(text2find);
	
    fp1 = fopen(fileOrig.c_str(),"r");
	
    if ( fp1 )  
    {
		fp2 = fopen(fileRepl.c_str(),"w");
		while(fgets(buffer,MAX_LINE+2,fp1))
		{
			buff_ptr = buffer;
			while ((find_ptr = strstr(buff_ptr,text2find)))
			{
				while(buff_ptr < find_ptr)
					fputc((int)*buff_ptr++,fp2);
				fputs(text2repl,fp2);
				buff_ptr += find_len;
			}
			fputs(buff_ptr,fp2);
		}
		fclose(fp2);
		fclose(fp1);
    }
    else 
    {
		perror(fileOrig.c_str());
    }
    return 0;
}



/* 
 **********************************************************************************
 * 
 * testDataFromFile - This function puts a given file into a string
 *
 **********************************************************************************/


std::string testDataFromFile(char* buffer, int bufSize, const char* fileName) {
	
	char path[512];
	int fd;
	int nb;
	
	struct stat sBuf;
	snprintf(path, sizeof(path), "%s", fileName);
	
	if (stat(path, &sBuf) == -1)
		return std::string("stat(") + path + "): " + strerror(errno);
	
	if (sBuf.st_size > bufSize)
		return std::string("buffer too small - ") + path;
	
	if ((fd = open(path, O_RDONLY)) == -1)
		return std::string("open(") + path + "): " + strerror(errno);
	
	nb = read(fd, buffer, sBuf.st_size);
	
	if (nb == -1)
		return std::string("read(") + path + "): " + strerror(errno);
	
	if (nb != sBuf.st_size)
		return std::string("bad size read from ") + path;
	
	buffer[nb] = 0;
	
	return "OK";
}



void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argC, char* argV[]) {

    int sockfd, new_fd; 
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; 
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    int n;
    char buf[BUFSIZE];
    std::string contentData; 
    std::string respFilename;
    std::string httppath;
    std::string aux;
    std::string  comparestr;
    std::string  command;
    std::string  param1;
    std::string  deviceNumber;
    std::string  deviceType;
    std::string  serverPort;
    char buff[MAX_HTTPRESP_BUFF];
    char *ss;
    int cont;
    int contres;
    char resu[128];
    std::string fileSuffix; 
    std::string fileOrig;
	std::string fileRepl;
	std::string str2find;
	std::string str2repl;
        std::string text2find;
        std::string text2repl;
	
	int ret;

    switch (argC) {
     case 4:
      deviceNumber = argV[1];
      deviceType = argV[2];
      serverPort = argV[3];
      break;
      default:
      fprintf(stdout, "Usage: %s [Device-Number] [Server-Port].\n", argV[0]);
      fprintf(stdout, " [Device-Number] = integer identifying the switch device given in the z-wave network inclusion.\n");
      fprintf(stdout, " [Device-Type] = Cuurently supported: SWITCH, RGBS.\n");
      fprintf(stdout, " [Server-Port] = TCP port of the RaspberryPI where the daemon will be attached. Recommended: 7777, 7778...\n");
      exit(0);
       break;
    }
	
    httppath = "HTTP/";	
//    deviceType = "SWITCH";

    memset(&hints, 0, sizeof hints);

//  To get the server listening both v4 & v6 in the RPI, force AF_INET6 as AF_UNSPEC does not seem to work for server.
//    hints.ai_family = AF_UNSPEC;
    hints.ai_family = AF_INET6;

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 

    if ((rv = getaddrinfo(NULL, serverPort.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); 

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; 
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) { 
            close(sockfd); 

            bzero(buf, BUFSIZE);
            n = read(new_fd, buf, BUFSIZE);
            if (n < 0)
                perror("read");
            printf("server received %d bytes: %s", n, buf);


           comparestr = " id=\"";
           ss = strstr(buf, comparestr.c_str());
           if (ss != NULL) 
            {
              printf("Found IDAS_CALL_ID at index = %d.\n", ss - buf);

            memset (resu,0, 128);
            cont = ss - buf;
            contres = 0;

            cont += strlen(comparestr.c_str());
            while ((buf[cont] != '\"') && (buf[cont] != '<') && (buf[cont])) {resu [contres] = buf[cont];cont++;contres++;}  
            printf("IDAS_CALL_ID = <%s>.\n", resu);

            fileSuffix = ".tmp";
            fileOrig = httppath + "HTTP_200";
	    fileRepl = httppath + "HTTP_200" + fileSuffix;
	    str2find = "IDAS_CALL_ID";
	    str2repl = resu;
				
	    ret = FindReplaceFile(str2find, str2repl, fileOrig, fileRepl);			
	   }
	   else {
//	    fileRepl = httppath + "HTTP_404";	
            printf("Not found an IDAS_CALL_ID (However command may still work)\n", resu);			
            fileRepl = httppath + "HTTP_200";
	   }

           comparestr = "FIZCOMMAND";
           ss = strstr(buf, comparestr.c_str());
           if (ss != NULL) 
            {
              printf("Found FIZCOMMAND at index = %d.\n", ss - buf);

            memset (resu,0, 128);
            cont = ss - buf;
            contres = 0;

            cont += strlen(comparestr.c_str()); 
            while (buf[cont] == ' ') cont++;
            while ((buf[cont] != ' ') && (buf[cont] != '<') && (buf[cont])) {resu [contres] = buf[cont];cont++;contres++;}
             
            printf("Calling <fizway_command %s>.\n", resu);
            command = "./fizway_command "+deviceNumber + " " + deviceType + " " + resu;

            system(command.c_str());

            respFilename = fileRepl;
            aux = testDataFromFile(buff, sizeof(buff), respFilename.c_str());
            contentData = buff;

            if (send(new_fd, contentData.c_str(), contentData.size(), 0) == -1)
                perror("send");
            printf("server sent %d bytes: %s", contentData.size(), contentData.c_str());

            }                              
           else
           {
            printf("NO FIZCOMMAND FOUND.\n");

            respFilename = httppath + "HTTP_404";
            aux = testDataFromFile(buff, sizeof(buff), respFilename.c_str());
            contentData = buff;

            if (send(new_fd, contentData.c_str(), contentData.size(), 0) == -1)
                perror("send");
            printf("server sent %d bytes: %s", contentData.size(), contentData.c_str());

           } 

            close(new_fd);
            exit(0);
        }
        close(new_fd); 
    }

    return 0;
}
