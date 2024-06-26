import mqtt from "mqtt";
import dotenv from "dotenv";
import { mqttHandle } from "./mqtthandel.mqtt.js";
dotenv.config({ path: "./config.env" });

dotenv.config();
var mqttClient;

function mqttBrokerInit() {
  const hostURL = `mqtt://${process.env.MQTT_HOST}:${process.env.MQTT_PORT}`;
  const options = {
    keepalive: 60,
    protocolID: "MQTT",
    protocolVersion: 4,
    clean: true,
    reconnectPeriod: 1000,
    connectTomeout: 30 * 1000,
  };

  mqttClient = mqtt.connect(hostURL, options);

  mqttClient.on("reconnect", () => {
    console.log(hostURL);
    console.log("Reconnecting...");
  });

  mqttClient.on("connect", () => {
    console.log("MQTT Client connected");
  });

  mqttClient.on("message", async (topic, message, packet) => {
    console.log(
      "Received Message: " + message.toString() + "\nOn topic: " + topic
    );
    if (JSON.parse(message.toString()).clientID == "") {
      console.log("Lack of clientID");
      return;
    }
    await mqttHandle(topic, JSON.parse(message.toString()));
  });
}

export { mqttBrokerInit, mqttClient };
