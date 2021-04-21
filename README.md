# Login_System 🕸

🖐 Hey! this is my simple project for login system for students attendees with an arduino and python 🐍 it shows how fast and stable for doing the job for attendees students and send email every time they login it shows that small ideas it can be so efficient to the student with their time!

![github-large](https://www.aranacorp.com/wp-content/uploads/python-arduino.png)



## How does it work?<br/>
The procedure starts with the students arrival, he is obliged to bring his ID card with him if he wishes to enter the institution.
Next, the students card, that contains the data of his name, ID number and email, is placed against the RFID reader which sends the data directly to the Arduino.
After that, the data is presented on the LCD screen and the student is asked to place his finger close to the MLX90614 (temperature body sensor).
In that moment, the Arduino receives the data that contains the students name, ID number, email and body temperature and adds it to the Google sheet.
The Python is responsible for checking the Google sheet for incoming data and sending the collected data to the students email as proof of his attendance.

## The Objective<br/>
The objective of this project is to ensure the safety of the Institutes students due to the Covid-19 pandemic whilst collecting the data of their attendance in a safe way.
 
 
## Libraries that i used in this project are :
 all thanks to them of course! ♥


**Python**<br/>
1.[Gspread](https://github.com/burnash/gspread)<br/>
2.[smtplib,ssl](https://github.com/python/cpython)<br/>

**Arduino - C++**<br/>
1.[LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)<br/>
2.[MFRC522](https://github.com/miguelbalboa/rfid)<br/>
3.[MLXX90614](https://github.com/adafruit/Adafruit-MLX90614-Library)<br/>
4.[ESP8266WIFI](https://github.com/ekstrand/ESP8266wifi)
