Project group number: 84
Group member names and x500s: 
	Justin Pederson: pede0831@umn.edu, server.c
	Chiemeka Nwakama: nwaka013@umn.edu, send_file and main
	Ibrahim Ismail-Adebiyi: ismai128@umn.edu, receive_file
CSELabs computer the code was tested on: csel-kh1250-01.cselabs.umn.edu
We did not make any changes to the makefile.
We are going to construct the client handling thread by making the critical 
section before we bind to the socket of the port and after clientHandler
in main. This is because we don't want the information in these sections to 
be mixing together. This would cause multiple different clients to send data
to the server and then they would want to go into the clientHandler and they 
could have the wrong data for the thread to complete the rotation. We plan on
sending packages by first sending a packet with the operation, flags, and image size
we will recieve this, extract values and then we will recieve the image data. We will
then use the information from the packet that we got to perform the proper operation 
on the image/image data and then send back the modifed image data per the client's 
request.


