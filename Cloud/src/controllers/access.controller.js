import AccessUserService from "../services/access.service.js";

class AccessController {
  signUp = async (req, res, next) => {
    const { name, email, password } = req.body;
    console.log("[POST] :: Sign up ", req.body);
    return res
      .status(201)
      .json(await AccessUserService.signUp({ name, email, password }));
  };

  logIn = async (req, res, next) => {
    const { email, password } = req.body;
    console.log("Email is ::", email);
    console.log("password is", password);
    return res
      .status(201)
      .json(await AccessUserService.logIn({ email, password }));
  };
}

export default new AccessController();
