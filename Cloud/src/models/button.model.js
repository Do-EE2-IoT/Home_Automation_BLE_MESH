import mongoose, { Schema } from "mongoose";

const buttonSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true,
  },
  user: {
    type: mongoose.Schema.Types.ObjectId,
    ref: "User",
    required: true,
  },
  control: {
    type: Boolean,
    default: true,
  },
  address: {
    type: String,
  },
  Group: {
    type: Array,
    default: [],
    unique: true,
  },
});

const ButtonModel = mongoose.model("Button", buttonSchema);

export default ButtonModel;
