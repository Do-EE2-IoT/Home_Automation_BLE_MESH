import mongoose, { Schema } from "mongoose";

const DOCUMENT_NAME = "User";
const COLLECTION_NAME = "USERS";

const userSchema = new mongoose.Schema(
  {
    name: {
      type: String,
      required: [true, " A user must have name"],
      trim: true,
      maxLength: 50,
    },
    email: {
      type: String,
      required: true,
      unique: true,
      trim:true,
      maxLength:50,
    },
    password: {
      type: String,
      required: true,
      trim:true,
      maxLength:100,
    },
    gateway:{
      type:String,
    },
    Unprovisioned : {
      type: mongoose.Schema.Types.ObjectId,
      ref: "New device",
    },
    button:{
      type: mongoose.Schema.Types.ObjectId,
      ref: "Button",
    },
    RGB:{
      type: mongoose.Schema.Types.ObjectId,
      ref: "RGB",
    }
  },
  {
    timestamps: true,
    collection: COLLECTION_NAME,
  }
);

const userModel = mongoose.model(DOCUMENT_NAME, userSchema);
export default userModel;
