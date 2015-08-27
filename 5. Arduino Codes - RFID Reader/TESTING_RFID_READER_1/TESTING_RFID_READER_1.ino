#include <SoftwareSerial.h>
SoftwareSerial softwareSerial(2, 3);
const int tagsAmountOf 					= 2;
const int bytesNetto 					= 12;
int bytesBrutto 						= bytesNetto + 4;
int tagsKnown[tagsAmountOf][bytesNetto]	= {
	{54, 65, 48, 48, 52, 65, 48, 66, 54, 57, 52, 50},
	{00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}
};
int 	tagUnknown[bytesNetto];
char 	tagUnknownChar[bytesNetto];
void setup(){
	Serial.begin(9600);
	softwareSerial.begin(9600);
}
void loop(){
	bool tagRead 		= false;
	int tagByteCounter 	= 0;
	int byteRead;
	if(softwareSerial.available() == bytesBrutto){
		tagRead = true;
	}
	if(tagRead){
		Serial.print("Card serial number: ");
		while(softwareSerial.available() > 0){
			byteRead = softwareSerial.read();
			if(
				byteRead != 2
				&& byteRead != 3
				&& byteRead != 10
				&& byteRead != 13
			){
				tagUnknown[tagByteCounter] = byteRead;
				tagUnknownChar[tagByteCounter] = byteRead;
				tagByteCounter ++;
				Serial.print(byteRead);
				if(tagByteCounter != bytesNetto){ Serial.print(" "); }
			}
			if(byteRead == 3){
				tagRead = false;
				Serial.println(".");
			}
		}
	}
	if(strlen(tagUnknownChar) == 0){
		return;
	}
	else{
		bool isTagRegistered;
		int i = 0;
		int j = 0;
		int bytesMatchCounter = 0;
		int lastMatchedIndex;
		while(1){
			if(tagUnknown[j] == tagsKnown[i][j]){
				j ++;
				bytesMatchCounter ++;
			}
			else if(tagUnknown[j] != tagsKnown[i][j]){
				i ++;
				j = 0;
				bytesMatchCounter = 0;
			}
			if(bytesMatchCounter == bytesNetto){
				isTagRegistered = true;
				break;
			}
			else if(i == tagsAmountOf - 1){
				isTagRegistered = false;
				break;
			}
		}
		if(isTagRegistered){
			lastMatchedIndex = i + 1;
			Serial.print("This card index is ");
			Serial.print(lastMatchedIndex);
			Serial.println(".");
		}
		else if(!isTagRegistered){
			Serial.println("This card index is not registered.");
		}
		for(int i = 0; i < bytesNetto; i ++){
			tagUnknownChar[i] = 0;
		}
	}
}