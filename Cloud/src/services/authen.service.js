import ErrorResponse from "../helpers/errorHandle.response.js";
// import dotenv from "dotenv";
// dotenv.config("../../config.env");
import util from "util";
import jwt from "jsonwebtoken";
import userModel from "../models/user.model.js";
import baseResponse from "../helpers/respond.js";

const authenticationHeader = async (req, res, next) => {
  const userID = await req.get("CLIENT_ID");
  const accessToken = await req.get("ACCESS_TOKEN");
  if (!accessToken) {
    res
      .status(404)
      .json(baseResponse(null, 404, "You must login to access this page"));
  }

  // verify token
  const decoded = await util.promisify(jwt.verify)(
    accessToken,
    process.env.JWT_SECRET
  );

  // check user exist ?
  const currentUser = await userModel.findById(decoded.userId);
  if (!currentUser) {
    res.status(404).json(baseResponse(null, 404, "User exit"));
  }

  if (currentUser._id != userID) {
    res
      .status(404)
      .json(baseResponse(null, 404, "You are not belong to this page"));
  }

  next();
};

export default authenticationHeader;
