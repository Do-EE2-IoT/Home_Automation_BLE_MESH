import dotenv from "dotenv";
import jwt from "jsonwebtoken";
import keyTokenModel from "../models/token.model.js";
dotenv.config("../../config.env");

const updateTokenByID = async (userId, Token) => {
  const tokenUpdate = keyTokenModel.findOneAndUpdate(
    { user: userId },
    { accessToken: Token },
    {
      new: true,
      upsert: true,
    }
  );
  return tokenUpdate;
};

const signTokenByID = async (userId) => {
  return jwt.sign({ userId }, process.env.JWT_SECRET, {
    expiresIn: process.env.JWT_EXPIRES,
  });
};

export { signTokenByID, updateTokenByID };
