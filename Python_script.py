import gspread
import time
import threading
import smtplib,ssl
from email.message import EmailMessage

lock = threading.RLock()

gc = gspread.service_account(filename='keys.json')
filename = "sheet name!"

class DataProcessThread(threading.Thread):
    def __init__(self, records, frow):
        threading.Thread.__init__(self)
        self.records = records
        self.frow = frow
    def run(self):
        for record in self.records:
            recordTime = record[0]
            recordName = record[1]
            recordID = record[2]
            recordTemp = record[3]
            recordEmail = record[4]

            isSent = self.emailsend(recordTime, recordName, recordID, recordTemp, recordEmail)
            if (isSent is True):
                self.checkmark(self.records.index(record)+ self.frow)


    def emailsend(self, time, name, id, temp, email):
        EmailAdd = "The email will send through"
        Pass = "The Password!"
        msg = EmailMessage()
        msg['Subject'] = 'the subject!'
        msg['From'] = EmailAdd
        msg['To'] = email
        msg.set_content('The body of the message!')
        try:
            with smtplib.SMTP_SSL('smtp.gmail.com',465) as smtp:
                smtp.login(EmailAdd,Pass)
                smtp.send_message(msg)
                return True
        except:
            return False

    def checkmark(self,nrow):
        lock.acquire()
        sh = gc.open(filename).sheet1
        lock.release()

        lock.acquire()
        sh.update_acell('F'+str(nrow), ' ✅')
        lock.release()

            
class DataReadThread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    def run(self):
        updated_row = 1 
        a = True
        while True:
            lock.acquire()
            sh = gc.open(filename).sheet1
            lock.release()
            if(a):
                print("waiting for new data")
                a = False
            if(updated_row != sh.row_count):
                separator = ':'
                first_row = updated_row+1
                last_row = sh.row_count

                lock.acquire()
                values = sh.get(separator.join([str(first_row), str(last_row)]))
                lock.release()

                updated_row = last_row
                newDataProcess = DataProcessThread(values, first_row)
                newDataProcess.start()
                a = True
            time.sleep(5)

startReading = DataReadThread()
startReading.start()