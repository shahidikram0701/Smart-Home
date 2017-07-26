import serial
import smtplib
from email.mime.text import MIMEText
from urllib import request
from http import cookiejar as cookielib
from getpass import getpass
import sys
import os
from stat import *

def send_sms(data):
    message = data
    number = "8197062848" #to which number sms should be sent

    if __name__ == "__main__":
        username = "9620115502"
        passwd = "Q9639K"

        message = "+".join(message.split(' '))

    #logging into the sms site
        url ='http://site24.way2sms.com/Login1.action?'
        data = 'username='+username+'&password='+passwd+'&Submit=Sign+in'

    #For cookies

        cj= cookielib.CookieJar()
        opener = request.build_opener(request.HTTPCookieProcessor(cj))

    #Adding header details
        opener.addheaders=[('User-Agent','Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.120')]
        try:
            usock =opener.open(url, data.encode("utf-8"))
        except IOError:
            print ("error")
            #return()

        jession_id =str(cj).split('~')[1].split(' ')[0]
        send_sms_url = 'http://site24.way2sms.com/smstoss.action?'
        send_sms_data = 'ssaction=ss&Token='+jession_id+'&mobile='+number+'&message='+message+'&msgLen=136'
        opener.addheaders=[('Referer', 'http://site25.way2sms.com/sendSMS?Token='+jession_id)]
        try:
            sms_sent_page = opener.open(send_sms_url,send_sms_data.encode("utf-8"))
        except IOError:
            print ("error")

        print("success")


arduino = serial.Serial('COM4', 9600, timeout=.1)
while True:
    data = arduino.readline()
    if data:
        send_sms(data.decode("utf-8"))
        msg = MIMEText(data.decode("utf-8"))
        msg['Subject'] = 'EMERGENCY';
        msg['From'] = 'mpcaproject@gmail.com'
        msg['To'] = 'mpcaproject@gmail.com'
        server = smtplib.SMTP("smtp.gmail.com", 587)
        server.ehlo()
        server.starttls()
        server.login("mpcaproject@gmail.com", "smarthome")
        server.send_message(msg)
        server.close()
        print(data)
