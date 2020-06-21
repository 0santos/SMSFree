/*
 * SMSFree.h - Biblioteca de envio de SMS grátis
 * Copyright (c) 2020 Ernâni Santos
 * All rights reserved.
*/
#include <SMSFree.h>

SMSFree::SMSFree() {}
SMSFree::~SMSFree() {}

static const char *LOCATION = "Location";
static const char *SET_COOKIE = "Set-Cookie";
static const char *HEADER_NAMES[] = {LOCATION, SET_COOKIE};

bool SMSFree::auth(const char *_username, const char *_password)
{
    DEBUG_SF(F("[HTTP] Auth method begin"));
    String uri;
    {
        http.begin(client, "http://www.cvmovel.cv/sites/all/themes/cvmovel/int/checklogin.php");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        http.addHeader("Connection", "keep-alive");
        http.collectHeaders(HEADER_NAMES, 2);
        DEBUG_SF(F("[HTTP] Request Method: POST"));
        DEBUG_SF(_username);
        DEBUG_SF(_password);
        int httpCode = http.POST("sso_username=" + String(_username) + "&sso_password=" + String(_password) + "&Submit=Entrar");
        if (httpCode != 302 || !http.hasHeader(SET_COOKIE))
        {
            DEBUG_SF(F("[HTTP] ERROR: Status code"));
            DEBUG_SF(httpCode);
            return false;
        }
        uri = http.header(LOCATION);
        cookie = http.header(SET_COOKIE);
        DEBUG_SF(F("[HTTP] Request redirected to"));
        DEBUG_SF(uri);
        DEBUG_SF(F("[HTTP] Response cookie"));
        DEBUG_SF(cookie);
        delay(2000);
    }
    {
        DEBUG_SF(F(""));
        DEBUG_SF(F("[HTTP] Redirected begin"));
        http.begin(client, uri);
        http.collectHeaders(HEADER_NAMES, 2);
        int httpCode = http.GET();
        if (httpCode != 200 || !http.hasHeader(SET_COOKIE))
        {
            DEBUG_SF(F("[HTTP] ERROR: Status code"));
            DEBUG_SF(httpCode);
            return false;
        }
        const String &payload = http.getString();

        cookie = http.header(SET_COOKIE);
        http.end();
        DEBUG_SF(F("[HTTP] Response cookie"));
        DEBUG_SF(cookie);
        delay(500);

        return payload.indexOf(_username) != -1;
    }
}

bool SMSFree::send(const String _number, String _message, unsigned int _maxcount)
{
    DEBUG_SF(F("[HTTP] Send method begin"));
    String result;
    {
        http.begin(client, "http://www.cvmovel.cv/envio-de-mensagens-escritas");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        http.addHeader("Cookie", cookie.c_str());
        http.collectHeaders(HEADER_NAMES, 2);
        DEBUG_SF(F("[HTTP] Request Method: POST"));
        DEBUG_SF(_number);
        DEBUG_SF(_message);
        DEBUG_SF(_maxcount);
        int httpCode = http.POST("numero=" + _number + "&freeSms=" + _message + "&left=" + _maxcount);
        if (httpCode != 302 || !http.hasHeader(LOCATION))
        {
            DEBUG_SF(F("ERROR: Status code"));
            DEBUG_SF(httpCode);
            return false;
        }
        result = http.header(LOCATION);
        http.end();
        DEBUG_SF(F("[HTTP] Response result"));
        DEBUG_SF(result);
        delay(500);

        return result.indexOf("ok") != -1;
    }
}

/*
 * Match string against the extended regular expression in
 * pattern, treating errors as no match.
 *
 * return 1 for match, 0 for no match
 */

int match(const char *str)
{
    int status;
    regex_t re;

    if (regcomp(&re, "^((95|97|98|99|59|58)\[0-9]{5})$", REG_EXTENDED) != 0)
    {
        return 0; /* report error */
    }
    status = regexec(&re, str, (size_t)0, NULL, 0);
    regfree(&re);
    if (status != 0)
    {
        return 0; /* report error */
    }
    return 1;
}

bool SMSFree::put(const String _dest, String _msg)
{
    if (_msg.length() == 0)
    {
        DEBUG_SF(F("Message is empty"));
        return false;
    }

    if (_msg.length() > maxlength)
        _msg = _msg.substring(0, maxlength);
    else
        maxlength = maxlength - _msg.length();

    _msg.replace(" ", "+");
    _msg.trim();

    if (match(_dest.c_str()) != 1)
    {
        DEBUG_SF(F("Invalid phone number"));
        return false;
    }

    DEBUG_SF(_dest);
    DEBUG_SF(_msg);
    DEBUG_SF(maxlength);
    if (send(_dest.c_str(), _msg.c_str(), maxlength) == true)
    {
        DEBUG_SF(F("OK: Message sent successfully"));
        return true;
    }
    else
    {
        DEBUG_SF(F("ERROR: Sending message"));
        return false;
    }
}

void SMSFree::debugOutput(bool debug)
{
    _debug = debug;
}

template <typename Generic>
void SMSFree::DEBUG_SF(Generic text)
{
    if (_debug)
    {
        Serial.print("*SF: ");
        Serial.println(text);
    }
}