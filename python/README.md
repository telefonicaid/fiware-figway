Description
-------------

- FIGWAY is an opensource tool to work with FIWARE Orion ContextBroker and/or FIWARE IDAS.2.6 (also known as SBC2.6).  

- The newest version is all under the "python/" folder. It is written in python and it has been tested in RaspberryPI and MacbookPro
 with OSX 10.9.3. It should work fine in any linux or any other system with Python2.7 installed. 

-  The "c++/" folder holds the previous version written in C++ and working only in RaspberryPI platform. You will find in
 that folder specific instructions while the remaining text within this file refers to the newest one at "python/".

- Prior to use this software, you need to configure it at "python/config.ini":
 * Note that to obtain the "username" & "token" within "[user]" configuration section, you need to have an account at 
   http://www.fi-ware.org/lab/ and then execute the script "python/get_token.py".
 * In the local [local] configuration section you need to configure the "host_type" (use "RaspberryPI" or "MAC" or "Linux", etc.)
   and the "host_id=" (use the 3 lower bytes in hexa of one of your Ethernet MACs or your e-mail if you do not know how to obtain
   the first).


- Current available folders:
python/ContextBroker/  -> Specific tools to talk to a ContextBroker. Use ContextBroker to read FIWARE IoT Data.
                           There is an specific README.md file in this directory.
                           Meanwhile, execute the commands with no arguments to get help.
python/SensorsUL20/    -> Specific tools to talk to IDAS2.6 to send IoT Data to FIWARE and to simulate virtual sensors (using Ultralight 2.0).
                           There is an specific README.md file in this directory.


Credits
------------
- Developed by Carlos Ralli Ucendo (@carlosralli). Nov 2014.
- Thanks to Rafael Baños Lopez for his great support and lots of testing/debugging efforts!


