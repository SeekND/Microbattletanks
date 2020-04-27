# Micro Battle Tanks
Internet/Local FPV micro IR Battle tanks


## See explanation video here: ## TBD


The creation of this tank comes after the findings I got from creating the Carlandoo jeep.
Instead of using any ESC, this project uses only two 9g servos for mobility.

I;m using the same device as before, called ESP8266 that creates their own wifi network and an app called RemoteXY that allows you to connect to said ESP8266 AND even design your own graphical inferface.

While studying this app I found it even is able to host a middle man server that your ESP connects to.
This means that if you want to control your ESP via the internet you don't have to open up ports in your router, like you do with other apps.

Unfortunately the 9g servos used only work with 4-5v so we need to add a polulu 5v boost to our project.
If you wish to use more voltage in your batteries, just use a polulu that brinks down the voltage instead.

For your benefit I made two seperate codes, one for controlling the Micro Tank locally and one for controlling it over the internet.


## DOWNLOAD THE APP

* Remotexy is available for IOS and Android >> http://remotexy.com
* If you intend to run this car over the internet you will need to register for a free account.
* Just as a note, I have 0 involvement with this company. I just found it to be a really great app for what I wanted to do :)


## PRINT THE PARTS

* 3D print of the Carlandoo parts. 
* Follow the guide in https://www.thingiverse.com/thing:3928745


## BUILD THE ELECTRONICS

You will need the following electronic parts as mentioned in Thingyverse*

- 1x ESP8266 (https://www.banggood.com/Geekcreit-NodeMcu-Lua-WIFI-Internet-Things-Development-Board-Based-ESP8266-CP2102-Wireless-Module-p-1097112.html)
- 1x 5v polulu boost > 
- 2x 9g servos (you will have to modify these) > 
- 1s battery (recommend 300mha battery) > 
- battery connector (recommend mxcp connector) > https://www.dronejunkie.co.uk/mylipo-lead-and-socket-for-mcpxpw-type-lipos
- 1x servo tester

*You can also install an FPV camera by simply connecting an AIO micro FPV camera to the polulu. 
*Make sure your camera can connect to 5v

Start by placing the ESP8266 in the bottom part of the base body of the Micro Tank.
I prefer putting the USB facing backwards so it's easier to install the code afterwards

Here comes the hard part. You will need to open up the 9g servos in otder to make them 180 rotational.
Here is a video by flitetest to show what you need to do >> https://www.youtube.com/watch?v=XUm2nj7te8E

Use hotglue to secure the servos in the body of the Micro Tank.
Once the servos and the ESP8266 are in place you can start soldering in all the electronics.

* Connect battery connector to 5v polulu IN pins (+ and -)

* Connect polulu OUT POS (+) to ESP8266 VIN
* Connect polulu OUT NEG (-) to ESP8266 GROUND closest to VIN

* Connect BROWN (-) wire from all 9g servos to polulu OUT NEG (-)
* Connect RED (+) wire from all 9g servos to polulu OUT POS (+)
* Connect left servo YELLOW (signal) wire to ESP8266 D7
* Connect right servo YELLOW (signal) wire to ESP8266 D6

* Extend 8cm with 28awg wire and connect middle leg of the IR receiver (-) to polulu OUT NEG (-)
* Extend 8cm with 28awg wire and connect left leg of the IR receiver (+)  to polulu OUT POS (+)
* Extend 8cm with 28awg wire and connect right leg of the IR receiver (signal) to ESP8266 D5

* Extend 8cm with 28awg wire and connect longer leg of the IR led (+) to ESP8266 D2
* Extend 8cm with 28awg wire and connect shorter leg of the IR receiver (-) to ESP8266 GROUND closest to D2



## LOCAL MICRO TANK CODE

Connect the esp8266 to your computer with a usb cable and load up the microbattletank_local.ino code using Arduino IDE.
If you don't know how to use Arduino IDE with ESP8266 please watch this short video: https://www.youtube.com/watch?v=OC9wYhv6juM
Remember you will need to install the RemoteXY Library in Arduino IDE. Read up on RemoteXY here: https://remotexy.com/en/help/

My upload settings are:
Board: Wemos D1 R1
Erase flash: All flash contents
All other settings are default

Once the flash is complete connect a battery to your Micro Tank, wait a few moments and you should be able to see a new network appear in your phone.
- Connect to this wireless network
- and then launch RemoteXY app
- in RemoteXY app click top right + and select WIFI Point, make sure the port is 6789

You should now be connected to your Micro Tank.
Enjoy!





## INTERNET MICRO TANK CODE

Before uploading any code you need to register for a free account in remotexy.com
Once registered, log in and press My Tokens on the top right of the page.
Create a new token for your tank. Name it as you see fit.


Connect the esp8266 to your computer with a usb cable and load up the microbattletank_cloud.ino code using Arduino IDE.
If you don't know how to use Arduino IDE with ESP8266 please watch this short video: https://www.youtube.com/watch?v=OC9wYhv6juM
Remember you will need to install the RemoteXY Library in Arduino IDE. Read up on RemoteXY here: https://remotexy.com/en/help/

Before uploading you will need to change a few settings in the code.

You will need to fill in your access point name (any accesspoints with spaces will NOT work)
Then you need to fill in your access point password
You will need to fill in the token you got from remotexy.com
And finally, you can even change some parameters of the tank if you wish under //////////////// USER VALUES.

You are now ready for upload :)

My upload settings are:
Board: Wemos D1 R1
Erase flash: All flash contents
All other settings are default

Once the flash is complete connect a battery to your Micro Tank, wait a few moments and your Micro Tank should now be connected to your accesspoint.
- Launch RemoteXY app
- in RemoteXY app click top right + and select Cloud
- Leave the other fields as they are and fill in your token.

You should now be connected to your Micro Tank and you or anyone with that token will be able to control it from anywhere in the world!!
Enjoy!


# CONTACTS

- https://www.youtube.com/SeekNDFPV
- https://www.facebook.com/SeekNDFPV/


# SUPPORT

If you'd like to help support the project please contribute with ideas.

If you would like to contribute in another way, I would be very grateful as well.
http://paypal.me/SeekND

Thank you for your support!!
