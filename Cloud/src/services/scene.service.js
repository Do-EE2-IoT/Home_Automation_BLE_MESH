import baseResponse from "../helpers/respond.js";
import DoorModel from "../models/door.model.js";
import userModel from "../models/user.model.js";

class ScenceService {
  checkPassword = async (Infor) => {
    const user = await userModel.findById(Infor.clientID);
    if (!user) {
      return baseResponse(null, 404, "User exit !!");
    }
    const door = await DoorModel.findOne({
      user: Infor.clientID,
      address: Infor.address,
    });

    if (!door) {
      return baseResponse(null, 404, "You dont have this door");
    }

    if (door.password != Infor.oldPassword) {
      return baseResponse(
        null,
        404,
        "Entered wrong password, Let's enter again"
      );
    }

    const doorUpdatePassword = await DoorModel.findOne(
      {
        user: Infor.user,
        address: Infor.address,
      },
      {
        password: Infor.newPassword,
      },
      {
        new: true,
      }
    );
    if (!doorUpdatePassword) {
      return baseResponse(
        null,
        404,
        "Cannot update new password for door, let's try!"
      );
    }

    return baseResponse(
      doorUpdatePassword,
      201,
      "Update new password successfully"
    );
  };
}

export default new ScenceService();
