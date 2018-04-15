# Multifunkční senzor pohybu, teploty a vlhkosti ESP32, MQTT a HomeAssistant

Cílem tohoto projektu je za pomocí protokolu MQTT ovládat a vyčítat senzory umístěné na vývojové desce ESP32. Projekt vznikl v rámci Fakulty Informačních Technologií VUT v Brně, jako studentský projekt k předmětu PDS.

## Popis projektu

Jako MQTT Server (resp. Broker) bylo využito Raspberry PI 2 s běžícím HASSIO a běžící službou mosquitto MQTT. Home assistent bylo potřeba nakonfigurovat za použití konfiguračního skriptu configuration.yaml tak, aby umožňoval zobrazovat a nastavovat dostupné senzory pomocí webového rozhraní a protokolu MQTT na vývojové desce ESP32.
ESP32 poté funguje jako MQTT client (přes WIFI), který přispívá (publish), nebo odebírá (subscribe) údaje o senzorech, které implementuje. Konkrétně implementuje snímání teploty, vlhkosti a pohybu. Pomocí sejmutého pohybu a ovládání virtuálního tlačítka pak ovládá osvětlení - pro jednoduchost LED diodu.

## Zapojení

![alt text](https://raw.githubusercontent.com/xtruhl05/ledstrip/master/img/idea.png?token=AOIdCNBmrx-8uNJY8GmIB9fOM35mvm45ks5a1wiewA%3D%3D)

## Použité součástky

- ESP32 Development module (MQTT client)
- Raspberry PI 2 model B   (MQTT server)
- DHT11 (snímač teploty a vlhkosti)
- AS312(AM312) (detektor pohybu)
- LED dioda (osvětlení)
- 1K odpor (ochrana LED Diody)
- 2x zdroj 5V ~500mA (USB)

## Postup spuštění

1. Zprovoznění HassIO na Raspberry PI. Stačí se držet návodu zde: https://www.home-assistant.io/getting-started/.
2. Instalace Arduino IDE a instalace použitých knihoven. Stačí se držet návodu zde: http://navody.arduino-shop.cz/navody-k-produktum/vyvojova-deska-esp32.html.
3. Nahrání konfigurace pro Home Assistant na Raspberry PI za pomocí SSH přístupu.
4. Kompilace zdrojového kódu source.ino a nahrání tohoto Firmware do ESP32 za pomocíé Arduino IDE. 

## Použité knihovny

- PubSubClient (MQTT Client). Dostupná zde: https://github.com/knolleary/pubsubclient.
- DHT11 library (Ovládání senzoru DHT11). Dostupná zde: https://github.com/adafruit/DHT-sensor-library.