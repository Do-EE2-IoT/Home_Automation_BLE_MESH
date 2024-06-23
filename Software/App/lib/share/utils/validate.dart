bool isPasswordValidate({
  required String password,
  required int minLength,
  int? maxLength,
}) {
  if (password.isEmpty) return false;
  String rawRegExp =
      r"^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[@$!%*?&])[A-Za-z\d@$!%*?&]";
  // Add range requirement {minlength,maxlength}
  String range =
      "{$minLength,${(maxLength != null) ? maxLength.toString() : ''}}";
  // Trường hợp có yêu cầu nhập tối đa vào mật khẩu.
  if (!RegExp(rawRegExp + range).hasMatch(password)) return false;
  return true;
}

extension StringUtils on String? {
  bool get isStringNotEmpty => this != null && this!.trim().isNotEmpty;

  bool get isNullOrEmpty => this == null || (this?.trim().isEmpty ?? true);
}

bool isListNotEmpty(List<dynamic>? list) => list != null && list.isNotEmpty;

String convertDoubleToStringSmart(double? value) {
  return '${value?.toInt() == value ? value?.toInt() : value}';
}

bool isPhoneValidate({required String? value}) {
  if (value == null) return false;
  if (value.trim().isEmpty ||
      !RegExp(r'(?:[+0]9)?[0-9]{10}$').hasMatch(value)) {
    return false;
  }

  return true;
}

bool isEmail(String? value) {
  RegExp email = RegExp(
      r"^((([a-z]|\d|[!#\$%&'\*\+\-\/=\?\^_`{\|}~]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])+(\.([a-z]|\d|[!#\$%&'\*\+\-\/=\?\^_`{\|}~]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])+)*)|((\x22)((((\x20|\x09)*(\x0d\x0a))?(\x20|\x09)+)?(([\x01-\x08\x0b\x0c\x0e-\x1f\x7f]|\x21|[\x23-\x5b]|[\x5d-\x7e]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(\\([\x01-\x09\x0b\x0c\x0d-\x7f]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]))))*(((\x20|\x09)*(\x0d\x0a))?(\x20|\x09)+)?(\x22)))@((([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.)+(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))$");

  if (value == null) return false;
  if (value.trim().isEmpty || !email.hasMatch(value.toLowerCase())) {
    return false;
  }

  return true;
}
