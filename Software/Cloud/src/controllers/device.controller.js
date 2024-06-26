import deviceService from "../services/device.service.js";

class DeviceController {
  addMyGateway = async (req, res, next) => {
    const macGateway = req.body.mac;
    const clientID = req.get("CLIENT_ID");
    console.log("POST :: FIND MY GATREWAY - ", macGateway);
    return res
      .status(201)
      .json(await deviceService.addMyGateway({ macGateway, clientID }));
  };
  getAllNewDevice = async (req, res, next) => {
    console.log("GET :: ALL NEW DEVICE ");
    const clientID = req.get("CLIENT_ID");
    return res
      .status(201)
      .json(await deviceService.getDetectedDevice(clientID));
  };

  addNewDevice = async (req, res, next) => {
    console.log("POST :: ADD NEW DEVICE ");
    const clientID = req.get("CLIENT_ID");
    const { type, uuid, mac } = req.body;
    return res
      .status(201)
      .json(await deviceService.addNewDevice({ type, uuid, mac, clientID }));
  };

  deleteDevice = async (req, res, next) => {
    console.log("POST :: Delete device");
    const clientID = req.get("CLIENT_ID");
    const { address, type } = req.body;
    return res
      .status(201)
      .json(await deviceService.deleteDevice({ clientID, address, type }));
  };

  registerRoom = async (req, res, next) => {
    console.log("PATCH :: register device for room 1");
    const clientID = req.get("CLIENT_ID");
    const { type, group, address } = req.body;
    return res
      .status(201)
      .json(
        await deviceService.registerRoom({ clientID, group, type, address })
      );
  };

  // -----------------------------------RGB---------------------------------//
  rgbInfor = async (req, res, next) => {
    const RGBAddress = req.params.address;
    console.log("GET :: RGB infor", RGBAddress);
    const clientID = req.get("CLIENT_ID");
    return res
      .status(201)
      .json(await deviceService.rgbInfor({ RGBAddress, clientID }));
  };

  rgbControl = async (req, res, next) => {
    const clientID = req.get("CLIENT_ID");
    const unicastAddress = req.params.address;
    if (req.query.groupaddress) {
      const groupAddress = req.query.groupaddress;
      const { red, green, blue } = req.body;
      return res.status(201).json(
        await deviceService.rgbControl({
          red,
          green,
          blue,
          clientID,
          unicastAddress,
          groupAddress,
        })
      );
    } else {
      const { red, green, blue } = req.body;
      return res.status(201).json(
        await deviceService.rgbControl({
          red,
          green,
          blue,
          clientID,
          unicastAddress,
        })
      );
    }
  };

  sensorInfor = async (req, res, next) => {
    const SensorAddress = req.params.address;
    console.log("GET :: Sensor infor", SensorAddress);
    const clientID = req.get("CLIENT_ID");
    return res
      .status(201)
      .json(await deviceService.sensorInfor({ SensorAddress, clientID }));
  };

  sirenInfor = async (req, res, next) => {
    const SirenAddress = req.params.address;
    console.log("GET :: Siren infor", SirenAddress);
    const clientID = req.get("CLIENT_ID");
    return res
      .status(201)
      .json(await deviceService.sirenInfor({ SirenAddress, clientID }));
  };

  doorInfor = async (req, res, next) => {
    const DoorAddress = req.params.address;
    console.log("GET :: Door infor", DoorAddress);
    const clientID = req.get("CLIENT_ID");
    return res
      .status(201)
      .json(await deviceService.doorInfor({ DoorAddress, clientID }));
  };

  encoderInfor = async (req, res, next) => {
    const EncoderAddress = req.params.address;
    console.log("GET :: encoder infor", EncoderAddress);
    const clientID = req.get("CLIENT_ID");
    return res
      .status(201)
      .json(await deviceService.encoderInfor({ EncoderAddress, clientID }));
  };

  buttonInfor = async (req, res, next) => {
    const ButtonAddress = req.params.address;
    console.log("GET :: Button infor", ButtonAddress);
    const clientID = req.get("CLIENT_ID");
    return res
      .status(201)
      .json(await deviceService.buttonInfor({ ButtonAddress, clientID }));
  };
}
export default new DeviceController();
