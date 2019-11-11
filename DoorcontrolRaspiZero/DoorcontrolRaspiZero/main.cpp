#include <wiringPi.h>
#include <time.h>
#include <ds18b20.h> 
#include "global.h"
#include <string>
#include <cstring>
#include <ctime>
#include "mqtt/async_client.h"

using namespace std;
using namespace std::chrono;

#define	OUT1	4	// header 16 = wiring pi pin 4 = GPIO4 = BCM GPIO 23
#define	OUT2	5	// header 18 = wiring pi pin 5 = GPIO5 = BCM GPIO 24
#define	OUT3	10	// header 24 = wiring pi pin 10 = CE0 = BCM GPIO 8
#define	OUT4	11	// header 26 = wiring pi pin 11 = CE1 = BCM GPIO 7
#define LED		0

const std::string DFLT_ADDRESS{ "tcp://192.168.1.116:1883" };
const string TOPIC{ "data" };
const int	 QOS = 1;
//const auto PERIOD = seconds(5);
const int MAX_BUFFERED_MSGS = 120;	// 120 * 5sec => 10min off-line buffering
const string PERSIST_DIR{ "data-persist" };
const auto PERIOD = seconds(5);
const char* PAYLOADS[] = {
	"Hello World!",
	"Hi there!",
	"Is anyone listening?",
	"Someone is always listening.",
	nullptr
};


time_t tTimestampNow;
struct tm actTime;

//door myDoor;
param myParams;

int main(void)
{
	wiringPiSetup();
	pinMode(LED, OUTPUT);
	pinMode(OUT1, OUTPUT);
	pinMode(OUT2, OUTPUT);
	pinMode(OUT3, OUTPUT);
	pinMode(OUT4, OUTPUT);
	myParams.morningTime = 730;
	myParams.lunchTime = 1100;
	myParams.eveningTime = 1700;
	myParams.nighTime = 2030;

	string address = DFLT_ADDRESS;

	mqtt::async_client cli(address, "");

	cout << "  ...OK" << endl;

	try {
		cout << "\nConnecting..." << endl;
		mqtt::connect_options connOpts;
		connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
		connOpts.set_clean_session(true);
		connOpts.set_automatic_reconnect(true);
		connOpts.set_user_name("hassbroker");
		connOpts.set_password("hassio");

		cli.connect(connOpts)->wait();
		cout << "  ...OK" << endl;

		cout << "\nPublishing messages..." << endl;

		mqtt::topic top(cli, "test", QOS);
		mqtt::token_ptr tok;

		size_t i = 0;
		while (PAYLOADS[i]) {
			tok = top.publish(PAYLOADS[i++]);
		}
		tok->wait();	// Just wait for the last one to complete.
		cout << "OK" << endl;

		// Disconnect
		cout << "\nDisconnecting..." << endl;
		cli.disconnect()->wait();
		cout << "  ...OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}


	while (true)
	{
		time(&tTimestampNow);
		actTime = *localtime(&tTimestampNow);
		/*
		digitalWrite(OUT1, HIGH);  // Ein
		delay(100); // ms
		digitalWrite(OUT1, LOW);	  // Aus
		delay(100);

		digitalWrite(OUT2, HIGH);  // Ein
		delay(100); // ms
		digitalWrite(OUT2, LOW);	  // Aus
		delay(100);

		digitalWrite(OUT3, HIGH);  // Ein
		delay(100); // ms
		digitalWrite(OUT3, LOW);	  // Aus
		delay(100);
		
		digitalWrite(OUT4, HIGH);  // Ein
		delay(1000); // ms
		digitalWrite(OUT4, LOW);	  // Aus
		delay(1000);
		*/
		
	}
	return 0;
}

