import SceneService from "../services/scene.service.js";

class SceneController {
  checkDoor = async (req, res, next) => {
    const clientID = req.get("CLIENT_ID");
    const address = req.params.address;
    const oldPassword = req.body.old;
    const newPassword = req.body.new;
    return res
      .status(201)
      .json(
        await SceneService.checkPassword({
          clientID,
          address,
          oldPassword,
          newPassword,
        })
      );
  };
}

export default new SceneController();
