OLED Alarm Project

First of all User can only set up up to 3 alarms interact  with the screen, all other alarms settings have to go to the web portal, and on Web portal maximum 10 alarms. 
Alarms that sets up from the web portal will not show on the OLED screen. 
We do the screen logic first, don’t worry about the wet portal at the moment but remember in the future, we will use web portal, so the code needs to be extendable for that feature.
Also, I’m going to use Wokwi simulation for this ESP32 project, in VS Code. So also make the .toml file, platformio.ini, and diagram.json file. For Visual Studio Code IDE.
The Temp sensor, please change to use DHT22, as Wokwi supports. Other wire connection are the same. Wifi use Wokwi wifi settings. Like below:

#include <WiFi.h>
void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

Buttons: Set, Select, Adjust.
Set: toggle between screens.
Select: select which setting needs to be changed.
Adjust: change the value.


Focus on the screen now:

the idle screen 

Show small icon for: Wifi signal on the top left corner ,Bluetooth signal on the right top corner.(if no Bluetooth connection detected, no need to show but leave that space empty.
Show only “Time”bigger and fill the whole line. show The temperature and humidity smaller in the next line. Only need to Show “Alarm on” or “Alarm off” the next line if there is any alarm enabled or not. And show date and day the bottom line. Adjust the text size to fit the screen.


Check Alarm screen like 

show “Alarms” as the screen title
Alarm 1, show Times and enable status
Alarm 2, show Times and enable status
Alarm 3, show Times and enable status
display Alarm number and name if the name is set on Web portal. If no name, display the alarm number only in front of the time to indicate which alarm it is.
If alarm is repeated alarm then there will be a (R) after the time, then show the day below the time like.(Mon, Tue , Wed, Thu, Fri, Sat, Sun) 
If alarm is a specific date alarm show the date as well after the time.
If at this screen, press “Select” button, user can select which alarm need to be configure (toggle between the 3 alarms) and press “Set” button to enter Alarm configuration screen. 


Alarm configuration screen

When you select any of the alarm and press Set button, it goes this screen. If you select alarm 1 from Alarm screen,for example, you can change this Alarm as below:

First user needs to decide what Type of alarm he/she needs: one time, specific date or repeated alarm. 

One Time Alarm: user can change to the desired hour or minute that toggling by Select button and adjust the value via “Adjust” buttons, and confirm and save by Set button.
For specific date alarm: users need have a chance to select a date. Use “select” button to toggle year, month and day, modify the value by Adjust button. “Set” button to confirm and save. Remember same buttons logic in the future. You can think of the detail of how to modify the value for the year, I can give an example like: start from current year to max 10 year from now, so when the year reach the max year then back to current year.
For repeated alarm: user can select multiple day to repeat, only selected day will show below the time. Use same button logic.
For any type of alarm, User can Enabled or disabled current selected alarm. Same button logic: Select, Adjust, Confirm.
User can select which Melody for the buzzer. Note: melody is preset in the code, and cannot be change by user. The melodies are “super mario”, “ happy birthday”, “merry Christmas”. The default melody is Super Mario. 

Repeated alarms, the day can be cleared by one button,

User can setup up to 10 alarms on The web, and only see the top 3 on screen. (Sequence by alarm number), user can only set up alarm from top to bottom, cannot skip an alarm, like leave the alarm 2 empty to setup alarm 3. This will make the logic simple and avoid leaving empty space on screen. 

• OLED SDA: 16
• OLED SCL: 17
• Mode button: 33
• Confirm button: 4
• Adjust button: 5
• DHT22 data: 14
• Buzzer: 15

I want alarm names to be stored even if only set via Web (to reserve the memory space) so when open it Web, user can see the saved alarm

disabled alarms still show in alarm list with full details, unless alarm is removed. So user need be able to delete alarms that set up in the screen. Other alarms can be exits from Web.
Use switch case instead of “if else” got better clean code when possible.
Let me know if you have any questions or any logic issue you find, we make sure all the requirements are class before we start the code.

Thought:

Deep sleep to save energy:

Keep in deep Sleep when no alarm enabled or 5mins further away of the next alarm. Until user touch T3 pin or alarm 5 mins closer.
Or add a fallback to skip sleeping entirely if alarm is too close.
Touch T3 to wake up.
Go into Deep sleep after 5 mins no interruptions.
