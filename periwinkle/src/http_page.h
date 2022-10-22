const char PAGE_FirstConfiguration[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <title>%HTTP_TITLE%</title>
    </head>
    <body>
        <form method="post" action="config">
            <table>
                <tr>
                    <td>NETWORK CONFIG</td>
                </tr>
                <tr>
                    <td>WiFi SSID</td>
                    <td><input type="text" name="wifi_ssid"></td>
                </tr>
                <tr>
                    <td>WiFi Password</td>
                    <td><input type="text" name="wifi_password"></td>
                </tr>
                <input type="hidden" name="wifi_mode" value="true">
                <tr>
                    <td>SYSTEM CONFIG</td>
                </tr>
                <tr>
                    <td>System Name</td>
                    <td><input type="text" name="device_name"></td>
                </tr>
                <tr>
                    <td>System Auth Key</td>
                    <td><input type="text" name="token"></td>
                </tr>
                <tr>
                    <td>Server Host</td>
                    <td><input type="text" name="server_host"></td>
                </tr>
                <tr>
                    <td>Server Token</td>
                    <td><input type="text" name="server_token"></td>
                </tr>
                <tr>
                    <td></td>
                    <td><input type="submit" name="Save"></td>
                </tr>

            </table>
        </form>
    </body>
</html>
)=====";