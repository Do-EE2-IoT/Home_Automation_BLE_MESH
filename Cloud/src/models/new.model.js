
import mongoose, { Schema } from "mongoose";

const newDeviceSchema = new mongoose.Schema({
    type:
    {
        type:String,
        required: true,
        enum:['button','encoder','rgb','sensor','siren','door'],
    },
    uuid:{ 
        type:String,
        required:true,
        maxLength:50,
        unique:true,
    },
    mac:{
        type:String,
        required:true,
        maxLength:50,
        unique:true
    },
    user: { 
        type: mongoose.Schema.Types.ObjectId,
        ref: "User",
        required: true,
      },
});

const newDeviceModel = mongoose.model("New device", newDeviceSchema);
export default newDeviceModel;