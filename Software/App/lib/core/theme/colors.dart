import 'package:flutter/material.dart';

class AppColors {
  static const Color mainColors = Color.fromRGBO(242, 103, 36, 1);
  static const Color basicGrey1 = Color(0xFF7E7E7E);
  static const Color basicGrey2 = Color(0xFFA9A9A9);
  static const Color basicGrey4 = Color(0xFFF4F4F4);

  static const Color basicWhite = Color(0xFFFFFFFF);
  static const Color basicBlack = Color(0xFF000000);

  static const Color primaryNavy = Color(0xFF1C1E66);
  static const Color primaryCam1 = Color(0xFFFD5C00);

  static const Color secondaryNavyPastel = Color(0xFFF1F3FF);
  static const Color secondaryCamPastel2 = Color(0xFFFFF3EC);

  static const Color redDark = Color(0xFFD32F2F);

  // for Light Theme
  static const lightPrimaryColor = Color(0xFFeff6ff);
  static const lightAccentColor = Color(0xFFf0eff2);
  static const stickyHeadLight = Color(0xFFffffff);

  // for Light Theme
  static const darkPrimaryColor = Color(0xFF3e4161);
  static const darkAccentColor = Color(0xFF25273f);

  static const Color errorTextColor = Color(0xFFFFD54F);
  static const Color fillColor = Colors.white70;
  static const Color colorError = Color(0xFFff5f6d);

  static bool _isDarkMode() => false;
  static Color appBarColor() => _isDarkMode() ? basicGrey4 : basicGrey1;
  static Color accentColorTheme(bool isDark) =>
      isDark ? basicGrey4 : basicGrey4;
  static Color textColor() => _isDarkMode() ? Colors.white : Colors.black;
  static Color inputText() =>
      _isDarkMode() ? darkPrimaryColor : lightAccentColor;
  static Color hintTextColor() =>
      _isDarkMode() ? Colors.white54 : Colors.black54;
  static Color errorText() => _isDarkMode() ? errorTextColor : Colors.redAccent;

  static const Color colorTransparent = Colors.transparent;

  static const List<Color> colorGradientBlue = [
    Color(0xFF58a0ff),
    Color(0xFF5967ff),
  ];
}
