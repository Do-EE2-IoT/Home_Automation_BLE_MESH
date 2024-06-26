import ErrorResponse from "../helpers/errorHandle.response.js";
import newDeviceModel from "../models/new.model.js";
import RGBModel from "../models/rgb.model.js";
import SensorModel from "../models/sensor.model.js";
import SirenModel from "../models/siren.model.js";
import EncoderModel from "../models/encoder.model.js";
import DoorModel from "../models/door.model.js";
import userModel from "../models/user.model.js";
import ButtonModel from "../models/button.model.js";

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
    const checkDetected = await newDeviceModel.findOne({
      user: data.clientID,
      mac: data.mac,
    });
    if (checkDetected) {
      console.log("This user have this device before, cannot add");
      return;
    } else {
      console.log("Prepare add new device");
    }
    const tryCheckDetectedDevice = await newDeviceModel.findOne({
      uuid: data.uuid,
      mac: data.mac,
      user: data.clientID,
    });
    if (tryCheckDetectedDevice) {
      console.log("This device save inside database before");
      return;
    }
    const detectedDevice = await newDeviceModel.create({
      type: data.type,
      uuid: data.uuid,
      mac: data.mac,
      user: data.clientID,
    });
    if (!detectedDevice) {
      console.log("Cannot update detected device");
      return;
    }
  } else if (topic === "/device/rgb") {
    const tryOldRGB = await RGBModel.findOne({
      user: data.clientID,
      address: data.address,
    });

    if (tryOldRGB) {
      console.log("This user owned this RGB");
      return;
    } else {
      console.log("Prepare add new RGB for user");
    }
    const newRGB = await RGBModel.create({
      name: "Rgb",
      user: data.clientID,
      address: data.address,
    });

    if (!newRGB) {
      console.log("Cannot add RGB for user");
    } else {
      console.log("Successfully add RGB device for user name ::", newRGB.user);
    }
  } else if (topic === "/device/sensor") {
    const tryOldSensor = await SensorModel.findOne({
      user: data.clientID,
      address: data.address,
    });
    if (tryOldSensor) {
      console.log("This sensor owned this sensor");
      return;
    } else {
      console.log("Prepare to add new sensor");
    }
    const newSensor = await SensorModel.create({
      name: "Sensor",
      user: data.clientID,
      address: data.address,
    });

    if (!newSensor) {
      console.log("Cannot add Sensor for user");
    } else {
      console.log(
        "Successfully add Sensor device for user name ::",
        newSensor.user
      );
    }
  } else if (topic === "/device/siren") {
    const tryOldSiren = await SirenModel.findOne({
      user: data.clientID,
      address: data.address,
    });
    if (tryOldSiren) {
      console.log("This user owned this Siren");
      return;
    } else {
      console.log("Prepare to add new Siren");
    }
    const newSiren = await SirenModel.create({
      name: "Siren",
      user: data.clientID,
      address: data.address,
    });

    if (!newSiren) {
      console.log("Cannot add Siren for user");
    } else {
      console.log(
        "Successfully add Siren device for user name ::",
        newSiren.user
      );
    }
  } else if (topic === "/device/door") {
    const tryOldDoor = await DoorModel.findOne({
      user: data.clientID,
      address: data.address,
    });
    if (tryOldDoor) {
      console.log("This user owned this Door");
      return;
    } else {
      console.log("Prepare to add new Door");
    }
    const newDoor = await DoorModel.create({
      name: "Door",
      user: data.clientID,
      address: data.address,
    });

    if (!newDoor) {
      console.log("Cannot add Door for user");
    } else {
      console.log(
        "Successfully add Door device for user name ::",
        newDoor.user
      );
    }
  } else if (topic === "/device/encoder") {
    const tryOldEncoder = await EncoderModel.findOne({
      user: data.clientID,
      address: data.address,
    });
    if (tryOldEncoder) {
      console.log("This user onwed this encoder");
      return;
    } else {
      console.log("Preprare add new encoder ");
    }
    const newEncoder = await EncoderModel.create({
      name: "Encoder",
      user: data.clientID,
      address: data.address,
    });

    if (!newEncoder) {
      console.log("Cannot add Encoder for user");
    } else {
      console.log(
        "Successfully add Encoder device for user name ::",
        newEncoder.user
      );
    }
  } else if (topic === "/device/button") {
    const tryOldButton = await ButtonModel.findOne({
      user: data.clientID,
      address: data.address,
    });
    if (tryOldButton) {
      console.log("This user onwed this button");
      return;
    } else {
      console.log("Preprare add new button ");
    }
    const newButton = await ButtonModel.create({
      name: "button",
      user: data.clientID,
      address: data.address,
    });

    if (!newButton) {
      console.log("Cannot add button for user");
    } else {
      console.log(
        "Successfully add button device for user name ::",
        newButton.user
      );
    }
  } else if (topic === "/device/deletedeviceres") {
    if (data.type == "button") {
      const button = await ButtonModel.findOne({
        user: data.clientID,
        address: data.address,
      });
      if (!button) {
        return;
      } else {
        const deleteButton = await ButtonModel.findOneAndDelete({
          address: data.address,
        });
        if (!deleteButton) {
          console.log("Cannot delete this button");
        }
        console.log("Delete button successfully");
      }
    } else if (data.type === "rgb") {
      const rgb = await RGBModel.findOne({
        user: data.clientID,
        address: data.address,
      });
      if (!rgb) {
        console.log("this Not exist this RGB");
        return;
      } else {
        const deleteRgb = await RGBModel.findOneAndDelete({
          address: data.address,
        });
        if (!deleteRgb) {
          console.log("Can not delete this RGB");
        }
        console.log("Delete RGB successfully");
      }
    } else if (data.type === "encoder") {
      const encoder = await EncoderModel.findOne({
        user: data.clientID,
        address: data.address,
      });
      if (!encoder) {
        console.log("this Not exist this encoder");
        return;
      } else {
        const deleteencoder = await encoderModel.findOneAndDelete({
          address: data.address,
        });
        if (!deleteencoder) {
          console.log("Can not delete this encoder");
        }
        console.log("Delete encoder successfully");
      }
    } else if (data.type === "sensor") {
      const sensor = await SensorModel.findOne({
        user: data.clientID,
        address: data.address,
      });
      if (!sensor) {
        console.log("this Not exist this sensor");
        return;
      } else {
        const deletesensor = await sensorModel.findOneAndDelete({
          address: data.address,
        });
        if (!deletesensor) {
          console.log("Can not delete this sensor");
        }
        console.log("Delete sensor successfully");
      }
    } else if (data.type === "siren") {
      const siren = await SirenModel.findOne({
        user: data.clientID,
        address: data.address,
      });
      if (!siren) {
        console.log("this Not exist this encoder");
        return;
      } else {
        const deleteensiren = await SirenModel.findOneAndDelete({
          address: data.address,
        });
        if (!deleteensiren) {
          console.log("Can not delete this siren");
        }
        console.log("Delete siren successfully");
      }
    } else if (data.type === "door") {
      const door = await DoorModel.findOne({
        user: data.clientID,
        address: data.address,
      });
      if (!door) {
        console.log("this Not exist this door");
        return;
      } else {
        const deletedoor = await DoorModel.findOneAndDelete({
          address: data.address,
        });
        if (!deletedoor) {
          console.log("Can not delete this door");
        }
        console.log("Delete door successfully");
      }
    }
  } else if (topic === "/device/registerroom") {
    const user = await userModel.findById(data.clientID);
    if (!user) {
      console.log("Not exist user");
      return;
    }
    if (data.type === "button") {
      const button = await ButtonModel.findOne({
        user: data.clientID,
        address: data.address,
      });
      if (!button) {
        console.log("This user dont have this button");
        return;
      }
      if (button.Group.includes(data.group)) {
        console.log("This user registed room 1 before");
        return;
      }

      const buttonRegiterRoom = await ButtonModel.findOneAndUpdate(
        { user: data.clientID },
        { $push: { Group: data.group } },
        { new: true }
      );
      if (!buttonRegiterRoom) {
        console.log(`Cannot register room ${data.group} for this user`);
        return;
      }
    }
  } else if (topic === "/device/deleteroom") {
    const user = await userModel.findById(data.clientID);
    if (!user) {
      console.log("Not exist user");
      return;
    }
    if (data.type === "button") {
      const button = await ButtonModel.findOne({
        user: data.clientID,
        address: data.address,
        Group: data.group,
      });
      if (!button) {
        console.log("This user don't have this button or registed this room yet");
        return;
      }
      const buttonDeleteRoom = await ButtonModel.findOneAndUpdate(
        { user: data.clientID },
        { $pull: { Group: data.group } }
      );
      if (!buttonDeleteRoom) {
        console.log(`Cannot delete room ${data.group} for this user`);
        return;
      }
    }
  }
};
export { mqttHandle };
