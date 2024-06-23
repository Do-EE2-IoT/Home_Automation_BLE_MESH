import 'package:flutter/cupertino.dart';
import 'package:get/get.dart';

import '../../../share/share.src.dart';
import '../../core.src.dart';


abstract class BaseGetWidget<T extends BaseGetxController> extends GetView<T> {
  const BaseGetWidget({Key? key}) : super(key: key);

  Widget buildWidgets(BuildContext context);

  @override
  Widget build(BuildContext context) {
    return buildWidgets(context);
  }

  Widget baseShowLoading(WidgetCallback child, {Color? colorIcon}) {
    return Obx(
      () => controller.isShowLoading.value
          ? Center(child: UtilWidget.buildLoading(colorIcon: colorIcon))
          : child(),
    );
  }

  // Widget baseShimmerLoading(WidgetCallback child, {Widget? shimmer}) {
  //   return Obx(
  //     () => controller.isShowLoading.value
  //         ? shimmer ?? UtilWidget.buildShimmerLoading()
  //         : child(),
  //   );
  // }

  // Widget buildLoadingOverlay(WidgetCallback child, {Color? colorIcon}) {
  //   return Obx(
  //     () => Stack(
  //       children: [
  //         LoadingOverlayPro(
  //           progressIndicator: UtilWidget.buildLoading(colorIcon: colorIcon),
  //           isLoading: controller.isLoadingOverlay.value,
  //           child: child(),
  //         ),
  //       ],
  //     ),
  //   );
  // }

  // static Widget listEmpty() {
  //   return const Column(
  //     mainAxisAlignment: MainAxisAlignment.center,
  //     children: [
  //       FaIcon(
  //         FontAwesomeIcons.circleExclamation,
  //         size: 30,
  //         color: DefaultTheme.greyText,
  //       ),
  //       Text(
  //         AppStr.emptyList,
  //         style: TextStyle(
  //           fontSize: 20,
  //           color: DefaultTheme.greyText,
  //           fontWeight: FontWeight.bold,
  //         ),
  //       ),
  //     ],
  //   );
  // }
}
