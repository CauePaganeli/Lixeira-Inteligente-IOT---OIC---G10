# Lixeira IoT - Grupo 10

## Integrantes

- Cauê Tanigawa Paganeli
- Matheus Hypolito Galhardi dos Santos
- Rodrigo Soriani Trajano de Santana

## Descrição

Projeto desenvolvido para a disciplina Objetos Inteligentes Conectados.

O sistema monitora o nível de preenchimento de uma lixeira utilizando um sensor ultrassônico HC-SR04 conectado a um NodeMCU ESP8266.

As informações são transmitidas via protocolo MQTT para o Node-RED, onde são apresentadas em um dashboard de monitoramento em tempo real.


## Tecnologias Utilizadas

- NodeMCU ESP8266
- Sensor HC-SR04
- LED
- MQTT
- HiveMQ Broker
- Node-RED
- Arduino IDE

## Funcionamento

1. Sensor mede a distância.
2. NodeMCU calcula o status.
3. Dados são enviados via MQTT.
4. Node-RED recebe os dados.
5. Dashboard é atualizado em tempo real.

## Estrutura MQTT

Tópico:

grupo10/lixeira/nivel

Exemplo de mensagem:

```json
{
  "distancia_cm": 16.92,
  "status": "PARCIAL"
}
```

## Vídeo

Link disponível em:

video/link-video.txt

## Documentação

Disponível na pasta:

documentacao/

## Licença

Projeto acadêmico.
