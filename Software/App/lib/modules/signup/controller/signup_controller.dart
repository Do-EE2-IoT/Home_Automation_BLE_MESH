import 'package:aithings/lib.dart';
import 'package:aithings/modules/modules.src.dart';

class SignupController extends BaseGetxController {
  TextEditingController usernameCtrl = TextEditingController();
  TextEditingController passwordCtrl = TextEditingController();
  TextEditingController nameCtrl = TextEditingController();
  TextEditingController emailCtrl = TextEditingController();

  late SignupRepository repository;

  SignupController() {
    repository = SignupRepository(this);
  }

  Future<void> funcRegister() async {
    await repository
        .register(
      SignupReqModel(
        username: usernameCtrl.text,
        password: passwordCtrl.text,
        name: nameCtrl.text,
        email: emailCtrl.text,
      ),
    )
        .then((res) {
      if (res.code == AppConst.codeResultSuccess) {
        showSnackBar("Đăng ký thành công!");
        Get.offAndToNamed(AppRoutes.routeLogin);
      }
    });
  }
}
