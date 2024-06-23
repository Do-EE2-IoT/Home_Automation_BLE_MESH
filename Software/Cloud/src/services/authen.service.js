import ErrorResponse from "../helpers/errorHandle.response.js";
// import dotenv from "dotenv";
// dotenv.config("../../config.env");
import util from "util";
import jwt from "jsonwebtoken";
import userModel from "../models/user.model.js";

const authenticationHeader = async (req, res, next) => {
  const userID = await req.get("CLIENT_ID");
  const accessToken = await req.get("ACCESS_TOKEN");

  if (!accessToken) {
    throw new ErrorResponse("You must log in to access this resource", 404);
  }

  // verify token
  const decoded = await util.promisify(jwt.verify)(
    accessToken,
    process.env.JWT_SECRET
  );

  // check user exist ?
  const currentUser = await  userModel.findById(decoded.userId);
  if (!currentUser) {
    throw new ErrorResponse(
      "The Token belong to this user no longer exist yet"
    );
  }

  if (currentUser._id != userID) {
    throw new ErrorResponse(`you are not belong to this page`, 404);
  }

  next();
};

export default authenticationHeader;
