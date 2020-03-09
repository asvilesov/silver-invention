import smtplib
from email.mime.multipart import MIMEMultipart 
from email.mime.text import MIMEText 
from email.mime.base import MIMEBase 
from email import encoders 

def emailing_package(to_email, subject, body, filename = None):

	fh = open("../email_private.txt", "r")
	temp = fh.read().split('\n')

	gmail_user = temp[0]  
	gmail_password = temp[1]

	# instance of MIMEMultipart 
	msg = MIMEMultipart() 
	  
	# storing the senders email address   
	msg['From'] = gmail_user 
	  
	# storing the receivers email address  
	msg['To'] = to_email
	  
	# storing the subject  
	msg['Subject'] = subject
	  
	# attach the body with the msg instance 
	msg.attach(MIMEText(body, 'plain')) 
	  
	try:
		# open the file to be sent  
		attachment = open(filename, "rb")   
		# instance of MIMEBase and named as p 
		p = MIMEBase('application', 'octet-stream') 
		# To change the payload into encoded form 
		p.set_payload((attachment).read()) 
		# encode into base64 
		encoders.encode_base64(p) 
		p.add_header('Content-Disposition', "attachment; filename= %s" % filename) 
		# attach the instance 'p' to instance 'msg' 
		msg.attach(p) 
	except:
		print("Attachment not found")
	  
	# creates SMTP session 
	s = smtplib.SMTP('smtp.gmail.com', 587) 
	  
	# start TLS for security 
	s.starttls() 
	  
	# Authentication 
	s.login(gmail_user, gmail_password) 
	  
	# Converts the Multipart msg into a string 
	text = msg.as_string() 
	  
	# sending the mail 
	s.sendmail(gmail_user, to_email, text) 
	  
	# terminating the session 
	s.quit() 