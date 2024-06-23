import ErrorResponse from "../helpers/errorHandle.response.js";
import userModel from "../models/user.model.js";
import bcrypt from "bcrypt";
import { signTokenByID, updateTokenByID } from "./token.service.js";
import newDeviceModel from "../models/new.model.js";
import baseResponse from "../helpers/respond.js";

class AccessUserService {
  signUp = async (signupInFor) => {
    const newUserCheck = await userModel.findOne({ email: signupInFor.email });
    if (newUserCheck) {
      return baseResponse(
        null,
        409,
        `you cannot sign up this ${signupInFor.emal} `
      );
    }

    const passwordHash = await bcrypt.hash(signupInFor.password, 10);

    const newUser = await userModel.create({
      name: signupInFor.name,
      email: signupInFor.email,
      password: passwordHash,
    });

    if (!newUser) {
      return baseResponse(null, 400, "Your action can't carry out");
    }
    const data = {
      name: newUser.name,
      email: newUser.email,
    };
    return baseResponse(data, 201, "You sign up account successfully");
  };

  logIn = async (logInInfor) => {
    //check email exist

    const findUser = await userModel.findOne({ email: logInInfor.email });

    if (!findUser) {
      return baseResponse(null, 404, "You must sign up this email first!");
    }

    // check password
    const match = await bcrypt.compare(logInInfor.password, findUser.password);
    if (!match) {
      return baseResponse(
        null,
        404,
        "Authentication Error, enter password again"
      );
    }

    const token = await signTokenByID(findUser._id);
    const refreshToken = await updateTokenByID(findUser._id, token);
    if (!refreshToken) {
      baseResponse(null, 404, "Can't refresh token");
    }
    const data = {
      clientID: findUser._id,
      token: token,
    };
    return baseResponse(data, 200, "Log in successfully");
  };
}

export default new AccessUserService();
