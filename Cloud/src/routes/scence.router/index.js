import express from "express";
import asynMiddlewareHandler from "../../services/middleware.service.js";
import SceneController from "../../controllers/scene.controller.js";

const sceneRouter = express.Router();
sceneRouter.use(
  "/door/changepassword/:address",
  asynMiddlewareHandler(SceneController.checkDoor)
);

export default sceneRouter;
