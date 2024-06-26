import mongoose, { Schema } from "mongoose";

const doorSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true,
  },
  user: {
    type: mongoose.Schema.Types.ObjectId,
    ref: "User",
    required: true,
  },
  Control: {
    type: Boolean,
    default: true,
  },
  password: {
    type: String,
    default: "",
  },
  rfidCard: {
    type: Array,
    default: [],
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

const DoorModel = mongoose.model("Door", doorSchema);

export default DoorModel;
