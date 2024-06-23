import 'package:flutter/material.dart';
import 'package:get/get.dart';

import '../../core/core.src.dart';

class SplashPage extends GetView<AppController> {
  const SplashPage({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    Get.put(AppController(), permanent: true);
    return Container(
      color: AppColors.basicGrey4,
      alignment: Alignment.center,
      child: const Center(
        child: DefaultTextStyle(
          style: TextStyle(
            color: Colors.black,
            fontSize: 24,
          ),
          child: Text("AiThings"),
        ),
      ),
    );
  }
}
