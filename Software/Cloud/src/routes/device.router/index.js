import express from "express";
import asynMiddlewareHandler from "../../services/middleware.service.js";
import deviceController from "../../controllers/device.controller.js";

const deviceRouter = express.Router();
deviceRouter
  .post("/addmygateway", asynMiddlewareHandler(deviceController.addMyGateway))
  .get("/getnewdevice", asynMiddlewareHandler(deviceController.getAllNewDevice))
  .post("/addnewdevice", asynMiddlewareHandler(deviceController.addNewDevice))
  .post("/deletedevice", asynMiddlewareHandler(deviceController.deleteDevice))
  .patch("/registerroom/1", asynMiddlewareHandler(deviceController.registerRoom)) // group must be 0xC000
  .patch("/registerroom/2", asynMiddlewareHandler(deviceController.registerRoom)) // group must be 0xC001
  .get("/infor/button/:address", asynMiddlewareHandler(deviceController.buttonInfor))
  .get("/infor/rgb/:address", asynMiddlewareHandler(deviceController.rgbInfor))
  .patch(
    "/control/rgb/:address",
    asynMiddlewareHandler(deviceController.rgbControl)
  )
  .get(
    "/infor/sensor/:address",
    asynMiddlewareHandler(deviceController.sensorInfor)
  )
  .get(
    "/infor/siren/:address",
    asynMiddlewareHandler(deviceController.sirenInfor)
  )
  .get(
    "/infor/encoder/:address",
    asynMiddlewareHandler(deviceController.encoderInfor)
  )
  .get(
    "/infor/door/:address",
    asynMiddlewareHandler(deviceController.doorInfor)
  );

export default deviceRouter;
