import 'package:flutter/material.dart';

import '../values/dimens.dart';

class AppTextStyle {
  static TextStyle labelStyle({
    double? size,
    Color? color,
    TextOverflow? overflow,
    double? height,
  }) {
    return TextStyle(
      color: color ?? Colors.black,
      fontSize: size ?? AppDimens.sizeTextMediumTb,
      overflow: overflow ?? TextOverflow.ellipsis,
      height: height ?? 1,
    );
  }
}
