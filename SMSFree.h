/*
 * SMSFree.h - Biblioteca de envio de SMS grátis
 * Copyright (c) 2020 Ernâni Santos
 * All rights reserved.
*/
#ifndef SMSFREE_H
#define SMSFREE_H

#include <regex.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

class SMSFree
{
private:
    WiFiClient client;
    HTTPClient http;
    String cookie;
    unsigned int maxlength = 130;
    bool _debug = true;
    bool send(const String _number, String _message, unsigned int _maxcount);
    template <typename Generic>
    void DEBUG_SF(Generic text);

public:
    SMSFree();
    ~SMSFree();
    bool auth(const char *_username, const char *_password);
    bool put(const String _dest, String _msg);
    void debugOutput(bool debug);
};

#endif