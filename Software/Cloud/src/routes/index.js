import express from "express";
import accessRouter from "./access.router/index.js";
import asynMiddlewareHandler from "../services/middleware.service.js";
import authenticationHeader from "../services/authen.service.js";
import deviceRouter from "./device.router/index.js";

const ApplicationRouter = express.Router();
const FollowingServiceRouter = express.Router();

ApplicationRouter.use("/v1/api/access", accessRouter)
  .use(asynMiddlewareHandler(authenticationHeader))
  .use("/v1/api/device", deviceRouter);

FollowingServiceRouter.use(asynMiddlewareHandler(authenticationHeader)).use(
  "/v1/api/device",
  deviceRouter
);
export { ApplicationRouter, FollowingServiceRouter };
