#define commIntPin 12
#define commPort Serial4

static int recBufferSize = 256;

void commsInit();

void recTransmission();

void transmissionError(string errorMsg);
void sendTransmission(string msg);
void readSerial(char recBuffer[recBufferSize], int &index);