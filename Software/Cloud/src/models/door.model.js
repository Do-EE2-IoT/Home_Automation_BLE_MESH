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
  },
  password: {
    type: String,
  },
  rfidCard: {
    type: Array,
  },
  address: {
    type: String,
    unique: true,
  },
  Group: {
    type: Array,
  },
});

const DoorModel = mongoose.model("Door", doorSchema);

export default DoorModel;
