Project group number: 84
Group member names and x500s: 
	Justin Pederson: pede0831@umn.edu, server.c
	Chiemeka Nwakama: nwaka013@umn.edu, send_file and main
	Ibrahim Ismail-Adebiyi: ismai128@umn.edu, receive_file
CSELabs computer the code was tested on: csel-kh1250-01.cselabs.umn.edu
We did not make any changes to the makefile.
We did not make any additional assumptions.
For packaging sending we made the packet for the client and send it to the server after the 
connnection and accept was complete. This was then decoded and and used to get the image size, 
rotation angle, and operation. The image data was then send to the server from the client as well 
after the packet was send. This was then used to complete the rotation then used to send back 
to the client. This was done by sending another packet with the image size, operation, and the 
encrypted. This was then decoded in the client and then the image data was sent from the server
and used the size from the packet to get the image. 
Our intermediate submission was the same except had many bugs. 
We did not use AI tools to write code. 


