void RadioSetup() {
  RADIO.begin();                 // активировать модуль
  RADIO.setAutoAck(1);           // режим подтверждения приёма, 1 вкл 0 выкл
  RADIO.setRetries(0, 15);       // время между попыткой достучаться, число попыток
  RADIO.enableAckPayload();      // разрешить отсылку данных в ответ на входящий сигнал
  RADIO.setPayloadSize(32);      // размер пакета, в байтах
  RADIO.setChannel(CH_NUM);      // выбираем канал (в котором нет шумов!)
  RADIO.setPALevel(1);   // уровень мощности передатчика
  RADIO.setDataRate(SIG_SPEED);  // скорость обмена, должна быть одинакова на приёмнике и передатчике! При самой низкой скорости имеем самую высокую чувствительность и дальность!!
  RADIO.powerUp();               // начать работу
}
void Radio_TX_RX(uint8_t rx) {                // отправка пакета transmit_data и получеение телеметрии от приемника под номером rx
  RADIO.stopListening();                      // выключить прослышивание эфира
  RADIO.openWritingPipe(address[rx]);         // отправка будет проихводится от имени address[rx]
  if (RADIO.write(&transmit_data, sizeof(transmit_data))) {    //отправляем массив данных и если дошло....
    rx_connect[rx] = 1;
    if (!RADIO.available()) {                 // если получаем пустой ответ
    } else {
      while (RADIO.available() ) {            // если в ответе что-то есть
        RADIO.read(&telemetry[rx], 3);        // читаем // получили забитый данными telemetry ответа от приёмника
      }
    }
  } else {
     rx_connect[rx] = 0;
  }
  RADIO.startListening();
}
