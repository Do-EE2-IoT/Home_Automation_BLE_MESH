import ErrorResponse from "../helpers/errorHandle.response.js";
import newDeviceModel from "../models/new.model.js";
import RGBModel from "../models/rgb.model.js";
import SensorModel from "../models/sensor.model.js";
import SirenModel from "../models/siren.model.js";
import EncoderModel from "../models/encoder.model.js";
import DoorModel from "../models/door.model.js";
import userModel from "../models/user.model.js";
import publishTopic from "../mqtt/publish.mqtt/index.js";
import { subscribeTopic } from "../mqtt/subscribe.mqtt/index.js";
import baseResponse from "../helpers/respond.js";

class DeviceService {
  addMyGateway = async (Gateway) => {
    publishTopic(
      `/device/addgateway/${Gateway.macGateway}`,
      1,
      JSON.stringify({
        mac: Gateway.macGateway,
        clientID: Gateway.clientID,
      })
    );

    return baseResponse(
      null,
      200,
      "You are successfully send request add gateway"
    );
  };
  getDetectedDevice = async (id) => {
    const detectedDevice = await newDeviceModel.find({ user: id });
    return baseResponse(
      detectedDevice,
      200,
      "get detected devices successfully"
    );
  };

  addNewDevice = async (Infor) => {
    const User = await userModel.findById(Infor.clientID);
    if (!User) {
      throw new ErrorResponse("You are not belong to this page", 404);
    }
    publishTopic(
      `/device/addnewdevice/${User.gateway}`,
      1,
      JSON.stringify({
        uuid: Infor.uuid,
        mac: Infor.mac,
      })
    );
    const data = Infor.type;
    return baseResponse(data, 200, "Send request add device successfully");
  };

  rgbInfor = async (Infor) => {
    const rgb = await RGBModel.findOne({
      user: Infor.clientID,
      address: Infor.RGBAddress,
    });
    console.log(Infor.RGBAddress);

    if (!rgb) {
      return baseResponse(null, 404, "This RGB is not belong to you");
    }

    return baseResponse(
      rgb,
      200,
      `get rgb with ID ${rgb.address} successfully`
    );
  };

  rgbControl = async (Infor) => {
    const user = await userModel.findById(Infor.clientID);
    if (!user) {
      return baseResponse(null, 403, "User exit");
    }
    if (Infor.groupAddress) {
      console.log(Infor.groupAddress);
      const rgb = await RGBModel.findOne({
        user: Infor.clientID,
        Group: { $in: Infor.groupAddress },
      });
      //  console.log(Infor.groupAddress);

      if (!rgb) {
        return baseResponse(null, 404, "This RGB not belong to you");
      }
      const payload = {
        address: Infor.groupAddress,
        red: Infor.red,
        blue: Infor.blue,
        green: Infor.green,
      };
      publishTopic(
        `/device/rgb/control/${user.gateway}`,
        1,
        JSON.stringify(payload)
      );
    } else {
      const rgb = await RGBModel.findOne({
        user: Infor.clientID,
        address: Infor.unicastAddress,
      });
      console.log(Infor.unicastAddress);

      if (!rgb) {
        return baseResponse(null, 404, "This RGB not belong to you");
      }
      const payload = {
        address: Infor.unicastAddress,
        red: Infor.red,
        blue: Infor.blue,
        green: Infor.green,
      };
      publishTopic(
        `/device/rgb/control/${user.gateway}`,
        1,
        JSON.stringify(payload)
      );
    }
    return baseResponse(
      null,
      200,
      "Send RGB control successfully, wait for respond"
    );
  };

  sensorInfor = async (Infor) => {
    const sensor = await SensorModel.findOne({
      user: Infor.clientID,
      address: Infor.SensorAddress,
    });
    console.log(Infor.SensorAddress);

    if (!sensor) {
      return baseResponse(null, 404, "This Sensor is not belong to you");
    }

    return baseResponse(
      sensor,
      200,
      `get sensor with ID ${sensor.address} successfully`
    );
  };

  sirenInfor = async (Infor) => {
    const siren = await SirenModel.findOne({
      user: Infor.clientID,
      address: Infor.SirenAddress,
    });
    console.log(Infor.SirenAddress);

    if (!siren) {
      return baseResponse(null, 404, "This Siren is not belong to you");
    }

    return baseResponse(
      siren,
      200,
      `get sensor with ID ${siren.address} successfully`
    );
  };

  encoderInfor = async (Infor) => {
    const encoder = await EncoderModel.findOne({
      user: Infor.clientID,
      address: Infor.EncoderAddress,
    });
    console.log(Infor.EncoderAddress);

    if (!encoder) {
      return baseResponse(null, 404, "This Encoder is not belong to you ");
    }

    return baseResponse(
      encoder,
      200,
      `get encoder with ID ${encoder.address} successfully`
    );
  };

  doorInfor = async (Infor) => {
    const door = await DoorModel.findOne({
      user: Infor.clientID,
      address: Infor.DoorAddress,
    });
    console.log(Infor.DoorAddress);

    if (!door) {
      return baseResponse(null, 404, "This Door is not belong to you");
    }

    return baseResponse(
      door,
      200,
      `get door with ID ${door.address} successfully`
    );
  };
}

export default new DeviceService();
