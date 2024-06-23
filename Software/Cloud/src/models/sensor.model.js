import mongoose, { Schema } from "mongoose";

const sensorSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true,
  },
  user: {
    type: mongoose.Schema.Types.ObjectId,
    ref: "User",
    required: true,
  },
  temperature: {
    type: String,
  },
  humidity: {
    type: String,
  },
  smoke: {
    type: String,
  },
  address: {
    type: String,
    unique: true,
  },
  Group: {
    type: Array,
  },
});

const SensorModel = mongoose.model("Sensor", sensorSchema);

export default SensorModel;
