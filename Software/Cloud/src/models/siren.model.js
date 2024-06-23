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

const SirenModel = mongoose.model("Siren", sirenSchema);

export default SirenModel;
