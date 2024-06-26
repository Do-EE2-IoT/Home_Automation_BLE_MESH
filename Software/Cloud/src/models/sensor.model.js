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
    type: Number,
    default: 0,
  },
  humidity: {
    type: Number,
    default: 0,
  },
  smoke: {
    type: Boolean,
    default: 0,
  },
  address: {
    type: String,
    unique: true,
  },
  Group: {
    type: Array,
    default: [],
  },
});

const SensorModel = mongoose.model("Sensor", sensorSchema);

export default SensorModel;
