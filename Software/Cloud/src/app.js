import compression from "compression";
import Express from "express";
import helmet from "helmet";
import morgan from "morgan";
import cors from "cors";
import bodyParser from "body-parser";
import instanceMongoDB from "./databases/init.mongodb.js";
import ErrorResponse from "./helpers/errorHandle.response.js";
import { ApplicationRouter } from "./routes/index.js";
import { mqttBrokerInit } from "./mqtt/init.mqtt.js";

const app = Express();
// init middleware **************************************
app.use(morgan("combined")); // System log: combined for product, dev for dev
app.use(helmet()); // Header protect
app.use(compression()); // Compress output
app.use(bodyParser.json()); // Parsing application/json
app.use(bodyParser.urlencoded({ extended: true })); // Parsing application/x-www-form-urlencoded
app.use(cors());

// init database
instanceMongoDB;

app.use("", ApplicationRouter);

app.use("*", (req, res, next) => {
  throw new ErrorResponse(`Can't find ${req.originalUrl} on this server`, 404);
});

app.use((err, req, res, next) => {
  res.status(err?.code || 500).json({
    status: err?.code || 500,
    message: err?.message || "Interal server handler",
    err: err?.name,
    stack: err?.stack,
  });
});

export default app;
