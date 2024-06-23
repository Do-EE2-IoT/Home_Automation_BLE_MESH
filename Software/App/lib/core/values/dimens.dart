import 'dart:io';

import 'package:flutter/material.dart';
import 'package:get/get.dart';

class AppDimens {
  static double fontSize10() => 10.divSF;
  static double fontSmallest() => 12.divSF;
  static double fontSmall() => 14.divSF;
  static double fontMedium() => 16.divSF;
  static double fontBig() => 18.divSF;
  static double fontBiggest() => 20.divSF;
  static double fontSize24() => 24.divSF;

  static const double sizeImage = 50;
  static const double sizeImageMedium = 70;
  static const double sizeImageBig = 90;
  static const double sizeImageLarge = 200;

  static const double sizeTextSmall = 12;
  static const double sizeTextDefualt = 14;
  static const double sizeTextMedium = 18;
  static const double sizeIconMedium = 25;
  static const double sizeTextMediumTb = 16;
  static const double sizeTextSmallTb = 16;
  static const double sizeTextLarge = 20;
  static const double sizeTextLarger = 22;
  static const double sizeTextLargerDaily = 24;
  static const double btnSmall = 20;
  static const double btnMediumTb = 35;
  static const double btnMedium = 50;
  static const double btnLarge = 70;

  static const double sizeIconVerySmall = 12;
  static const double sizeIconSmall = 20;
  static const double sizeIcon = 20;
  static const double sizeBodyMedium = 23;
  static const double sizeIconMedium25 = 25;
  static const double sizeIconSpinner = 30;
  static const double sizeIconLarge = 36;
  static const double sizeIconBig = 80;
  static const double sizeIconExtraLarge = 200;
  static const double sizeDialogNotiIcon = 40;

  static const double heightChip = 30;
  static const double widthChip = 100;
  static const double withContainer = 100;
  static const double heightContainer = 100;

  static const int maxLengthDescription = 250;

  static const double defaultPadding = 16.0;
  static const double paddingVerySmall = 8.0;
  static const double paddingVerySmallest = 4.0;
  static const double paddingSmall = 12.0;
  static const double paddingMedium = 20.0;
  static const double paddingHuge = 32.0;
  static const double paddingVeryHuge = 48.0;
  static const double paddingItemList = 18.0;
  static const double borderSmall = 25.0;

  static const double showAppBarDetails = 200;
  static const double sizeAppBarBig = 120;
  static const double sizeAppBarMedium = 92;
  static const double sizeAppBar = 72;
  static const double sizeAppBarSmall = 44;

  static const double radiusImg = 220.0;
  static const double sizeText = 16;

  // radiusBorder
  static const double radius8 = 8;
  static const double radius5 = 5;
  static const double radius12 = 12;
  static const double radius20 = 20;
  static const double radius50 = 50;

  // home
  static const double sizeItemNewsHome = 110;
  static const double heightImageLogoHome = 50;
  // divider
  static const double paddingDivider = 15.0;

  // appbar
  static const double paddingSearchBarBig = 50;
  static const double paddingSearchBar = 45;
  static const double paddingSearchBarMedium = 30;
  static const double paddingSearchBarSmall = 12;

  //other
  static const double paddingTitleAndTextForm = 3;
  static double bottomPadding() {
    return Platform.isIOS ? AppDimens.paddingMedium : AppDimens.paddingSmall;
  }

  //MENU
  //Positioned
  static const double bottomPositioned = -5;
  static const double rightPositioned = 18;
  static const double opacityPositioned = 0.5;
  static const double heightPositioned = 30;
  static const double widthPositioned = 30;
  //font Weight
  static const fontWeightMedium = FontWeight.w500;
  static const fontWeightBold = FontWeight.bold;
  //padding
  static const double paddingOnlyLeft = 55.0;
  static const double paddingOnlyRight = 8.0;
  static const double paddingOnlyHorizontal = 8.0;
  static const double padding3Horizontal = 3.0;
  static const double paddingOnlyTop150 = 150.0;
  static const double paddingBottom = 60.0;

  //Sizedbox
  static const double widthSizedBox = 80.0;
  static const double heightSizedBox = 30.0;

  static const double borderRadiusBigger = 10.0;
  static const double borderRadiusBig = 8.0;
  static const double borderRadiusMed = 5.0;
  static const double borderRadiusSmall = 2.0;
  static const double borderCardDefault = 15.0;
  
  // elevation
  static const double elevationSmall = 1.0;
  static const double elevationMed = 1.5;
  static const double elevationLarge = 2.0;

  static const int filterHeight = 6;
  static const double heightBottomTabBar = 80;
  static const double heightBottomTabBarAndroid = 65;
  static const double sizeCustomIndicator = 50.0;
  static const double sizeBorderNavi = 30.0;
  static const double paddingTabBar = 5.0;
  static const double paddingSmallBottomNavigation = 6.0;
  static const double paddingBottomTabBar = 18.0;
  static const double sizeBottomNavi80 = 80.0;

  // dropdown
  static const double heightDropDownDefault = 30.0;
}

extension GetSizeScreen on num {
  /// Tỉ lệ fontSize của các textStyle
  double get divSF {
    return this / Get.textScaleFactor;
  }

  // Tăng chiều dài theo font size
  double get mulSF {
    return this * Get.textScaleFactor;
  }
}
