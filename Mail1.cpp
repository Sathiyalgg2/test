// From teh site: http://forum.arduino.cc/index.php?topic=67701.15
// Then this: http://forum.arduino.cc/index.php?topic=67701.0

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

byte MAC[]={0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};  // Your MAC
File picture; EthernetClient client;

static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
void encodeblock(unsigned char in[3],unsigned char out[4],int len) {
 out[0]=cb64[in[0]>>2]; out[1]=cb64[((in[0]&0x03)<<4)|((in[1]&0xF0)>>4)];
 out[2]=(unsigned char) (len>1 ? cb64[((in[1]&0x0F)<<2)|((in[2]&0xC0)>>6)] : '=');
 out[3]=(unsigned char) (len>2 ? cb64[in[2]&0x3F] : '=');
}
void encode() {
 unsigned char in[3],out[4]; int i,len,blocksout=0;
 while (picture.available()!=0) {
   len=0; for (i=0;i<3;i++) { in[i]=(unsigned char) picture.read(); if (picture.available()!=0) len++; else in[i]=0; }
   if (len) { encodeblock(in,out,len); for(i=0;i<4;i++) client.write(out[i]); blocksout++; }
   if (blocksout>=19||picture.available()==0) { if (blocksout) client.print("\r\n");  blocksout=0; }
 }
}

void setup() {
 Ethernet.begin(MAC); delay(2000);
 pinMode(10,OUTPUT); SD.begin(4);
}

void loop() {
 picture=SD.open("picture.jpg",FILE_READ);
 client.connect("smtp.mail.yahoo.com",25);
 client.print("HELO\nAUTH PLAIN\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");  // x: base64 encoded login: \0foo@yahoo.com\0password
 client.print("MAIL FROM:<foo@yahoo.com>\nRCPT TO:<bar@fakemail.com>\nDATA\nFrom: \"Foo\" <foo@yahoo.com>\r\nTo: bar@fakemail.com\r\nSubject: Picture attached\r\n");
 client.print("Content-Type: image/jpeg; name=\"picture.jpg\"\r\nContent-Disposition: attachment; filename=\"picture.jpg\"\r\nContent-Transfer-Encoding: base64\r\n\r\n");
 encode();
 client.print("\r\n.\r\nQUIT\n");
 client.stop();
  picture.close();
 while(1);
}
