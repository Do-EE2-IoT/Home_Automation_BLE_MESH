import mongoose, { Schema } from "mongoose";

const rgbLightSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true,
  },
  user: {
    type: mongoose.Schema.Types.ObjectId,
    ref: "User",
    required: true,
  },
  red: {
    type: Number,
    min: 0,
    max: 255,
    default: 0,
  },

  green: {
    type: Number,
    min: 0,
    max: 255,
    default: 0,
  },

  blue: {
    type: Number,
    min: 0,
    max: 255,
    default: 0,
  },
  address: {
    type: String,
  },
  Group: {
    type: Array,
    default: [],
  },
});

const RGBModel = mongoose.model("RGB", rgbLightSchema);

export default RGBModel;
