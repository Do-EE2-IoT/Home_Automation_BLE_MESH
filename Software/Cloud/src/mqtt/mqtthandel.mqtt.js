import ErrorResponse from "../helpers/errorHandle.response.js";
import newDeviceModel from "../models/new.model.js";
import RGBModel from "../models/rgb.model.js";
import SensorModel from "../models/sensor.model.js";
import SirenModel from "../models/siren.model.js";
import EncoderModel from "../models/encoder.model.js";
import DoorModel from "../models/door.model.js";
import userModel from "../models/user.model.js";

const mqttHandle = async (topic, data) => {
  console.log(topic);
  console.log(data);
  if (topic === "/device/resgateway") {
    const updateGateway = await userModel.findOneAndUpdate(
      { _id: data.clientID },
      {
        gateway: data.mac,
      }
    );

    if (!updateGateway) {
      console.log("Cannot find gateway ?");
    } else {
      console.log("Update gateway for user name ::", updateGateway.name);
    }
  } else if (topic === "/device/detect") {
    const detectedDevice = await newDeviceModel.create({
      type: data.type,
      uuid: data.uuid,
      mac: data.mac,
      user: data.clientID,
    });
    if (!detectedDevice) {
      console.log("Cannot update detected device");
    }
  } else if (topic === "/device/rgb") {
    const newRGB = await RGBModel.create({
      name: "RGB",
      user: data.clientID,
      address: data.address,
    });

    if (!newRGB) {
      console.log("Cannot add RGB for user");
    } else {
      console.log("Successfully add RGB device for user name ::", newRGB.user);
    }
  } else if (topic === "/device/sensor") {
    const newSensor = await SensorModel.create({
      name: "Sensor",
      user: data.clientID,
      address: data.address,
    });

    if (!newSensor) {
      console.log("Cannot add Sensor for user");
    } else {
      console.log("Successfully add Sensor device for user name ::", newSensor.user);
    }
  } else if (topic === "/device/siren") {
    const newSiren = await SirenModel.create({
      name: "Siren",
      user: data.clientID,
      address: data.address,
    });

    if (!newSiren) {
      console.log("Cannot add Siren for user");
    } else {
      console.log("Successfully add Siren device for user name ::", newSiren.user);
    } 
  } else if (topic === "/device/door") {
    const newDoor = await DoorModel.create({
      name: "Door",
      user: data.clientID,
      address: data.address,
    });

    if (!newDoor) {
      console.log("Cannot add Door for user");
    } else {
      console.log("Successfully add Door device for user name ::", newDoor.user);
    } 
  } else if (topic === "/device/encoder") {
    const newEncoder = await EncoderModel.create({
      name: "Encoder",
      user: data.clientID,
      address: data.address,
    });

    if (!newEncoder) {
      console.log("Cannot add Encoder for user");
    } else {
      console.log("Successfully add Encoder device for user name ::", newEncoder.user);
    } 
  } 
};
export { mqttHandle };
