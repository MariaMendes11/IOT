void handleAlarme(AdafruitIO_Data *data){
  String valor = data -> toString();

  Serial.print(F("Valor recebido do feed: "));
  Serial.println(valor);

  if(valor == "true"){
    alarmeAtivo = true;
    Serial.println("Alerme ARMADO pelo dash / app");
  }
  else{
    alarmeAtivo = false;
    Serial.println("Alerme DESARMADO pelo dash / app");
  }
}

void ativarAlarme (){
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_ALARME, HIGH);
}

void desligarAlarme (){
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_ALARME, LOW);
}