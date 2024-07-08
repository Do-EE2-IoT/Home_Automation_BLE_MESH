import mongoose, { Schema } from "mongoose";

const sirenSchema = new mongoose.Schema({
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
    default: 0,
  },
  address: {
    type: String,
  },
  Group: {
    type: Array,
  },
});

const SirenModel = mongoose.model("Siren", sirenSchema);

export default SirenModel;
