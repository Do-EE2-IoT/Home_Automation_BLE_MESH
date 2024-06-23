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
  Control: {
    type: Boolean,
  },
  address: {
    type: String,
    unique: true,
  },
  Group: {
    type: Array,
  },
});

const ButtonModel = mongoose.model("Button", buttonSchema);

export default ButtonModel;
