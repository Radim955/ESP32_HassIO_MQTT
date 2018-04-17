# Multifunkční senzor pohybu, teploty a vlhkosti ESP32, MQTT a HomeAssistant

Cílem tohoto projektu je za pomocí protokolu MQTT ovládat a vyčítat senzory umístěné na vývojové desce ESP32. Projekt vznikl v rámci Fakulty Informačních Technologií VUT v Brně, jako studentský projekt k předmětu PDS.

## Popis projektu

Jako MQTT Server (resp. Broker) bylo využito Raspberry PI 2 s běžícím HASSIO a běžící službou mosquitto MQTT. Home assistent bylo potřeba nakonfigurovat za použití konfiguračního skriptu configuration.yaml tak, aby umožňoval zobrazovat a nastavovat dostupné senzory pomocí webového rozhraní a protokolu MQTT na vývojové desce ESP32.

ESP32 poté funguje jako MQTT client (přes WIFI), který přispívá (publish), nebo odebírá (subscribe) údaje o senzorech, které implementuje. Konkrétně implementuje snímání teploty, vlhkosti a pohybu. Pomocí sejmutého pohybu a ovládání virtuálního tlačítka pak ovládá osvětlení - pro jednoduchost LED diodu.

## Zapojení

![alt text](https://github.com/Radim955/PDS-Project1/blob/master/img/Scheme.jpg)

## Použité součástky

- ESP32 Development module (MQTT client)
- Raspberry PI 2 model B   (MQTT server, Home Assistant)
- DHT11 (snímač teploty a vlhkosti)
- AS312(AM312) (detektor pohybu)
- LED dioda (osvětlení)
- 1K odpor (ochrana LED Diody)
- 2x zdroj 5V ~500mA (USB)

## Postup zprovoznění

1. Zprovoznění HassIO na Raspberry PI včetně mosquitto MQTT serveru a SSH. Stačí se držet návodu zde: https://www.home-assistant.io/getting-started/.
2. Instalace Arduino IDE a instalace použitých knihoven. Stačí se držet návodu zde: http://navody.arduino-shop.cz/navody-k-produktum/vyvojova-deska-esp32.html.
3. Nahrání konfigurace (config/configuration.yaml) pro Home Assistant na Raspberry PI za pomocí SSH do složky /config/.
4. Kompilace zdrojového kódu (source.ino) a nahrání tohoto Firmware do ESP32 za pomocí Arduino IDE. 

## Použité knihovny pro ESP32

- PubSubClient (MQTT Client). Dostupná zde: https://github.com/knolleary/pubsubclient.
- DHT11 library (Ovládání senzoru DHT11). Dostupná zde: https://github.com/adafruit/DHT-sensor-library.

## MQTT Protokol v projektu

Nejprve je potřeba se připojit k MQTT serveru. To se provádí pomocí ID zprávy Connect. Pro odpojení se od MQTT serveru pak pomocí zprávy Disconnect.

### Subscribe

Pomocí této zprávy se zaregistrujeme u MQTT Brokeru k odběru zpráv od ostatních zařízení v síti. Používáme k tomu tzv. Topic. V projektu byly použity následující:

- project/led/switch

Hodnota

### Publish

Pomocí této zprávy sdílíme informace MQTT Brokeru, který pak rozesílá tuto zprávu ostatním v síti, kteří náš topic odebírají. V projektu jsou sdíleny následující Topicy:

- project/temperature
- project/humidity
- project/motion
- project/led/status

### Hodnoty

S každou zprávou, která nese hodnoty se tedy odesílá Topic a samotná data. Data osvětlení se odesílají jako 0 nebo 1 (zhasnuto, rozsvíceno), data senzoru pohybu jako 0 nebo 1 (žádný pohyb, pohyb), data teploty a vlhkosti jako desetinná čísla (s desetinnou tečkou). 

## Upozornění
                                                                                                            
Dokumentace ve složce /doc je dokumentace veřejně dostupná z dostupných materiálů a slouží pro lepší zorientování se v použitých zařízeních. Pro aktuální verze najděte prosím dokumentaci na stránkách výrobce. Nejedná se o vlastní dílo!
