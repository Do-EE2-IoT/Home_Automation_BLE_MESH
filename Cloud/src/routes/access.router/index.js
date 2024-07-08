import express from "express";
import asynMiddlewareHandler from "../../services/middleware.service.js";
import AccessController from ".././../controllers/access.controller.js"


const accessRouter = express.Router();
accessRouter
  .post("/user/signup", asynMiddlewareHandler(AccessController.signUp))
  .post("/user/login", asynMiddlewareHandler(AccessController.logIn));

export default accessRouter;
