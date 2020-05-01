/*
Završni projekt: Slanje temperature e-mailom
*/

/* Koristen LM35 analogni senzior,  750mV=25°C, 10mV/°C */

int sensorValue;
int outputValue;
double outputValuetmp;
const int analogInPin = A0;  /*Pin Analognog senzor*/
/* Kreirane su globalne varijable koje se koriste za preuzimanje i obradu podataka sa senzora */


#include <SPI.h> 
/* This library allows you to communicate with 
SPI (Serial Peripheral Interface) devices, with the Arduino as the master device.  */

#include <Ethernet.h> 
/*This library is designed to work with the Arduino Ethernet Shield, 
Arduino Ethernet Shield 2, Leonardo Ethernet, and any other W5100/W5200/W5500-based devices. 
The library allows an Arduino board to connect to the Internet. 
The board can serve as either a server accepting incoming connections or a client making outgoing ones.*/

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xF8, 0xAB }; // MAC adresa Ethernet shielda
byte ip[] = { 161,53,168, 59};                              // IP adresa mreže
byte gateway[] = { 161, 53, 168, 1 }; 
/*A gateway is a piece of networking hardware used in telecommunications 
 * for telecommunications networks that allows data to flow from one discrete network to another. 
 * Gateways are distinct from routers or switches in that they communicate using more than one protocol 
 * and can operate at any of the seven layers of the open systems interconnection model (OSI).
The term gateway can also loosely refer to a computer or computer program configured to perform 
the tasks of a gateway, such as a default gateway or router.*/                      

byte subnet[] = { 255, 255, 255,128};    
/*A Subnet mask is a 32-bit number that masks an IP address, 
and divides the IP address into network address and host address.*/                 


#define time 1000 
#define emailInterval 60 // GDJE SE OVO KORISTI?

char message1[35], message2[35]; // GDJE SE OVO KORISTI?

IPAddress server(161,53,166,3);  /*IP adresa mail serverea  marjan.fesb.hr */

EthernetClient  client; 
/*Creates a client which can connect to a specified 
internet IP address and port (defined in the client.connect() function)*/

/* funkcija za slanje emaila */
void sendEmail()  
{ 
  Serial.println("spajanje na server..."); 
  
 if (client.connect(server,25)) 
   {   // spajanje na server na portu 25, smtp
    /*Connects to a specified IP address and port. 
    The return value indicates success or failure. Also supports DNS lookups when using a domain name.*/
   
    Serial.println("spojeno na server"); 
    // kod za slanje imaila (telnet naredbe)
    client.println("HELO fesb.hr");  delay(time);// Delay za 1000 mS
    client.println("MAIL FROM:<tperkovi@fesb.hr>");  delay(time);  
    client.println("RCPT TO:<tperkovi@fesb.hr>");   delay(time);   
    client.println("DATA");  delay(time);  
    /* data (optional): the data to print (char, byte, int, long, or string)*/
    client.println("Subject: poruka");  
    client.print(outputValuetmp); 
    /* Šalje se vrijednost temperature koja je spremljena u varijabli outputValuetemp*/
    client.println(" °C");  
    client.println(".");  
    client.println("QUIT");  
    client.stop(); /* Disconnect from the server.*/
     
   } else 
   { 
     Serial.println("veza nije uspjela!"); 
   } 
 
} 




void setup() 
{
   Ethernet.begin(mac, ip);  /* pocetak ethernet veze */
 Serial.begin(9600);   /* pocetak serijske veze */
 delay(1000);  /* pauza, da se veze stignu uspostaviti;*/
}


/* Ocitanje se vrsi svakih 5s */
void loop() {

/* Ocitanje i ispis temperature senzora */
  sensorValue = analogRead(analogInPin); /* Vrijednost sa senzora u mV */
/*mapiranje vrijednosti sa AD pretvaraca u mV */
  outputValue = map(sensorValue, 0, 1023, 0, 5000); 
/* Pretvaram naponsku vrijednsot senzora u celzijeve stupnejve */

  outputValuetmp = (double)(outputValue-500)/10.0;
 /*Sada trebam outputValue pretvoriti u string i poslati mailom */
 sendEmail();   
 /*pozivanje funkcije za slanje emaila*/

/*ocitanje se izvrsava svakih 5s */
  delay(5000);

/*mikrokontroler ne moze nista drugo izvrsavati ovih 5s!! */
}
