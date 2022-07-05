#include <mcp_can.h>
#include <SPI.h>
#define CAN0_INT 2

unsigned long timer = 500;
unsigned long last_time = 0;
unsigned char len = 0;
unsigned char rxbuf[8];
long unsigned int rxId;
char msgString[128];
const int SPI_CS_PIN = 17;
MCP_CAN CAN0(17);

uint16_t crc16_big( uint8_t * data_p , uint8_t length){
  uint8_t i = 0, j = 0;
  uint16_t crc = 0xffff;
  for ( i = 0 ; i<length ; i++ , data_p++ ){
    crc = (( uint16_t ) * data_p) ^ crc;
    for ( j = 0 ; j < 8 ; j++ )
    {
      if (crc & 0x0001) crc = (crc >> 1) ^ 0xA001 ;
      else crc = crc >> 1;   
    }} 
    return (crc);
} 
unsigned char data[4] = { 0x09 , 0x81 , 0x01 , 0x00 } ;
//unsigned char data1[8] = { 0x00 , 0x00 , crc16_big(data,8 ) } ;


void sendMsg(){
  if (millis() - last_time > timer) { last_time = millis(); 
    CAN0.sendMsgBuf( 0x001 , 8 , data ); //Send 1 <<
}}

void lenId(){
  if((rxId & 0x80000000) == 0x80000000)  
    sprintf(msgString, "Id : 0x%.8lX  Dlc : %1d  Data : ", (rxId & 0x1FFFFFFF ) , len );
  else
    sprintf ( msgString , " Id : 0x%.3lX  Dlc : %1d  Data : ", rxId , len );
Serial.print( msgString );
}
void RRF(){
 if((rxId & 0x40000000) == 0x40000000 ){
    sprintf(msgString, " REMOTE REQUEST FRAME");
  Serial.print(msgString);
}else{
for ( byte i = 0; i<len; i++ ) {
  Serial.print(rxbuf[i], HEX);
  Serial.print(" ");  
}}}

void ReadMsg(){ CAN0.readMsgBuf(&rxId, &len, rxbuf);}

void setup() {
  Serial.begin (115200); 
  pinMode ( CAN0_INT ,INPUT );
  while ( CAN_OK != CAN0.begin ( MCP_ANY , CAN_250KBPS , MCP_16MHZ )){
    Serial.print ( "CANBUS FAIL" );
    delay (100);  
}}
void loop(){
  sendMsg();
  ReadMsg();
  lenId();

  RRF();
     
  Serial.print("\n");
  delay(500);     
}


