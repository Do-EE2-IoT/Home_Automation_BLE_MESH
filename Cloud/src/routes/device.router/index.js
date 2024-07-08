import express from "express";
import asynMiddlewareHandler from "../../services/middleware.service.js";
import deviceController from "../../controllers/device.controller.js";

const deviceRouter = express.Router();
deviceRouter
  .post("/addmygateway", asynMiddlewareHandler(deviceController.addMyGateway))
  .get("/mygateway", asynMiddlewareHandler(deviceController.myGateWay))
  .get(
    "/refreshnewdevice",
    asynMiddlewareHandler(deviceController.refreshNewDevice)
  )
  .get("/getnewdevice", asynMiddlewareHandler(deviceController.getAllNewDevice))
  .post("/addnewdevice", asynMiddlewareHandler(deviceController.addNewDevice))
  .post("/deletedevice", asynMiddlewareHandler(deviceController.deleteDevice))
  .patch("/registerroom", asynMiddlewareHandler(deviceController.registerRoom))
  .patch("/deleteroom", asynMiddlewareHandler(deviceController.deleteRoom))
  .get(
    "/infor/button/:address",
    asynMiddlewareHandler(deviceController.buttonInfor)
  )
  .get("/infor/rgb/:address", asynMiddlewareHandler(deviceController.rgbInfor))

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
  )
  .patch(
    "/control/rgb/:address",
    asynMiddlewareHandler(deviceController.rgbControl)
  )
  .patch(
    "/control/button/:address",
    asynMiddlewareHandler(deviceController.buttonControl)
  )
  .patch(
    "/control/siren/:address",
    asynMiddlewareHandler(deviceController.sirenControl)
  )
  .patch(
    "/control/door/:address",
    asynMiddlewareHandler(deviceController.doorControl)
  );

export default deviceRouter;
