import compression from "compression";
import Express from "express";
import helmet from "helmet";
import morgan from "morgan";
import cors from "cors";
import bodyParser from "body-parser";
import instanceMongoDB from "./databases/init.mongodb.js";
import ErrorResponse from "./helpers/errorHandle.response.js";
import { ApplicationRouter, FollowingServiceRouter } from "./routes/index.js";
import { mqttBrokerInit, mqttClient } from "./mqtt/init.mqtt.js";
import { subscribeTopic } from "./mqtt/subscribe.mqtt/index.js";

const FollowingService = Express();
// init middleware **************************************
FollowingService.use(morgan("combined")); // System log: combined for product, dev for dev
FollowingService.use(helmet()); // Header protect
FollowingService.use(compression()); // Compress output
FollowingService.use(bodyParser.json()); // Parsing application/json
FollowingService.use(bodyParser.urlencoded({ extended: true })); // Parsing application/x-www-form-urlencoded
FollowingService.use(cors());

// init database
mqttBrokerInit();
subscribeTopic("/device/resgateway", 1);
subscribeTopic("/device/detect", 1);
subscribeTopic("/device/registerroom", 1);
subscribeTopic("/device/deleteroom", 1);
subscribeTopic("/device/button", 1);
subscribeTopic("/device/rgb", 1);
subscribeTopic("/device/sensor", 1);
subscribeTopic("/device/siren", 1);
subscribeTopic("/device/door", 1);
subscribeTopic("/device/encoder", 1);
subscribeTopic("/device/deletedeviceres", 1);

FollowingService.use("", FollowingServiceRouter);
FollowingService.use("*", (req, res, next) => {
  throw new ErrorResponse(`Can't find ${req.originalUrl} on this server`, 404);
});
FollowingService.use((err, req, res, next) => {
  res.status(err?.code || 500).json({
    status: err?.code || 500,
    message: err?.message || "Interal server handler",
    err: err?.name,
    stack: err?.stack,
  });
});

export default FollowingService;
