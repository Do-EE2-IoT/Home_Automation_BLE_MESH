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
    default: "",
  },
  humidity: {
    type: String,
    default: "",
  },
  smoke: {
    type: String,
    default: "",
  },
  address: {
    type: String,
    default: "",
  },
  Group: {
    type: Array,
    default: [],
  },
});

const SensorModel = mongoose.model("Sensor", sensorSchema);

export default SensorModel;
