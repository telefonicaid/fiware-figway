
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
#include <time.h>

#define MAX_STR_COMP 80
#define MAX_LINE 128 
#define MAX_FILE 256
#define MAX_SML_BUFF 10000
#define PROTOCOL_SML      "SML"
#define PROTOCOL_NGSI     "NGSI"   
#define CONFIG_FILE       "./Config"


/* ************************************************************************************************************
 * 
 * FindReplaceFile - This function finds a string in a file and replaces it with another string in a new file
 *
 **************************************************************************************************************/


int FindReplaceFile(std::string str2find, std::string str2repl, std::string fileOrig, std::string fileRepl) {
 
    char text2find[MAX_STR_COMP];
    char text2repl[MAX_STR_COMP] ;
      
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
 * main ()
 *
 **************************************************************************************************************/


int main(int argC, char* argV[]) {

    std::string      smlPath;
    std::string      fileSuffix = ".obs";
    std::string      deviceType;
    std::string      deviceID; 
    std::string      obsType;
    std::string      obsValue;
    std::string      str2find;
    std::string      str2repl;

    std::string      fileTemplate;
    std::string      fileOrig;
    std::string      fileRepl;
    std::string      fileRepl2;
    std::string      fileFinal ;

    std::string      platformType;
    FILE           * configFile;
    int Num;
    int debugSwitch;
    int ret;
    char buff[MAX_SML_BUFF];
    char addressBuff[MAX_FILE];

    std::string obsTime;
    char obsTime_str[30];
    time_t now_t;
    struct tm now;
    time(&now_t);
    now = *localtime(&now_t);
    snprintf (obsTime_str, sizeof(obsTime_str), "%4d-%02d-%02dT%02d:%02d:%02dZ",now.tm_year+1900, now.tm_mon+1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);
    obsTime = obsTime_str;

//    fprintf(stdout, "Observation Timestamp <%s>.\n", obsTime.c_str());

    switch (argC) {
     case 5:
      deviceID = argV[1];
      deviceType = argV[2];
      obsType = argV[3];
      obsValue = argV[4];
     break;
     default:
     fprintf(stdout, "Usage: %s [Device-ID] [Device-Type] [Obs-type] [Obs-value].\n", argV[0]);
     fprintf(stdout, " [Device-ID] = 4 characters string identifying the device (Recommended: Hexa coding: 0001, 0002, etc.).\n");
     fprintf(stdout, " [Device-Type] = device type (must match SensorML or NGSI file Register and Observation Templates).\n");
     fprintf(stdout, " [Obs-Type] = observation type (must match SensorML or NGSI file Register and Observation Templates).\n");
     fprintf(stdout, " [Obs-Value] = string with the Observation value.\n\n");
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
//				printf("-> Config file: No Debug.\n");
				debugSwitch = 0;        
			}
			else 
			{
				debugSwitch = Num;
				printf("-> Config file: Debug Level <%d>.\n",debugSwitch);
			}
        }
				
		memset(addressBuff, 0, MAX_FILE);
		if(sscanf(line, "PLATFORM_PROTO = %s", addressBuff) == 1)
		{
			platformType = addressBuff;
			if (debugSwitch >= 4) printf("-> Config file: Platform Protocol = <%s>.\n", platformType.c_str()); 
		}
		
	}
	
	fclose (configFile);
		

    if (strcmp (platformType.c_str() , PROTOCOL_NGSI) == 0)
    {
     smlPath = "NGSI/";
    }
    else if (strcmp (platformType.c_str() , PROTOCOL_SML) == 0)
    {
     smlPath ="SensorML/";
    }
    else {
    fprintf(stdout, "Illegal M2M Platform protocol <%s>.\n", platformType.c_str());
    exit(0);
    }

	fileTemplate = smlPath + "Observation_" + deviceType + "_" + obsType;
        fileOrig  = smlPath + "Observation_" + deviceType + "_" + obsType + "_" + deviceID + fileSuffix;
        fileRepl  = smlPath + "Observation_" + deviceType + "_" + obsType + "_" + deviceID + fileSuffix + "_temp";
        fileRepl2  = smlPath + "Observation_" + deviceType + "_" + obsType + "_" + deviceID + fileSuffix + "_temp2";
        fileFinal = smlPath + "Observation_" + deviceType + "_" + obsType + "_" + deviceID + fileSuffix;
	str2find = obsType + "_VALUE";
        str2repl = obsValue;

        if( access( fileOrig.c_str(), F_OK ) != -1 ) {        
         if (debugSwitch >= 2) fprintf(stdout, "-> Modifying Observations file: <%s>.\n", fileOrig.c_str());
        } 
        else {
         if (debugSwitch >= 2) fprintf(stdout, "-> No Observations file: <%s>. Creating new one from template <%s>.\n", fileOrig.c_str(), fileTemplate.c_str());    
         fileOrig = fileTemplate;    
        }

	ret = FindReplaceFile(str2find, str2repl, fileOrig, fileRepl);

        str2find = "TIME_ISO8601";
        str2repl = obsTime;

        ret = FindReplaceFile(str2find, str2repl, fileRepl, fileRepl2);

        ret = remove (fileFinal.c_str());
        ret = remove (fileRepl.c_str());
        ret = rename (fileRepl2.c_str(), fileFinal.c_str());
	
        if (debugSwitch >= 1) printf("-> Created file <%s> with observation type <%s> and value <%s>. TimeStamp <%s>.\n", fileFinal.c_str(), obsType.c_str(), obsValue.c_str(),obsTime.c_str());
}
