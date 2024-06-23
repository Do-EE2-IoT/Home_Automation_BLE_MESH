part of 'signup_page.dart';

Widget _body(SignupController controller) {
  return SingleChildScrollView(
    child: Column(
      children: [
        Form(
          child: Column(
            children: [
              InputTextField(
                isRequired: true,
                maxLengthInputForm: 50,
                textInputType: TextInputType.emailAddress,
                controller: controller.usernameCtrl,
                fillColor: AppColors.fillColor,
                isReadOnly: controller.isShowLoading.value,
                labelText: SignupStr.username,
                labelStyle: AppTextStyle.labelStyle(),
                hintText: SignupStr.inputUsername,
                isSmallValidator: true,
                floatingLabelStyle: AppTextStyle.labelStyle(
                  size: AppDimens.sizeTextLarge,
                ),
              ),
              UtilWidget.sizedBox20,
              InputTextField(
                isRequired: true,
                maxLengthInputForm: 50,
                labelText: SignupStr.password,
                hintText: SignupStr.inputPassword,
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
              ),
              UtilWidget.sizedBox20,
              InputTextField(
                isRequired: true,
                maxLengthInputForm: 50,
                labelText: SignupStr.name,
                hintText: SignupStr.inputName,
                isReadOnly: controller.isShowLoading.value,
                controller: controller.nameCtrl,
                fillColor: AppColors.fillColor,
                labelStyle: AppTextStyle.labelStyle(),
                isSmallValidator: true,
                iconNextTextInputAction: TextInputAction.done,
                floatingLabelStyle: AppTextStyle.labelStyle(
                  size: AppDimens.sizeTextLarge,
                ),
              ),
              UtilWidget.sizedBox20,
              InputTextField(
                isRequired: true,
                maxLengthInputForm: 50,
                labelText: SignupStr.email,
                hintText: SignupStr.inputEmail,
                isReadOnly: controller.isShowLoading.value,
                controller: controller.emailCtrl,
                fillColor: AppColors.fillColor,
                labelStyle: AppTextStyle.labelStyle(),
                isSmallValidator: true,
                iconNextTextInputAction: TextInputAction.done,
                floatingLabelStyle: AppTextStyle.labelStyle(
                  size: AppDimens.sizeTextLarge,
                ),
              ),
            ],
          ),
        ),
        UtilWidget.sizedBox20,
        buildButtonSignup(controller),
      ],
    ).paddingSymmetric(
        vertical: AppDimens.paddingHuge, horizontal: AppDimens.paddingMedium),
  );
}

Widget buildButtonSignup(SignupController controller,
    {Function? function, String? titleButton}) {
  return Obx(
    () => UtilWidget.buildButton(
        titleButton ?? SignupStr.register, function ?? controller.funcRegister,
        isLoading: controller.isShowLoading.value,
        backgroundColor: AppColors.mainColors),
  );
}
