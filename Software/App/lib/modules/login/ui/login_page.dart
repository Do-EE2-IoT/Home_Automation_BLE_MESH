import 'package:aithings/modules/login/login.src.dart';
import '../../../lib.dart';

part 'login_widget.dart';

class LoginPage extends BaseGetWidget<LoginController> {
  const LoginPage({Key? key}) : super(key: key);

  @override
  LoginController get controller => Get.put(LoginController());

  @override
  Widget buildWidgets(context) {
    return Scaffold(
      backgroundColor: AppColors.mainColors,
      body: _body(controller),
    );
  }
}
