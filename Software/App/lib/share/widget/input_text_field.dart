import 'package:aithings/lib.dart';

class InputTextField extends StatefulWidget {
  final IconData? iconLeading;

  final String? hintText;

  final TextEditingController controller;

  final FocusNode? currentNode;

  final FocusNode? nextNode;

  final bool obscureText;

  final double borderRadius;

  final TextInputAction iconNextTextInputAction;

  final ValueChanged<String>? submitFunc;

  final ValueChanged<String>? onNext;

  final FormFieldValidator<String>? validator;

  /// 0 : LengthLimitingText. Giới hạn ký tự nhập(nếu có)
  /// 1 : digitsOnly. Chỉ nhập số
  /// 2 : TaxCode. Kiểu nhập là mã số thuế
  /// 3 : Không cho nhập các ký tự đặc biệt. dấu cách
  /// 4 : Nhập giá trị tiền
  /// 5 : Cho phép nhập giá trị âm
  final int inputFormatters;

  final TextInputType textInputType;

  /// The [lengthLimit] must be null, -1 or greater than zero. If it is null or -1
  /// then no limit is enforced.
  final int? maxLengthInputForm;

  final bool isReadOnly;

  final bool autoFocus;

  final Color? fillColor;

  final bool isRequired;

  final Color? textColor;

  final Color? hintTextColor;

  final double? hintTextSize;

  final double? textSize;

  final Color? prefixIconColor;

  final Color? errorTextColor;

  final Color? suffixColor;

  final ValueChanged<String>? onChanged;

  final int? maxLines;

  final Widget? suffixIcon;

  final TextAlign? textAlign;

  final bool isShowCounterText;

  final bool enable;

  final String? labelText;

  final VoidCallback? onTap;

  final EdgeInsetsGeometry? contentPadding;

  final TextStyle? labelStyle;

  final FloatingLabelBehavior? floatingLabelBehavior;

  final TextStyle? floatingLabelStyle;

  final Color colorBorderText;

  final bool isShowSuffixIcon;

  final bool isSmallValidator;

  const InputTextField({
    super.key,
    this.iconLeading,
    this.hintText,
    this.isRequired = false,
    required this.controller,
    this.currentNode,
    this.submitFunc,
    this.nextNode,
    this.obscureText = false,
    this.iconNextTextInputAction = TextInputAction.next,
    this.onNext,
    this.validator,
    this.inputFormatters = 0,
    this.borderRadius = 10.0,
    this.textInputType = TextInputType.text,
    this.maxLengthInputForm,
    this.isReadOnly = false,
    this.autoFocus = false,
    this.fillColor,
    this.textColor,
    this.hintTextColor,
    this.hintTextSize,
    this.textSize,
    this.prefixIconColor,
    this.errorTextColor,
    this.suffixColor,
    this.onChanged,
    this.maxLines = 1,
    this.suffixIcon,
    this.isShowCounterText = false,
    this.textAlign,
    this.enable = true,
    this.contentPadding,
    this.labelText,
    this.onTap,
    this.labelStyle,
    this.floatingLabelBehavior,
    this.floatingLabelStyle,
    this.colorBorderText = Colors.green,
    this.isShowSuffixIcon = true,
    this.isSmallValidator = false,
  });

  @override
  InputTextFieldState createState() => InputTextFieldState();
}

class InputTextFieldState extends State<InputTextField> {
  final RxBool _isShowButtonClear = false.obs;
  final RxBool _showPassword = false.obs;

  @override
  void initState() {
    widget.controller.addListener(() {
      if (widget.controller.text.isNotEmpty) {
        _isShowButtonClear.value = true;
      }
    });
    _showPassword.value = widget.obscureText;
    super.initState();
  }

  List<TextInputFormatter> getFormatters() {
    switch (widget.inputFormatters) {
      case 1:
        return [
          FilteringTextInputFormatter.digitsOnly,
          LengthLimitingTextInputFormatter(widget.maxLengthInputForm),
        ];
      case 2:
        return [
          FilteringTextInputFormatter.allow(RegExp(r"[\d-]")),
          LengthLimitingTextInputFormatter(14),
        ];
      case 3:
        return [
          FilteringTextInputFormatter.allow(RegExp(r'[a-zA-Z\d-_.]')),
        ];
      case 4:
        return [
          NumericTextFormatter(),
        ];
      case 5:
        return [
          FilteringTextInputFormatter.allow(RegExp(r'[\d-]')),
        ];
      default:
        return [
          LengthLimitingTextFieldFormatterFixed(widget.maxLengthInputForm)
        ];
    }
  }

  Widget? _suffixIcon() {
    if (widget.suffixIcon != null) {
      return widget.suffixIcon;
    }
    if (!_isShowButtonClear.value || widget.isReadOnly) {
      return null;
    }
    if (!widget.isShowSuffixIcon) {
      return null;
    }
    return widget.obscureText
        ? GestureDetector(
            onTap: () {
              _showPassword.toggle();
            },
            child: Icon(
              _showPassword.value
                  ? Icons.visibility_off_outlined
                  : Icons.remove_red_eye_outlined,
              color: widget.suffixColor ?? AppColors.textColor(),
            ),
          )
        : Visibility(
            visible: _isShowButtonClear.value && !widget.isReadOnly,
            child: GestureDetector(
              onTap: () {
                widget.controller.clear();
                widget.onChanged?.call('');
                _isShowButtonClear.value = false;
              },
              child: Icon(
                Icons.clear,
                color: widget.suffixColor ?? AppColors.textColor(),
              ),
            ),
          );
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        UtilWidget.buildRichText(
          children: [
            widget.labelText ?? '',
            widget.isRequired ? ' (*)' : '',
          ],
          styleBuilder: (style, index) {
            final style = widget.labelStyle ?? Get.textTheme.bodyMedium;
            return style?.copyWith(
              color: index == 0 ? null : AppColors.redDark,
            );
          },
        ),
        UtilWidget.sizedBox8,
        Obx(
          () => TextFormField(
            maxLines: widget.maxLines,
            inputFormatters: getFormatters(),
            validator: widget.validator,
            autovalidateMode: AutovalidateMode.onUserInteraction,
            onChanged: (v) {
              if (!_isShowButtonClear.value || v.isEmpty) {
                _isShowButtonClear.value = v.isNotEmpty;
              }
              widget.onChanged?.call(v);
            },
            textInputAction: widget.iconNextTextInputAction,
            style: TextStyle(
                fontSize: widget.textSize ?? AppDimens.fontSmall(),
                color: widget.textColor ?? AppColors.textColor()),
            controller: widget.controller,
            obscureText: _showPassword.value,
            onTap: widget.onTap,
            enabled: widget.enable,
            autofocus: widget.autoFocus,
            focusNode: widget.currentNode,
            keyboardType: widget.textInputType,
            readOnly: widget.isReadOnly,
            maxLength: widget.maxLengthInputForm,
            textAlign: widget.textAlign ?? TextAlign.left,
            onFieldSubmitted: (v) {
              if (widget.iconNextTextInputAction.toString() ==
                  TextInputAction.next.toString()) {
                FocusScope.of(context).requestFocus(widget.nextNode);

                widget.onNext?.call(v);
              } else {
                widget.submitFunc?.call(v);
              }
            },
            decoration: createInputDecoration(),
          ),
        ),
      ],
    );
  }

  InputDecoration createInputDecoration() {
    return InputDecoration(
      counterText: widget.isShowCounterText ? null : '',
      filled: true,
      fillColor: widget.fillColor ?? AppColors.inputText(),
      hintStyle: TextStyle(
        fontSize: widget.hintTextSize ?? AppDimens.fontMedium(),
        color: widget.hintTextColor ?? AppColors.hintTextColor(),
      ),
      hintText: widget.hintText,
      errorStyle: widget.isSmallValidator
          ? TextStyle(
              color: widget.errorTextColor ?? AppColors.errorText(),
              fontSize: 12,
            )
          : TextStyle(
              color: widget.errorTextColor ?? AppColors.errorText(),
            ),
      prefixIcon: widget.iconLeading != null
          ? Icon(
              widget.iconLeading,
              color: widget.prefixIconColor ?? AppColors.hintTextColor(),
              size: 20,
            )
          : null,
      prefixStyle:
          const TextStyle(color: Colors.red, backgroundColor: Colors.white),
      border: OutlineInputBorder(
        borderRadius: BorderRadius.circular(widget.borderRadius),
      ),
      focusedBorder: OutlineInputBorder(
          borderRadius: BorderRadius.circular(widget.borderRadius),
          borderSide: BorderSide(color: widget.colorBorderText)),
      enabledBorder: OutlineInputBorder(
          borderRadius: BorderRadius.circular(widget.borderRadius),
          borderSide: BorderSide(color: widget.colorBorderText)),
      contentPadding:
          widget.contentPadding ?? const EdgeInsets.all(AppDimens.paddingSmall),
      suffixIcon: _suffixIcon(),
      floatingLabelBehavior: widget.floatingLabelBehavior,
      floatingLabelStyle: widget.floatingLabelStyle,
    );
  }
}
