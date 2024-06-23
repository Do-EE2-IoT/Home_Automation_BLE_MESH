import '../../../lib.dart';
import '../home.src.dart';

class HomePage extends BaseGetWidget<HomeController> {
  const HomePage({super.key});
  @override
  HomeController get controller => Get.find<HomeController>();
  @override
  Widget buildWidgets(BuildContext context) {
    return Scaffold(
      body: Column(
        children: [
          SizedBox(
            width: Get.width,
            child: Row(
              mainAxisSize: MainAxisSize.min,
              mainAxisAlignment: MainAxisAlignment.end,
              children: [
                UtilWidget.buildButton("Đăng xuất", () {
                  Get.offAllNamed(AppRoutes.routeLogin);
                },
                    width: 120,
                    height: 40,
                    borderRadius: const BorderRadius.all(Radius.circular(30))),
              ],
            ),
          ).paddingOnly(top: AppDimens.paddingSearchBar)
        ],
      ),
    );
  }
}
