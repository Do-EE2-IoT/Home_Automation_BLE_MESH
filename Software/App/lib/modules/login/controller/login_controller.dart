import 'package:aithings/lib.dart';
import 'package:aithings/modules/login/repository/login_repository.dart';

class LoginController extends BaseGetxController {
  TextEditingController usernameCtrl = TextEditingController();
  TextEditingController passwordCtrl = TextEditingController();
  late LoginRepository repository;

  @override
  onInit() {
    super.onInit();
    repository = LoginRepository(this);
  }

  Future<void> funcLogin() async {
    await repository.login(usernameCtrl.text, passwordCtrl.text).then((res) {
      if (res.code == AppConst.codeResultSuccess) {
        Get.offAllNamed(AppRoutes.routeHome);
      } else {
        showSnackBar(res.errorMessage);
      }
    });
  }
}
