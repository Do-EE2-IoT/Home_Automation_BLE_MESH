import ErrorResponse from "../helpers/errorHandle.response.js";
import userModel from "../models/user.model.js";
import bcrypt from "bcrypt";
import { signTokenByID, updateTokenByID } from "./token.service.js";
import newDeviceModel from "../models/new.model.js";


class AccessUserService {
  signUp = async (signupInFor) => {
    const newUserCheck = await userModel.findOne({ email: signupInFor.email });
    if (newUserCheck) {
      throw new ErrorResponse(
        "This email exist, need to be changed email",
        406
      );
    }

    const passwordHash = await bcrypt.hash(signupInFor.password, 10);

    const newUser = await userModel.create({
      name: signupInFor.name,
      email: signupInFor.email,
      password: passwordHash,
    });

    if (!newUser) {
      throw new ErrorResponse("Sign up - Unsuccessfully", 405);
    }

    return {
      User_Name: signupInFor.name,
      Email_Signup: signupInFor.email,
    };
  };

  logIn = async (logInInfor) => {
    //check email exist

  
      const findUser = await userModel.findOne({ email: logInInfor.email });
    
      if (!findUser) {
        throw new ErrorResponse("You must sign up this email first!", 404);
      }


    // check password
    const match = await bcrypt.compare(logInInfor.password, findUser.password);
    if (!match) {
      throw new Error("Authentication Error, enter password again", 404);
    }

    const token = await signTokenByID(findUser._id);
    const refreshToken = await updateTokenByID(findUser._id, token);
    if (!refreshToken) {
      throw new ErrorResponse("Can't refresh token", 404);
    }

    return {
      userId: findUser._id,
      token: token,
    };
  };
}

export default new AccessUserService();
