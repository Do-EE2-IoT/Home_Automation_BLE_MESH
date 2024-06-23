// import 'package:hive/hive.dart';
import 'package:package_info_plus/package_info_plus.dart';

import '../../../lib.dart';

// late Box hiveApp;

late PackageInfo packageInfo;

class AppController extends GetxController {
  @override
  Future<void> onInit() async {
    super.onInit();
    // initHive().then((value) async {
    //   // await initConfig();
    Get.put(BaseRequest(), permanent: true);
    //   try {
    //     // LoginController loginController = Get.put(LoginController());
    //     // await loginController.loginUserFromLogin(
    //     //     LoginModelRequest(
    //     //       username: hiveApp.get(AppConst.userName),
    //     //       password: hiveApp.get(AppConst.password),
    //     //     ),
    //     //     isLoginFromApp:
    //     //         true);
    //     Get.offAndToNamed(AppRoutes.routeLogin);
    //   } catch (e) {
    //     // Get.offAndToNamed(AppRoutes.routeLogin);
    //   }
    // });
    Future.delayed(const Duration(seconds: 2)).then((__) {
      Get.toNamed(AppRoutes.routeLogin);
    });
  }
}

Future<void> initHive() async {
  // WidgetsFlutterBinding.ensureInitialized();
  // final appDocumentDirectory =
  //     await path_provider.getApplicationDocumentsDirectory();
  // Hive.init(appDocumentDirectory.path);
  // hiveApp = await Hive.openBox(LocaleKeys.app_name.tr);
  // registerAdapters();
  // await openBox();
  packageInfo = await PackageInfo.fromPlatform();
}

// Future<void> openBox() async {
//   hiveUserLogin = await Hive.openBox(HiveAdapters.loginCaRequestModel);
// }