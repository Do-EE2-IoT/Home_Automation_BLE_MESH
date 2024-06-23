part of 'login_page.dart';

Widget _body(LoginController controller) {
  return Center(
    child: Container(
      decoration: BoxDecoration(
        color: AppColors.basicWhite,
        borderRadius: BorderRadius.circular(AppDimens.radius20),
      ),
      height: Get.height * 0.6,
      width: Get.width * 0.9,
      padding: const EdgeInsets.all(AppDimens.paddingSmall),
      child: Column(
        children: [
          const TextUtils(
            text: LoginStr.loginBtn,
            availableStyle: StyleEnum.heading1Bold,
            color: AppColors.basicBlack,
          ),
          Form(
            child: Column(
              children: [
                InputTextField(
                  isRequired: true,
                  maxLengthInputForm: 50,
                  textInputType: TextInputType.emailAddress,
                  validator: (value) =>
                      value.isNullOrEmpty ? LoginStr.usernameEmptyErr : null,
                  controller: controller.usernameCtrl,
                  fillColor: AppColors.fillColor,
                  isReadOnly: controller.isShowLoading.value,
                  labelText: LoginStr.username,
                  labelStyle: AppTextStyle.labelStyle(),
                  hintText: LoginStr.inputUsername,
                  isSmallValidator: true,
                  floatingLabelStyle: AppTextStyle.labelStyle(
                    size: AppDimens.sizeTextLarge,
                  ),
                ).paddingSymmetric(vertical: AppDimens.paddingMedium),
                InputTextField(
                  isRequired: true,
                  maxLengthInputForm: 50,
                  validator: (value) =>
                      isPasswordValidate(password: value ?? "", minLength: 6)
                          ? null
                          : LoginStr.loginErrorLengthPass,
                  labelText: LoginStr.password,
                  hintText: LoginStr.inputPassword,
                  obscureText: true,
                  isReadOnly: controller.isShowLoading.value,
                  controller: controller.passwordCtrl,
                  fillColor: AppColors.fillColor,
                  labelStyle: AppTextStyle.labelStyle(),
                  isSmallValidator: true,
                  iconNextTextInputAction: TextInputAction.done,
                  floatingLabelStyle: AppTextStyle.labelStyle(
                    size: AppDimens.sizeTextLarge,
                  ),
                  submitFunc: (v) {
                    // controller.funcLogin();
                  },
                ),
              ],
            ),
          ),
          UtilWidget.sizedBox20,
          buildButtonLogin(controller),
          UtilWidget.sizedBox20,
          _buildSignup(),
        ],
      ),
    ),
  );
}

Widget buildButtonLogin(LoginController controller,
    {Function? function, String? titleButton}) {
  return Obx(
    () => UtilWidget.buildButton(
        titleButton ?? LoginStr.loginBtn, function ?? controller.funcLogin,
        isLoading: controller.isShowLoading.value,
        backgroundColor: AppColors.mainColors),
  );
}

Widget _buildSignup() {
  return RichText(
    text: TextSpan(
      children: [
        const TextSpan(
          text: LoginStr.notHaveAccount,
          style: TextStyle(color: Colors.black),
        ),
        TextSpan(
          text: LoginStr.signup,
          style: const TextStyle(
            color: Colors.blueAccent,
          ),
          recognizer: TapGestureRecognizer()
            ..onTap = () {
              Get.toNamed(AppRoutes.routeSignup);
            },
        ),
      ],
    ),
  );
}
