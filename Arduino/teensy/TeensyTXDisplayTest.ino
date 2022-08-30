const int ledPin = 13;

int count = 0;
const int num_strings = 6;

void setup() {
	// Serial1.setTxPin(1);
	Serial1.begin(9600);


	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, HIGH);
	delay(250);
	digitalWrite(ledPin, LOW);
}

void loop() {

	count++;
	count = count % 100;

	Serial1.print("c:");
	Serial1.print(count);
	Serial1.print(" ");


	int num = count % (num_strings + 1);
	switch (num) {
		case 1:
			Serial1.print("Roder-Koder-Sludrer!");
			break;
		case 2:
			Serial1.print("Hygger-Bygger-Lodder?");
			break;
		case 3:
			Serial1.print("Hygger-KODER-Bygger-SLUDRER-Lodder");
			break;
		case 4:
			Serial1.print("Radler");
			break;
		case 5:
			Serial1.print("ERORR, haha");
		default:
			Serial1.print(" ");
			break;
	}


	digitalWrite(ledPin, HIGH);
	delay(250);
	digitalWrite(ledPin, LOW);

	delay(10000);
}
