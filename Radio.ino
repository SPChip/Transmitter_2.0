void RadioSetup() {
  RADIO.begin();              // активировать модуль
  RADIO.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  RADIO.setRetries(0, 5);    // (время между попыткой достучаться, число попыток)
  RADIO.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  RADIO.setPayloadSize(64);   // размер пакета, в байтах
  RADIO.openWritingPipe(address[0]);   // мы - труба 0, открываем канал для передачи данных
  RADIO.setChannel(CH_NUM);            // выбираем канал (в котором нет шумов!)
  RADIO.setPALevel(SIG_POWER);         // уровень мощности передатчика
  RADIO.setDataRate(SIG_SPEED);        // скорость обмена
  // должна быть одинакова на приёмнике и передатчике!
  // при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  RADIO.powerUp();         // начать работу
  RADIO.stopListening();   // не слушаем радиоэфир, мы передатчик
}
void Radio_TX_RX() {                // отправка пакета transmit_data и получеение телеметрии
  if (RADIO.write(&transmit_data, sizeof(transmit_data))) {
    trnsmtd_pack++;
    if (!RADIO.available()) {   // если получаем пустой ответ
    } else {
      while (RADIO.available() ) {                    // если в ответе что-то есть
        RADIO.read(&telemetry, sizeof(telemetry));    // читаем
        // получили забитый данными массив telemetry ответа от приёмника
      }
    }
  } else {
    failed_pack++;
  }
}
