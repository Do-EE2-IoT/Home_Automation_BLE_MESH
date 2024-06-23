import 'package:aithings/lib.dart';

class UtilWidget {
  static DateTime? _dateTime;
  static int _oldFunc = 0;

  /// Sử dụng để tránh trường hợp click liên tiếp khi thực hiện function
  static Widget baseOnAction({
    required Function onTap,
    required Widget child,
  }) {
    return GestureDetector(
      behavior: HitTestBehavior.opaque,
      onTap: () {
        DateTime now = DateTime.now();
        if (_dateTime == null ||
            now.difference(_dateTime ?? DateTime.now()) > 2.seconds ||
            onTap.hashCode != _oldFunc) {
          _dateTime = now;
          _oldFunc = onTap.hashCode;
          onTap();
        }
      },
      child: child,
    );
  }

  static Widget buildLoading({Color? colorIcon}) => CupertinoActivityIndicator(
        color: colorIcon,
      );
  static Widget buildRichText({
    required List<String> children,
    Color? textColor,
    double? fontSize,
    FontStyle? fontStyle,
    FontWeight? fontWeight,
    TextAlign? textAlign,
    TextOverflow? overflow,
    TextDecoration? decoration,
    TextStyle? Function(TextStyle? style, int index)? styleBuilder,
    GestureRecognizer? Function(int index)? recognizerBuilder,
  }) {
    final style = Get.textTheme.bodySmall?.copyWith(
      color: textColor,
      fontStyle: fontStyle,
      fontWeight: fontWeight,
      decoration: decoration,
      overflow: overflow ?? TextOverflow.ellipsis,
      fontSize: fontSize ?? AppConst.textDefault,
    );
    return Text.rich(
      textAlign: textAlign,
      TextSpan(
        text: children.first,
        recognizer: recognizerBuilder?.call(0),
        style: styleBuilder?.call(style, 0) ?? style,
        children: List.generate(
          children.length - 1,
          (index) {
            final text = children[index + 1];
            return TextSpan(
              text: text,
              style: styleBuilder?.call(style, index + 1) ?? style,
              recognizer: recognizerBuilder?.call(index + 1),
            );
          },
        ),
      ),
    );
  }

  static const Widget sizedBox0 = SizedBox.shrink();
  static const Widget sizedBox5 = SizedBox(height: 5);
  static const Widget sizedBox7 = SizedBox(height: 7);
  static const Widget sizedBox3 = SizedBox(height: 3);
  static const Widget sizedBox8 = SizedBox(height: AppDimens.paddingVerySmall);
  static const Widget sizedBox10 = SizedBox(height: 10);
  static const Widget sizedBox20 = SizedBox(height: 20);
  static const Widget sizedBox12 = SizedBox(height: 12);
  static const Widget sizedBoxWidth10 = SizedBox(width: 10);
  static const Widget sizedBoxWidth5 = SizedBox(width: 5);
  static const Widget dividerDefault = Divider(height: 3);
  static const Widget sizedBoxPaddingHuge =
      SizedBox(height: AppDimens.paddingHuge);
  static const Widget sizedBoxPadding =
      SizedBox(height: AppDimens.defaultPadding);

  static Widget buildButton(
    String btnTitle,
    Function function, {
    List<Color> colors = AppColors.colorGradientBlue,
    Color? backgroundColor,
    bool isLoading = false,
    bool showLoading = true,
    IconData? icon,
    Color? iconColor,
    double? iconSize,
    double? width,
    double? height,
    Color? colorText,
    BorderRadiusGeometry? borderRadius,
  }) {
    return Container(
      width: width ?? double.infinity,
      height: height ?? AppDimens.btnMedium,
      decoration: BoxDecoration(
          color: backgroundColor,
          gradient:
              backgroundColor != null ? null : LinearGradient(colors: colors),
          borderRadius: borderRadius ?? BorderRadius.circular(8)),
      child: baseOnAction(
        onTap: !isLoading ? function : () {},
        child: ElevatedButton(
          onPressed: null,
          style: ElevatedButton.styleFrom(
            foregroundColor: Colors.transparent,
            backgroundColor: Colors.transparent,
            shadowColor: Colors.transparent,
            shape: RoundedRectangleBorder(
                borderRadius: borderRadius ?? BorderRadius.circular(8.0)),
          ),
          child: Stack(
            children: [
              if (icon != null)
                Align(
                  alignment: Alignment.centerLeft,
                  child: Icon(
                    icon,
                    size: iconSize,
                    color: iconColor,
                  ),
                ),
              Center(
                child: Text(btnTitle.tr,
                    style: TextStyle(
                        fontSize: AppDimens.fontMedium(),
                        color: colorText ?? Colors.white)),
              ),
              Align(
                alignment: Alignment.centerRight,
                child: Visibility(
                  visible: isLoading && showLoading,
                  child: const SizedBox(
                    height: AppDimens.btnSmall,
                    width: AppDimens.btnSmall,
                    child: CircularProgressIndicator(
                      strokeWidth: 2,
                      backgroundColor: Colors.white,
                      valueColor: AlwaysStoppedAnimation<Color>(
                        AppColors.colorError,
                      ),
                    ),
                  ),
                ),
              )
            ],
          ),
        ),
      ),
    );
  }

  static PreferredSizeWidget buildAppBar(String title,
      {Color? textColor,
      Color? actionsIconColor,
      Color? backbuttonColor,
      Color? backgroundColor,
      bool centerTitle = false,
      Function()? funcLeading,
      bool leading = true,
      List<Widget>? actions,
      bool isColorGradient = false,
      List<Color>? colorTranparent,
      PreferredSizeWidget? widget}) {
    return AppBar(
      bottom: widget,
      actionsIconTheme:
          IconThemeData(color: actionsIconColor ?? AppColors.basicWhite),
      systemOverlayStyle: const SystemUiOverlayStyle(
        statusBarColor: Colors.transparent,
        statusBarIconBrightness: Brightness.light,
      ),
      title: UtilWidget.buildAppBarTitle(
        title,
        textColor: textColor ?? AppColors.basicWhite,
      ),
      automaticallyImplyLeading: false,
      centerTitle: centerTitle,
      titleSpacing: 0,
      leading: !leading
          ? null
          : BackButton(
              color: backbuttonColor ?? AppColors.basicWhite,
              onPressed: funcLeading,
            ),
      flexibleSpace: isColorGradient
          ? Container(
              decoration: BoxDecoration(
                gradient: LinearGradient(
                    begin: Alignment.bottomLeft,
                    end: Alignment.bottomRight,
                    colors: colorTranparent ??
                        <Color>[Colors.orangeAccent, Colors.deepOrange]),
              ),
            )
          : null,
      actions: actions,
      backgroundColor:
          isColorGradient ? null : backgroundColor ?? AppColors.mainColors,
    );
  }

  static buildAppBarTitle(String title,
      {bool? textAlignCenter,
      Color? textColor,
      TextOverflow? overflow,
      int? maxLines}) {
    textAlignCenter = textAlignCenter ?? GetPlatform.isAndroid;
    return TextUtils(
      text: title.tr,
      textAlign: textAlignCenter ? TextAlign.center : TextAlign.left,
      color: textColor ?? AppColors.basicWhite,
      availableStyle: StyleEnum.heading1Bold,
    );
  }
}
