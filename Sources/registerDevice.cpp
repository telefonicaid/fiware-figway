
// Copyright 2013 Telefonica Investigación y Desarrollo, S.A.U

// This file is part of FIGWAY software (a machine2machine gateway for the Raspberry PI).

// FIGWAY is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// FIGWAY is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License along with FIGWARE. If not, see http://www.gnu.org/licenses/.

// For those usages not covered by the GNU Affero General Public License please contact with: Carlos Ralli Ucendo [ralli@tid.es] 


// Developed by Carlos Ralli Ucendo (@carlosralli), Aug 2013.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <unistd.h>                             
#include <fcntl.h>                              
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <sys/ioctl.h> 
#include <sys/socket.h>
#include <net/if.h>

#include "clientSocketHttp6-test.h"
#define MAX_STR_COMP 80
#define MAX_LINE 128
#define MAX_FILE 256
#define MAX_SML_BUFF 10000
#define PROTOCOL_SML      "SML"
#define PROTOCOL_NGSI     "NGSI"
#define CONFIG_FILE       "./Config"


/* ************************************************************************************************************
 *
 * GetmyMAC() - This function returns a unique identifier of a Raspberry PI (model B) based on the 3 low
 *  bytes of the MAC address of eth0 network interface. All model B Raspberry PIs have an eth0 interface. 
 *
 **************************************************************************************************************/


std::string GetmyMAC() {
    int s;
    char rpiID_str[10];
    std::string rpiID;
    struct ifreq buffer;

    s = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&buffer, 0x00, sizeof(buffer));
    strcpy(buffer.ifr_name, "eth0");
    ioctl(s, SIOCGIFHWADDR, &buffer);
    close(s);


    snprintf (rpiID_str, sizeof(rpiID_str), "%.2X:%.2X:%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[3], (unsigned char)buffer.ifr_hwaddr.sa_data[4], (unsigned char)buffer.ifr_hwaddr.sa_data[5]);
    rpiID = rpiID_str;
//  printf("-> getMyMAC(): RaspberryPI IoT gateway ID (rpiID): <%s>.\n", rpiID.c_str());
    return rpiID;
}


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

/* ************************************************************************************************************
 * 
 * testDataFromFile - This function puts a given file into a string
 *
 **************************************************************************************************************/


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

/* ************************************************************************************************************
 * 
 * main ()
 *
 **************************************************************************************************************/



int main(int argC, char* argV[]) {

    std::string      idasIP;
    unsigned short   idasPort;
    std::string      smlPath;
    std::string      miApiKey;
    std::string      deviceType;
    std::string      deviceID; 
    std::string      str2find;
    std::string      str2repl;
    std::string      rpiID;
    std::string      fileOrig;
    std::string      fileRepl;
    std::string      fileSuffix;
    std::string      platformType;
    int              debugSwitch;

    std::string      resourceDir;
    std::string      contentData;
    std::string      aux;
    std::string      replyData;
    std::string      method = "POST";
    std::string      smlFilename;
    FILE           * configFile;
    int Num;	
    int ret;
    char buff[MAX_SML_BUFF];
    char addressBuff[MAX_FILE];

    switch (argC) {
     case 3:
      deviceID = argV[1];
      deviceType = argV[2];
     break;
     default:
     fprintf(stdout, "Usage: %s [Device-ID] [Device-Type].\n", argV[0]);
     fprintf(stdout, " [Device-ID]   = 4 characters string identifying the device (Recommended: 0001, 0002, etc.).\n");
     fprintf(stdout, " [Device-Type] = Device type (must match SensorML or NGSI file Register Templates).\n\n");
     exit(0);
     break;
    }

    configFile = fopen (CONFIG_FILE,"r");
    if (configFile==NULL)
    {
     fprintf(stdout,"No configuration file found: <%s>. Use <%s.example> to create a new one.\n", CONFIG_FILE);
     exit(0);
    }

    char line[MAX_FILE];
    int linenum=0;
    while(fgets(line, MAX_FILE, configFile) != NULL)
    {
     linenum++;
     if(line[0] == '#') continue;

     Num = 0;
     if(sscanf(line, "DEBUG = %d", &Num) == 1) 
     {  
       if ((Num +0)<0 || Num > 4|| Num == 0 ) 
        {
//          printf("-> Config file: No Debug.\n");
          debugSwitch = 0;        
         }
         else 
         {
       debugSwitch = Num;
       printf("-> Config file: Debug Level <%d>.\n",debugSwitch);
         }
        }

     memset(addressBuff, 0, MAX_FILE); 
     if(sscanf(line, "PLATFORM_IP = %s", addressBuff) == 1)
     {
      idasIP = addressBuff;
      if (debugSwitch >= 4) printf("-> Config file: Platform IP = <%s>.\n", idasIP.c_str()); 
     }

     memset(addressBuff, 0, MAX_FILE);
     if(sscanf(line, "PLATFORM_PROTO = %s", addressBuff) == 1)
     {
      platformType = addressBuff;
      if (debugSwitch >= 4) printf("-> Config file: Platform Protocol = <%s>.\n", platformType.c_str()); 
     }
 

     memset(addressBuff, 0, MAX_FILE);
     if(sscanf(line, "APIKEY = %s", addressBuff) == 1)
     {
      miApiKey = addressBuff;
      if (debugSwitch >= 4) printf("-> Config file: APIKEY = <%s>.\n", miApiKey.c_str());
     }

     Num = 0;
     if(sscanf(line, "PLATFORM_PORT = %d", &Num) == 1) 
     {
      if ((Num +0)<0 || Num > 65536|| Num == 0 ) 
      { 
       if (debugSwitch >= 4) printf("-> Config file: port out of range.\n");
       fclose (configFile); 
       exit(1); 
      } 
      else 
      {
       idasPort=Num;
       if (debugSwitch >= 4) printf("-> Config file: Platform Port = <%d>.\n", idasPort);       
      }
      }
     }

     fclose (configFile);

    if (strcmp (platformType.c_str() , PROTOCOL_NGSI) == 0)
    {
     smlPath = "NGSI/";
     fileSuffix = ".ngsi";
     resourceDir = "/ngsi9/registerContext";
    }
    else if (strcmp (platformType.c_str() , PROTOCOL_SML) == 0)
    {
    smlPath ="SensorML/";
    fileSuffix = ".sml";
    resourceDir = "/idas/sml?apikey=" + miApiKey; 
    }
    else {
    fprintf(stdout, "Illegal M2M Platform protocol <%s>.\n", platformType.c_str());
    exit(0);
    }

    rpiID = GetmyMAC ();
	
    fileOrig = smlPath + "Register_" + deviceType;
    fileRepl = smlPath + "Register_" + deviceType + "_" + deviceID + fileSuffix;
    str2find = "DEVICE_TYPE:RPI_ID:DEVICE_ID";
    str2repl = deviceType + ":" + rpiID + ":" + deviceID;
	
    ret = FindReplaceFile(str2find, str2repl, fileOrig, fileRepl);
	
    if (debugSwitch >= 3) fprintf(stdout, "-> RaspberryPI IoT gateway ID (rpiID): <%s>.\n", rpiID.c_str());
    if (debugSwitch >= 3) fprintf(stdout, "-> Registry of a sensor type: <%s>.\n", deviceType.c_str());
	
    smlFilename = smlPath + "Register_" + deviceType + "_" + deviceID + fileSuffix;

    if (debugSwitch >= 3) fprintf(stdout, "-> Calling SendHttptSocket6, Server = <%s>, Port: <%d>.\n", idasIP.c_str(), idasPort);   
    if (debugSwitch >= 3) fprintf(stdout, "-> Calling testDatafromFile, smlFilename = <%s>.\n", smlFilename.c_str()); 

    aux = testDataFromFile(buff, sizeof(buff), smlFilename.c_str());
    contentData = buff;

    if (debugSwitch >= 3) fprintf(stdout, "-> M2M Platform HTTP POST resourceDir = <%s>.\n", resourceDir.c_str()); 
    if (debugSwitch >= 2) fprintf(stdout, "-> M2M Platform HTTP POST content: \n **** \n %s \n **** \n", contentData.c_str());
 
    replyData = sendHttpSocket( idasIP,
                                idasPort,
                                method,
                                resourceDir,
                                std::string ("application/xml"),
                                contentData);
    
    if (debugSwitch >= 1) fprintf(stdout, "-> M2M Platform reply: \n **** \n %s \n **** \n", replyData.c_str());

}
