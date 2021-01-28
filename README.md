# Setting up your Raspberry Pi
## 1. Booting the Pi

The Raspberry Pi Foundation has very good documentation on their site for inital setup of your Pi: https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up/

You will want to use the Raspberry Pi OS (32-bit) image for your pi.


## 2. Enabling I2C

Adafruit has very good documentation on their site for enabling I2C serial communication on your Pi: https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c

Don't be concerned if nothing shows up when you try to detect I2C devices because you haven't connected anything yet!


## 3. Installing 2501lib and the required libraries

To install the 2501 library (and the required libraries it uses), I've created a simple install script for you.  Open up a terminal and type the following command to download the library and install script from my GitHub:
`git clone https://github.com/TylerSeiford/2501lib.git`

Once that is done, run the following command to enter the cloned GitHub repository:
`cd 2501lib`

Then run the following command and everything should install itself:
`./install.sh`


## 4. Installing VS Code

You will want to follow this blog post on how to install visual studio code on your Pi: https://pimylifeup.com/raspberry-pi-visual-studio-code/


## 5. Update OS

To get all updates to installed packages, run the following commands in terminal:
 - `sudo apt update` (This updates your pi's list of available packages and their latest versions)

 - `sudo apt upgrade` (This will download and install the latest versions of the installed packages)

The second command will ask you to confirm if you want to install the updates, just type a y letter and then press enter to do so.


## 6. Enabling remote terminal access

Using a separate keyboard, mouse, and monitor can be a pain to always accessing your Pi, and would be very difficult once you have your Pi on your robot.  A simple and secure method to remotely access your Pi is to enable SSH.  Raspberry Pi Foundation has a good tutorial on this which can be found here: https://www.raspberrypi.org/documentation/remote-access/ssh/

Once you have confirmed that SSH works, you can safely disconnect your Pi and use it over SSH.  This will not allow you full access to the user interface, but in my experience that is not needed if you setup VS Code to work remotely.


## 7. Remote VS Code [Advanced]

VS Code running on the Pi can be fairly slow, and trying to run it though something like a remote desktop connection or X11 forwarding is also slow, in my experience the best way to do it is to also install VS Code on your local computer, and use the SSH extension.  Microsoft has some documentation on it here: https://code.visualstudio.com/docs/remote/ssh


## 8. Remote GUI access

Remote access with a GUI will require you to either use a remote desktop connection or X11 forwarding.  If you want to use either of these, follow the instructions below for your computer OS.


### Windows Client:
 - SSH+X11: https://www.raspberrypi.org/documentation/remote-access/ssh/windows.md && https://sourceforge.net/projects/vcxsrv/
 - RDP: https://pimylifeup.com/raspberry-pi-remote-desktop/

### ChromeOS Client [UNTESTED]
 - SSH+X11: https://chrome.google.com/webstore/detail/secure-shell-app/pnhechapfaindjhompbnflcldabbghjo?hl=en
 - RDP: https://play.google.com/store/apps/details?id=com.microsoft.rdc.android

### Linux Client
 - SSH+X11: https://www.raspberrypi.org/documentation/remote-access/ssh/unix.md

### MacOS Client
 - SSH+X11: https://www.raspberrypi.org/documentation/remote-access/ssh/unix.md
