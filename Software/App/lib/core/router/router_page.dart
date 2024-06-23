
import '../../lib.dart';
import '../../modules/modules.src.dart';

class RoutePage {
  static var routes = [
    GetPage(
      name: AppRoutes.initApp,
      page: () => const SplashPage(),
      transition: Transition.cupertinoDialog,
    ),
    GetPage(
      name: AppRoutes.routeLogin,
      page: () => const LoginPage(),
      transition: Transition.cupertinoDialog,
    ),
    GetPage(
      name: AppRoutes.routeSignup,
      page: () => const SignupPage(),
    ),
    GetPage(
      name: AppRoutes.routeHome,
      page: () => const HomePage(),
      transition: Transition.cupertinoDialog,
    ),
  ];
}
