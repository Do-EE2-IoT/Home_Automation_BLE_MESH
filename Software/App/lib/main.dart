import 'package:aithings/asset/asset.src.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/services.dart';
import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:get/get.dart';

import 'core/core.src.dart';
import 'share/share.src.dart';

Future<void> main() async {
  runApp(const Application());
}

class Application extends StatefulWidget {
  const Application({Key? key}) : super(key: key);

  @override
  State<Application> createState() => _Application();
}

class _Application extends State<Application> {
  @override
  void initState() {
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.portraitUp,
    ]);
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: KeyBoard.hide,
      child: GetMaterialApp(
        locale: const Locale('vi', 'VN'),
        debugShowCheckedModeBanner: false,
        initialRoute: AppRoutes.initApp,
        getPages: RoutePage.routes,
        builder: (context, child) => ScrollConfiguration(
          behavior: MyBehavior(),
          child: MediaQuery(
              data:
                  MediaQuery.of(context).copyWith(alwaysUse24HourFormat: true),
              child: child ?? Container()),
        ),
        localizationsDelegates: const [
          GlobalCupertinoLocalizations.delegate,
          GlobalMaterialLocalizations.delegate,
          GlobalWidgetsLocalizations.delegate,
          DefaultCupertinoLocalizations.delegate
        ],
        supportedLocales: const [
          Locale('vi', ''),
          Locale('en', ''),
        ],
        title: AppStr.appName,
        theme: getThemeByAppTheme(false).copyWith(),
      ),
    );
  }
}

class MyBehavior extends ScrollBehavior {
  @override
  Widget buildOverscrollIndicator(
    BuildContext context,
    Widget child,
    ScrollableDetails details,
  ) {
    return child;
  }
}
