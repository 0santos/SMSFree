#include <SMSFree.h>

SMSFree sms;

/* User and Password from CVMovel Mobile Network */
const char *_username = "";
const char *_password = "";

const String _to = "";                  // Destinaction phone number
String _message = "Hello from ESP8266"; // Max character = 130

/* Set these to your desired credentials */
const char *ssid = "yourSSid";
const char *password = "yourPassword";

void setup()
{
    Serial.begin(115200);
    //sms.debugOutput(false);

    Serial.println();
    Serial.println();
    Serial.println();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) // Connect to your WiFi router
    {
        delay(500);
        Serial.print(".");
    }
    // If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());

    //=======================================================================
    //                            Main Program
    //=======================================================================

    Serial.println("Connecting to Gateway Server ...");
    if (!sms.auth(_username, _password))
    {
        Serial.print("Authentification failed, check username and password\n");
        return;
    }
    Serial.println("Authentification Successful");

    Serial.println("Sending Free SMS ...");
    if (!sms.put(_to, _message))
    {
        Serial.print("Error\n");
        return;
    }
    Serial.print("Ok\n");
}

void loop()
{
    // put your main code here, to run repeatedly:
}