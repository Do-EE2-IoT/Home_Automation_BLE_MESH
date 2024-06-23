import '../../../lib.dart';
import '../signup.src.dart';

part 'signup_widget.dart';

class SignupPage extends BaseGetWidget<SignupController> {
  const SignupPage({Key? key}) : super(key: key);

  @override
  SignupController get controller => Get.put(SignupController());

  @override
  Widget buildWidgets(context) {
    return Scaffold(
      appBar: UtilWidget.buildAppBar(SignupStr.signupTitle),
      body: _body(controller),
    );
  }
}
