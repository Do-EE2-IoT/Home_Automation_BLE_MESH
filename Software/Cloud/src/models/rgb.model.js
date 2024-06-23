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
  },

  green: {
    type: Number,
    min: 0,
    max: 255,
  },

  blue: {
    type: Number,
    min: 0,
    max: 255,
  },
  address: {
    type: String,
    unique: true,
  },
  Group: {
    type: Array,
  },
});

const RGBModel = mongoose.model("RGB", rgbLightSchema);

export default RGBModel;
